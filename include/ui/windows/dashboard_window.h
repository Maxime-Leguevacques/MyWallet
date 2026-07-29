#pragma once

#include "ui/window.h"


class DashboardWindow : public Window
{
public:
	DashboardWindow(const std::string& _name);

	void Update() override;
};
