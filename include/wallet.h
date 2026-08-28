#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "asset.h"
#include "order.h"


struct EntryOverview
{
	float monthlyInvestment;
	std::unordered_map<std::string, float> assetsPercentage;
};


class Wallet
{
public:
	EntryOverview entryOverview;

private:
	std::vector<Asset> assets_;
	std::vector<Order> orders_;

private:
	Wallet() = default;
	~Wallet() = default;

public:
	Wallet(const Wallet&) = delete;
	Wallet& operator=(const Wallet&) = delete;
	static Wallet& GetInstance();

	void AddAsset(const Asset _asset);
	void DeleteAsset(const Asset& _asset);
	void DeleteAllAssets();

	const std::vector<Asset>& GetAssets() const;
};
