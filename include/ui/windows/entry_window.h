#pragma once

#include <vector>

#include "ui/window.h"


class EntryWindow : public Window
{
private:
	bool creatingNewOrder_ = false;
	
public:
	EntryWindow(const std::string& _name);

	void Update() override;

private:
	void NewOrderSectionUpdate();
};
