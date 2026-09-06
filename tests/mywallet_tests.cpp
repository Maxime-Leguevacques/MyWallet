#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "order_manager.h"
#include "serializer.h"
#include "wallet.h"

namespace
{
void ResetState()
{
	Wallet::GetInstance().ClearAssets();
	Wallet::GetInstance().entryOverview = EntryOverview{};
	OrderManager::GetInstance().ClearOrders();
}

void SeedAssets()
{
	Wallet::GetInstance().AddAsset({ .name = "Asset A", .isin = "ISIN_A", .ticker = "AAA", .broker = "Broker A", .positions = {} });
	Wallet::GetInstance().AddAsset({ .name = "Asset B", .isin = "ISIN_B", .ticker = "BBB", .broker = "Broker B", .positions = {} });
}
}

int main()
{
	// Wallet mapping and validation
	ResetState();
	SeedAssets();
	assert(Wallet::TickerToIsin("AAA") == "ISIN_A");
	assert(Wallet::IsinToTicker("ISIN_B") == "BBB");
	assert(Wallet::TickerToIsin("UNKNOWN").empty());
	assert(!Wallet::GetInstance().CanAddAsset({ .name = "Asset C", .isin = "ISIN_A", .ticker = "CCC", .broker = "Broker C", .positions = {} }));
	assert(!Wallet::GetInstance().CanAddAsset({ .name = "", .isin = "ISIN_C", .ticker = "CCC", .broker = "Broker C", .positions = {} }));

	// Order timeline and reset consistency
	Order order1{ .isin = "ISIN_A", .quantity = 50.0f, .positionAfterTrade = 50.0f, .day = 1, .month = 1, .year = 2026, .hour = 10, .minute = 0 };
	Order order2{ .isin = "ISIN_B", .quantity = 100.0f, .positionAfterTrade = 100.0f, .day = 2, .month = 1, .year = 2026, .hour = 10, .minute = 0 };
	assert(OrderManager::GetInstance().CanAddOrder(order1));
	OrderManager::GetInstance().AddOrder(order1);
	OrderManager::GetInstance().AddOrder(order2);
	assert(OrderManager::GetInstance().GetOrders().size() == 2);
	assert(OrderManager::GetInstance().GetWalletPositions().size() == 2);
	assert(OrderManager::GetInstance().GetCurrentWalletPositions() != nullptr);
	OrderManager::GetInstance().ClearOrders();
	assert(OrderManager::GetInstance().GetOrders().empty());
	assert(OrderManager::GetInstance().GetWalletPositions().empty());
	assert(OrderManager::GetInstance().GetCurrentWalletPositions() == nullptr);

	// Serializer roundtrip and load regressions
	ResetState();
	SeedAssets();
	Wallet::GetInstance().entryOverview.monthlyInvestment = 1234.0f;
	Wallet::GetInstance().entryOverview.assetsPercentage["ISIN_A"] = 60.0f;
	Wallet::GetInstance().entryOverview.assetsPercentage["ISIN_B"] = 40.0f;
	OrderManager::GetInstance().AddOrder(order1);
	OrderManager::GetInstance().AddOrder(order2);

	const std::filesystem::path testHome = std::filesystem::temp_directory_path() / "mywallet_test_home";
	std::filesystem::remove_all(testHome);
	std::filesystem::create_directories(testHome);
	setenv("HOME", testHome.string().c_str(), 1);

	Serializer::SaveData();
	Wallet::GetInstance().entryOverview.monthlyInvestment = 0.0f;
	Wallet::GetInstance().ClearAssets();
	OrderManager::GetInstance().ClearOrders();
	Serializer::LoadData();

	assert(Wallet::GetInstance().entryOverview.monthlyInvestment == 1234.0f);
	assert(Wallet::GetInstance().GetAssets().size() == 2);
	assert(OrderManager::GetInstance().GetOrders().size() == 2);
	assert(OrderManager::GetInstance().GetWalletPositions().size() == 2);

	// Missing save file should not clear loaded data
	const std::filesystem::path saveFile = testHome / ".local" / "share" / "MyWallet" / "wallet.json";
	std::filesystem::remove(saveFile);
	Serializer::LoadData();
	assert(Wallet::GetInstance().GetAssets().size() == 2);
	assert(OrderManager::GetInstance().GetOrders().size() == 2);

	// Malformed save file should not clear loaded data
	{
		std::filesystem::create_directories(saveFile.parent_path());
		std::ofstream ofs(saveFile);
		ofs << "{ malformed json ";
	}
	Serializer::LoadData();
	assert(Wallet::GetInstance().GetAssets().size() == 2);
	assert(OrderManager::GetInstance().GetOrders().size() == 2);

	return 0;
}
