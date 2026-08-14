#pragma once

#include "ui/window.h"


class AssetListWindow : public Window
{
public:
	AssetListWindow(const std::string& _name);

	void Update() override;
};
