#include "order_manager.h"

#include <iostream>

#include "wallet.h"


OrderManager& OrderManager::GetInstance()
{
	static OrderManager instance;
	return instance;
}

void OrderManager::AddOrder(const Order& _order)
{
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
	
	for (int i = 0; i < walletPositions_.size(); i++)
	{
		std::cout << "{" << std::endl;
		for (const auto& [isin, position] : walletPositions_[i])
		{
			std::cout << Wallet::GetInstance().IsinToTicker(isin) << " | " << position << std::endl;
		}
		std::cout << "}" << std::endl;
	}
}

void OrderManager::DeleteOrders()
{
	std::vector<Order> orders;
	orders_ = orders;
}

const std::vector<Order>& OrderManager::GetOrders() const
{
	return orders_;
}

const std::vector<std::unordered_map<std::string, float>>& OrderManager::GetWalletPositions() const
{
	return walletPositions_;
}

const std::unordered_map<std::string, float>& GetCurrentWalletPositions()
{
	std::unordered_map<std::string, float> currentPosition = {{"", 0.0f }};
	if (!walletPositions_.empty())
		currentPosition = walletPositions[walletPositions.size() - 1];
	return currentPosition;
}
