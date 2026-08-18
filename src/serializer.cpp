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
	// get save directory
	std::filesystem::path saveDir;
	if (const char* home = std::getenv("HOME"))
		saveDir = std::filesystem::path(home) / ".local" / "share";
	else
	{
		std::cout << "ERROR: couldn't find save directory" << std::endl;
		return;
	}

	// save all assets
	nlohmann::json data;
	data["assets"] = nlohmann::json::array();

	std::vector<Asset> assets = Wallet::GetInstance().GetAssets();
	for (const Asset& asset : assets)
	{
		data["assets"].push_back({
			{ "name", asset.name },
			{ "isin", asset.isin },
			{ "ticker", asset.ticker },
			{ "broker", asset.broker }
		});
	}

	std::ofstream file (saveDir / "wallet.json");
	file << data.dump(8);
	std::cout << "saved all data to " << saveDir << " as:\n" << data.dump(8) << std::endl;
}

void Serializer::LoadData()
{
	std::cout << "loaded all data" << std::endl;
}
