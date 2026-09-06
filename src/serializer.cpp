#include "serializer.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstdlib>

#include "wallet.h"
#include "order_manager.h"


Serializer& Serializer::GetInstance()
{
	static Serializer instance;
	return instance;
}

void Serializer::SaveData()
{
	const std::optional<std::filesystem::path> saveDirectory = GetSaveDirectory();
	if (!saveDirectory)
	{
		std::cerr << "ERROR: couldn't find home directory" << std::endl;
		return;
	}

	std::error_code ec;
	std::filesystem::create_directories(*saveDirectory, ec);
	if (ec)
	{
		std::cerr << "ERROR: couldn't create save directory: " << ec.message() << std::endl;
		return;
	}

	// create json save
	nlohmann::json json;
	SaveWallet(json);

	std::ofstream file(*saveDirectory / "wallet.json");
	if (!file.is_open())
	{
		std::cerr << "ERROR: couldn't open wallet save file for writing" << std::endl;
		return;
	}
	file << json.dump(8);
}

void Serializer::LoadData()
{
	const std::optional<std::filesystem::path> saveDirectory = GetSaveDirectory();
	if (!saveDirectory)
	{
		std::cerr << "ERROR: couldn't find home directory" << std::endl;
		return;
	}

	const std::filesystem::path saveFile = *saveDirectory / "wallet.json";

	std::error_code ec;
	if (!std::filesystem::exists(saveFile, ec) || ec)
		return;

	std::ifstream ifs(saveFile);
	if (!ifs.is_open())
	{
		std::cerr << "ERROR: couldn't open wallet save file for reading" << std::endl;
		return;
	}

	nlohmann::json json;
	try
	{
		ifs >> json;
	}
	catch (const std::exception& exception)
	{
		std::cerr << "ERROR: couldn't parse wallet save file: " << exception.what() << std::endl;
		return;
	}

	EntryOverview parsedEntryOverview;
	std::vector<Asset> parsedAssets;
	std::vector<Order> parsedOrders;

	if (!ParseEntryOverview(json, parsedEntryOverview))
	{
		std::cerr << "ERROR: invalid save file entry section" << std::endl;
		return;
	}
	if (!ParseAssets(json, parsedAssets))
	{
		std::cerr << "ERROR: invalid save file assets section" << std::endl;
		return;
	}
	if (!ParseOrders(json, parsedOrders))
	{
		std::cerr << "ERROR: invalid save file orders section" << std::endl;
		return;
	}

	Wallet::GetInstance().entryOverview = parsedEntryOverview;
	Wallet::GetInstance().ClearAssets();
	for (const Asset& asset : parsedAssets)
		Wallet::GetInstance().AddAsset(asset);

	OrderManager::GetInstance().ClearOrders();
	for (const Order& order : parsedOrders)
		OrderManager::GetInstance().AddOrder(order);
}

void Serializer::SaveWallet(nlohmann::json& _json)
{
	Wallet& wallet = Wallet::GetInstance();
		
	// entry investment overview table
	_json["entry"]["monthly_investment"] = wallet.entryOverview.monthlyInvestment;

	_json["entry"]["percentages"] = nlohmann::json::array();
	for (const auto& [isin, percentage] : wallet.entryOverview.assetsPercentage)
		_json["entry"]["percentages"].push_back({
			{ "isin", isin },
			{ "percentage", percentage }
		});
	
	// assets
	_json["assets"] = nlohmann::json::array();

	const std::vector<Asset>& assets = Wallet::GetInstance().GetAssets();
	for (const Asset& asset : assets)
	{
		_json["assets"].push_back({
			{ "name", asset.name },
			{ "isin", asset.isin },
			{ "ticker", asset.ticker },
			{ "broker", asset.broker },
		});
	}

	// orders
	_json["orders"] = nlohmann::json::array();
	const std::vector<Order>& orders = OrderManager::GetInstance().GetOrders();
	for (const Order& order : orders)
	{
		_json["orders"].push_back({
			{ "isin", order.isin },
			{ "quantity", order.quantity },
			{ "position_after_trade", order.positionAfterTrade },
			{ "day", order.day },
			{ "month", order.month },
			{ "year", order.year },
			{ "hour", order.hour },
			{ "minute", order.minute }
		});
	}
}

