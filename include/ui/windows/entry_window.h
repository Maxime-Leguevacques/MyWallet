#pragma once

#include <vector>

#include "ui/window.h"
#include "order.h"


class EntryWindow : public Window
{
private:
	bool creatingNewOrder_ = false;
	Order newOrder_;
	
public:
	EntryWindow(const std::string& _name);

	void Update() override;

private:
	void NewOrderSectionUpdate();
};
