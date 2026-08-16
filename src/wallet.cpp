#include "wallet.h"

#include <iostream>


Wallet& Wallet::GetInstance()
{
	static Wallet instance;
	return instance;
}

void Wallet::AddNewAsset()
{
	std::cout << "added new asset" << std::endl;
	Asset newAsset;
	assets_.push_back(newAsset);
}

const std::vector<Asset>& Wallet::GetAssets() const
{
	return assets_;
}
