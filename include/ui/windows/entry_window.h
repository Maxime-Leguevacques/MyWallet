#pragma once

#include <vector>

#include "ui/window.h"


struct NewOrderData
{
	std::string ticker;
	int day = 1, month = 1, year = 2000, hour = 0, minute = 0;
	float quantity = 0;
	float positionAfterTrade = 0.0f;
};


class EntryWindow : public Window
{
private:
	bool creatingNewOrder_ = false;
	NewOrderData nod_;
	
public:
	EntryWindow(const std::string& _name);

	void Update() override;

private:
	void AddNewOrder(const NewOrderData& _nod);

	void NewOrderUpdate();
	void OrderListUpdate();
};
