#pragma once

#include <filesystem>
#include <optional>
#include <vector>

#include "nlohmann/json.hpp"
#include "asset.h"
#include "order.h"
#include "wallet.h"


class Serializer
{
public:
	Serializer(const Serializer&) = delete;
	Serializer& operator=(const Serializer&) = delete;
	static Serializer& GetInstance();

	static void SaveData();
	static void LoadData();

private:
	Serializer() = default;
	~Serializer() = default;

	static std::optional<std::filesystem::path> GetSaveDirectory();
	static void SaveWallet(nlohmann::json& _json);
	static bool ParseEntryOverview(const nlohmann::json& _json, EntryOverview& _entryOverview);
	static bool ParseAssets(const nlohmann::json& _json, std::vector<Asset>& _assets);
	static bool ParseOrders(const nlohmann::json& _json, std::vector<Order>& _orders);
};
