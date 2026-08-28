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
	
	SaveDashboard(json);
	SaveAssets(json);

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

	LoadDashboard(json);
	LoadAssets(json);
}

void Serializer::SaveDashboard(nlohmann::json& _json)
{
	Wallet& wallet = Wallet::GetInstance();
		
	// Entry investment table
	_json["entry"]["monthly_investment"] = wallet.entryOverview.monthlyInvestment;

	_json["entry"]["percentages"] = nlohmann::json::array();
	for (const auto& [isin, percentage] : wallet.entryOverview.assetsPercentage)
		_json["entry"]["percentages"].push_back({
			{ "isin", isin },
			{ "percentage", percentage }
		});
}

void Serializer::SaveAssets(nlohmann::json& _json)
{
	_json["assets"] = nlohmann::json::array();

	const std::vector<Asset>& assets = Wallet::GetInstance().GetAssets();
	for (const Asset& asset : assets)
	{
		_json["assets"].push_back({
			{ "name", asset.name },
			{ "isin", asset.isin },
			{ "ticker", asset.ticker },
			{ "subticker", asset.subticker },
			{ "broker", asset.broker }
		});
	}
}

void Serializer::LoadDashboard(const nlohmann::json& _json)
{
	EntryOverview eo;
	eo.monthlyInvestment = _json["entry"]["monthly_investment"];
	for (int i = 0; i < _json["entry"]["percentages"].size(); i++)
		eo.assetsPercentage.emplace(_json["entry"]["percentages"][i]["isin"], _json["entry"]["percentages"][i]["percentage"]);

	Wallet::GetInstance().entryOverview = eo;
}

void Serializer::LoadAssets(const nlohmann::json& _json)
{
	Wallet::GetInstance().DeleteAllAssets();
	for (int i = 0; i < _json["assets"].size(); i++)
	{
		Asset asset;
		asset.name = _json["assets"][i]["name"];
		asset.isin = _json["assets"][i]["isin"];
		asset.ticker = _json["assets"][i]["ticker"];
		asset.subticker = _json["assets"][i]["subticker"];
		asset.broker = _json["assets"][i]["broker"];
		Wallet::GetInstance().AddAsset(asset);
	}
}
