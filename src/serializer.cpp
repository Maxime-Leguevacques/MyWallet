#include "serializer.h"

#include <iostream>
#include <filesystem>
#include <fstream>

#include "nlohmann/json.hpp"

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
		saveFile = std::filesystem::path(home) / ".local" / "share";
	else
	{
		std::cout << "ERROR: couldn't find home directory" << std::endl;
		return;
	}

	// create json save
	nlohmann::json json;
	
	// save dashboard
	
	// save all assets
	json["assets"] = nlohmann::json::array();

	std::vector<Asset> assets = Wallet::GetInstance().GetAssets();
	for (const Asset& asset : assets)
	{
		json["assets"].push_back({
			{ "name", asset.name },
			{ "isin", asset.isin },
			{ "ticker", asset.ticker },
			{ "broker", asset.broker }
		});
	}

	// write to file
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
		saveFile = std::filesystem::path(home) / ".local" / "share" / "wallet.json";
	}
	else
	{
		std::cout << "ERROR: couldn't find save file" << std::endl;
		return;
	}
	
	// load json file to json object
	std::ifstream ifs(saveFile);
	nlohmann::json json = nlohmann::json::parse(ifs);

	// replace wallet assets
	Wallet::GetInstance().DeleteAllAssets();
	for (int i = 0; i < json["assets"].size(); i++)
	{
		Asset asset;
		asset.name = json["assets"][i]["name"];
		asset.isin = json["assets"][i]["isin"];
		asset.ticker = json["assets"][i]["ticker"];
		asset.broker = json["assets"][i]["broker"];
		Wallet::GetInstance().AddAsset(asset);
	}
}

void Serializer::SaveDashboard()
{

}

void Serializer::SaveAssets()
{

}

void Serializer::LoadDashboard()
{

}

void Serializer::LoadAssets()
{

}
