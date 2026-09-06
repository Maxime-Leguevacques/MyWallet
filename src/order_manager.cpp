#include "order_manager.h"

#include "wallet.h"


OrderManager& OrderManager::GetInstance()
{
	static OrderManager instance;
	return instance;
}

void OrderManager::AddOrder(const Order& _order)
{
	if (!CanAddOrder(_order))
		return;

	// first, add to order list
	orders_.push_back(_order);

	// then, update wallet positions 
	// start by checking current wallet position contains an asset
	if (!walletPositions_.empty())
	{
		// if it does, then we create a new wallet position but we keep the other assets position
		std::unordered_map<std::string, float> newPosition = walletPositions_[walletPositions_.size() -1];
		// replace the traded asset
		newPosition[_order.isin] = _order.positionAfterTrade;
		// create new wallet position item
		walletPositions_.push_back(newPosition);
	}
	else
		walletPositions_.push_back({{ _order.isin, _order.positionAfterTrade }});
}

bool OrderManager::CanAddOrder(const Order& _order) const
{
	if (_order.isin.empty())
		return false;
	if (Wallet::GetInstance().GetAsset(_order.isin) == nullptr)
		return false;
	if (_order.quantity <= 0.0f)
		return false;
	if (_order.positionAfterTrade < 0.0f)
		return false;
	return true;
}

void OrderManager::ClearOrders()
{
	orders_.clear();
	walletPositions_.clear();
}

const std::vector<Order>& OrderManager::GetOrders() const
{
	return orders_;
}

const std::vector<std::unordered_map<std::string, float>>& OrderManager::GetWalletPositions() const
{
	return walletPositions_;
}

const std::unordered_map<std::string, float>* OrderManager::GetCurrentWalletPositions() const
{
	if (walletPositions_.empty())
		return nullptr;
	return &walletPositions_.back();
}
