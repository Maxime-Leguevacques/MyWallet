#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "order.h"


class OrderManager
{
private:
	std::vector<Order> orders_;
	std::vector<std::unordered_map<std::string, float>> walletPositions_;

public:
	OrderManager(const OrderManager&) = delete;
	OrderManager& operator=(const OrderManager&) = delete;
	static OrderManager& GetInstance();

	void AddOrder(const Order& _order);
	bool CanAddOrder(const Order& _order) const;
	void ClearOrders();
	const std::vector<Order>& GetOrders() const;

	const std::vector<std::unordered_map<std::string, float>>& GetWalletPositions() const;
	const std::unordered_map<std::string, float>* GetCurrentWalletPositions() const;

private:
	OrderManager() = default;
	~OrderManager() = default;
};
