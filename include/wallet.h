#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "asset.h"
#include "order.h"


struct EntryOverview
{
	float monthlyInvestment = 0.0f;
	std::unordered_map<std::string, float> assetsPercentage;
};


class Wallet
{
public:
	EntryOverview entryOverview;

private:
	std::vector<Asset> assets_;

public:
	Wallet(const Wallet&) = delete;
	Wallet& operator=(const Wallet&) = delete;
	static Wallet& GetInstance();

	void AddAsset(const Asset& _asset);
	void DeleteAsset(const Asset& _asset);
	void ClearAssets();

	bool HasIsin(const std::string& _isin) const;
	bool HasTicker(const std::string& _ticker) const;
	bool CanAddAsset(const Asset& _asset) const;

	static std::string TickerToIsin(const std::string& _ticker);
	static std::string IsinToTicker(const std::string& _isin);

	Asset* GetAsset(const std::string& _isin);
	const Asset* GetAsset(const std::string& _isin) const;
	const std::vector<Asset>& GetAssets() const;

private:
	Wallet() = default;
	~Wallet() = default;
};
