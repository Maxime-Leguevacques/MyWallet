#include "wallet.h"

#include <iostream>


Wallet& Wallet::GetInstance()
{
	static Wallet instance;
	return instance;
}

bool Wallet::CanAddAsset(const Asset& _asset)
{
	if (_asset.name.empty() || _asset.isin.empty() || _asset.ticker.empty() || _asset.broker.empty())
		return false;

	// check if isin or ticker already exists
	for (const Asset& asset : assets_)
		if (asset.isin == _asset.isin || asset.ticker == _asset.ticker)
			return false;
	
	return true;
}

void Wallet::AddAsset(const Asset& _asset)
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

void Wallet::DeleteAssets()
{
	std::vector<Asset> newAssets;
	assets_ = newAssets;
}

std::string Wallet::TickerToIsin(const std::string& _ticker)
{
	for (const Asset& asset : Wallet::GetInstance().GetAssets())
		if (asset.ticker == _ticker)
			return asset.isin;
	return "";
}

std::string Wallet::IsinToTicker(const std::string& _isin)
{
	for (const Asset& asset : Wallet::GetInstance().GetAssets())
		if (asset.isin == _isin)
			return asset.ticker;
	return "";
}

Asset* Wallet::GetAsset(const std::string& _isin)
{
	for (Asset& asset : assets_)
		if (asset.isin == _isin)
			return &asset;
	
	return nullptr;
}

const std::vector<Asset>& Wallet::GetAssets() const
{
	return assets_;
}
