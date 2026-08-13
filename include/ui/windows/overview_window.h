#pragma once

#include "ui/window.h"


class OverviewWindow : public Window
{
public:
	OverviewWindow(const std::string& _name);

	void Update() override;
};
