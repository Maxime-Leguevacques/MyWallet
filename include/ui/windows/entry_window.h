#pragma once

#include "ui/window.h"


class EntryWindow : public Window
{
private:
	bool creatingNewOrder_ = false;
	
	float monthlyInvestment = 0.0f; 

public:
	EntryWindow(const std::string& _name);

	void Update() override;

private:
	void NewOrderSectionUpdate();
};