std::optional<std::filesystem::path> Serializer::GetSaveDirectory()
{
	const char* home = std::getenv("HOME");
	if (home == nullptr)
		return std::nullopt;
	return std::filesystem::path(home) / ".local" / "share" / "MyWallet";
}

bool Serializer::ParseEntryOverview(const nlohmann::json& _json, EntryOverview& _entryOverview)
{
	if (!_json.contains("entry") || !_json["entry"].is_object())
		return false;

	const nlohmann::json& entryJson = _json["entry"];
	if (!entryJson.contains("monthly_investment") || !entryJson["monthly_investment"].is_number())
		return false;
	if (!entryJson.contains("percentages") || !entryJson["percentages"].is_array())
		return false;

	EntryOverview entryOverview;
	entryOverview.monthlyInvestment = entryJson["monthly_investment"].get<float>();

	for (const nlohmann::json& percentageItem : entryJson["percentages"])
	{
		if (!percentageItem.is_object())
			return false;
		if (!percentageItem.contains("isin") || !percentageItem["isin"].is_string())
			return false;
		if (!percentageItem.contains("percentage") || !percentageItem["percentage"].is_number())
			return false;

		entryOverview.assetsPercentage[percentageItem["isin"].get<std::string>()] =
			percentageItem["percentage"].get<float>();
	}

	_entryOverview = std::move(entryOverview);
	return true;
}

bool Serializer::ParseAssets(const nlohmann::json& _json, std::vector<Asset>& _assets)
{
	if (!_json.contains("assets") || !_json["assets"].is_array())
		return false;

	std::vector<Asset> assets;
	for (const nlohmann::json& assetJson : _json["assets"])
	{
		if (!assetJson.is_object())
			return false;
		if (!assetJson.contains("name") || !assetJson["name"].is_string())
			return false;
		if (!assetJson.contains("isin") || !assetJson["isin"].is_string())
			return false;
		if (!assetJson.contains("ticker") || !assetJson["ticker"].is_string())
			return false;
		if (!assetJson.contains("broker") || !assetJson["broker"].is_string())
			return false;

		Asset asset;
		asset.name = assetJson["name"].get<std::string>();
		asset.isin = assetJson["isin"].get<std::string>();
		asset.ticker = assetJson["ticker"].get<std::string>();
		asset.broker = assetJson["broker"].get<std::string>();
		assets.push_back(asset);
	}

	_assets = std::move(assets);
	return true;
}

bool Serializer::ParseOrders(const nlohmann::json& _json, std::vector<Order>& _orders)
{
	if (!_json.contains("orders") || !_json["orders"].is_array())
		return false;

	std::vector<Order> orders;
	for (const nlohmann::json& orderJson : _json["orders"])
	{
		if (!orderJson.is_object())
			return false;
		if (!orderJson.contains("isin") || !orderJson["isin"].is_string())
			return false;
		if (!orderJson.contains("quantity") || !orderJson["quantity"].is_number())
			return false;
		if (!orderJson.contains("position_after_trade") || !orderJson["position_after_trade"].is_number())
			return false;
		if (!orderJson.contains("day") || !orderJson["day"].is_number_integer())
			return false;
		if (!orderJson.contains("month") || !orderJson["month"].is_number_integer())
			return false;
		if (!orderJson.contains("year") || !orderJson["year"].is_number_integer())
			return false;
		if (!orderJson.contains("hour") || !orderJson["hour"].is_number_integer())
			return false;
		if (!orderJson.contains("minute") || !orderJson["minute"].is_number_integer())
			return false;

		Order order;
		order.isin = orderJson["isin"].get<std::string>();
		order.quantity = orderJson["quantity"].get<float>();
		order.positionAfterTrade = orderJson["position_after_trade"].get<float>();
		order.day = orderJson["day"].get<int>();
		order.month = orderJson["month"].get<int>();
		order.year = orderJson["year"].get<int>();
		order.hour = orderJson["hour"].get<int>();
		order.minute = orderJson["minute"].get<int>();
		orders.push_back(order);
	}

	_orders = std::move(orders);
	return true;
}
