#include "wallet.h"

#include <iostream>


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
	for (int i = 0; i < assets_.size(); i++)
	{
		if (_asset.isin == assets_[i].isin)
		{
			assets_.erase(assets_.begin() + i);
			break;
		}
	}
}

void Wallet::DeleteAllAssets()
{
	std::vector<Asset> newAssets;
	assets_ = newAssets;
}

void Wallet::AddOrder(const Order& _order)
{
	orders_.push_back(_order);
	
	Asset* asset = GetAsset(_order.isin);
	int nbPositions = asset->positions.size();
	int currentPosition = nbPositions > 0 ? asset->positions[nbPositions - 1] : 0.0f;
	currentPosition += _order.quantity;
	asset->positions.push_back(currentPosition);
	GetAsset(_order.isin)->orders.push_back(_order);
}

void Wallet::DeleteAllOrders()
{
	std::vector<Order> newOrders;
	orders_ = newOrders;
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

const std::vector<Order>& Wallet::GetOrders() const
{
	return orders_;
}
