#include "wallet.h"

#include <iostream>


Wallet& Wallet::GetInstance()
{
	static Wallet instance;
	return instance;
}

void Wallet::AddNewAsset(const Asset _asset)
{
	assets_.push_back(_asset);
}

void Wallet::DeleteAsset(const Asset& _asset)
{
	for (int i = 0; i < assets_.size(); i++)
	{
		if (_asset.isin == assets_[i].isin)
		{
			assets_.erase(assets_.begin() + i);
			break;
		}
	}
}

const std::vector<Asset>& Wallet::GetAssets() const
{
	return assets_;
}
