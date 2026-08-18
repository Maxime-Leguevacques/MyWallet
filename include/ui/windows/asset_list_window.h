#pragma once

#include "ui/window.h"
#include "asset.h"


class Wallet;


class AssetListWindow : public Window
{
private:
	Wallet* wallet_ = nullptr;
	bool creatingNewAsset_ = false;
	Asset newAsset_;

public:
	AssetListWindow(const std::string& _name);

	void Update() override;

private:
	void NewAssetSectionUpdate();
	void AssetListUpdate();
};
