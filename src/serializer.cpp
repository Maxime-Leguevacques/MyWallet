#include "serializer.h"

#include <iostream>
#include <filesystem>
#include <fstream>

#include "wallet.h"


Serializer& Serializer::GetInstance()
{
	static Serializer instance;
	return instance;
}

void Serializer::SaveData()
{
	// get save file
	std::filesystem::path saveFile;
	if (const char* home = std::getenv("HOME"))	// save directory
		saveFile = std::filesystem::path(home) / ".local" / "share" / "MyWallet";
	else
	{
		std::cout << "ERROR: couldn't find home directory" << std::endl;
		return;
	}

	// create json save
	nlohmann::json json;
	SaveWallet(json);

	std::ofstream file (saveFile / "wallet.json");
	file << json.dump(8);
}

void Serializer::LoadData()
{
	// get save file
	std::filesystem::path saveFile;
	if (const char* home = std::getenv("HOME"))
	{
		// need to check if file exists
		saveFile = std::filesystem::path(home) / ".local" / "share" / "MyWallet" / "wallet.json";
	}
	else
	{
		std::cout << "ERROR: couldn't find save file" << std::endl;
		return;
	}

	// load json file to json object
	std::ifstream ifs(saveFile);
	nlohmann::json json = nlohmann::json::parse(ifs);
	LoadWallet(json);
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
			{ "broker", asset.broker }
		});
	}

	// orders
	_json["orders"] = nlohmann::json::array();
	const std::vector<Order>& orders = Wallet::GetInstance().GetOrders();
	for (const Order& order : orders)
	{
		_json["orders"].push_back({
			{ "id", order.id },
			{ "isin", order.isin },
			{ "quantity", order.quantity },
			{ "day", order.day },
			{ "month", order.month },
			{ "year", order.year },
			{ "hour", order.hour },
			{ "minute", order.minute }
		});
	}
}

void Serializer::LoadWallet(const nlohmann::json& _json)
{
	// entry investment overview table
	EntryOverview eo;
	eo.monthlyInvestment = _json["entry"]["monthly_investment"];
	for (int i = 0; i < _json["entry"]["percentages"].size(); i++)
		eo.assetsPercentage.emplace(_json["entry"]["percentages"][i]["isin"], _json["entry"]["percentages"][i]["percentage"]);

	Wallet::GetInstance().entryOverview = eo;
	
	// assets
	Wallet::GetInstance().DeleteAllAssets();
	for (int i = 0; i < _json["assets"].size(); i++)
	{
		Asset asset;
		asset.name = _json["assets"][i]["name"];
		asset.isin = _json["assets"][i]["isin"];
		asset.ticker = _json["assets"][i]["ticker"];
		asset.broker = _json["assets"][i]["broker"];
		Wallet::GetInstance().AddAsset(asset);
	}

	// orders
	Wallet::GetInstance().DeleteAllOrders();
	for (int i = 0; i < _json["orders"].size(); i++)
	{
		Order order;
		order.id = _json["orders"][i]["id"];
		order.isin = _json["orders"][i]["isin"];
		order.quantity = _json["orders"][i]["quantity"];
		order.day = _json["orders"][i]["day"];
		order.month = _json["orders"][i]["month"];
		order.year = _json["orders"][i]["year"];
		order.hour = _json["orders"][i]["hour"];
		order.minute = _json["orders"][i]["minute"];
		Wallet::GetInstance().AddOrder(order);
	}
}
