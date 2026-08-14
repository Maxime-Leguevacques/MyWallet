#pragma once

#include "ui/window.h"


class AssetStatsWindow : public Window
{
public:
	AssetStatsWindow(const std::string& _name);

	void Update() override;
};
