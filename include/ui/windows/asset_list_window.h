#pragma once

#include "ui/window.h"


class Wallet;


class AssetListWindow : public Window
{
private:
	Wallet* wallet_ = nullptr;

public:
	AssetListWindow(const std::string& _name);

	void Update() override;

private:
	void NewAssetButtonUpdate();
	void AssetListUpdate();
};
