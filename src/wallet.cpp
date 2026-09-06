#include "wallet.h"

Wallet& Wallet::GetInstance()
{
	static Wallet instance;
	return instance;
}

void Wallet::AddAsset(const Asset& _asset)
{
	assets_.push_back(_asset);
}

void Wallet::DeleteAsset(const Asset& _asset)
{
	for (size_t i = 0; i < assets_.size(); ++i)
	{
		if (_asset.isin == assets_[i].isin)
		{
			assets_.erase(assets_.begin() + i);
			break;
		}
	}
}

void Wallet::ClearAssets()
{
	assets_.clear();
}

bool Wallet::HasIsin(const std::string& _isin) const
{
	for (const Asset& asset : assets_)
	{
		if (asset.isin == _isin)
			return true;
	}
	return false;
}

bool Wallet::HasTicker(const std::string& _ticker) const
{
	for (const Asset& asset : assets_)
	{
		if (asset.ticker == _ticker)
			return true;
	}
	return false;
}

bool Wallet::CanAddAsset(const Asset& _asset) const
{
	if (_asset.name.empty() || _asset.isin.empty() || _asset.ticker.empty() || _asset.broker.empty())
		return false;
	if (HasIsin(_asset.isin) || HasTicker(_asset.ticker))
		return false;
	return true;
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

const Asset* Wallet::GetAsset(const std::string& _isin) const
{
	for (const Asset& asset : assets_)
		if (asset.isin == _isin)
			return &asset;

	return nullptr;
}

const std::vector<Asset>& Wallet::GetAssets() const
{
	return assets_;
}
