#pragma once

#include <vector>

#include "asset.h"



class Wallet
{
private:
	std::vector<Asset> assets_;

private:
	Wallet() = default;
	~Wallet() = default;

public:
	Wallet(const Wallet&) = delete;
	Wallet& operator=(const Wallet&) = delete;
	static Wallet& GetInstance();

	void AddNewAsset();

	const std::vector<Asset>& GetAssets() const;
};
