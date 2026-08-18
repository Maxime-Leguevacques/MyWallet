#include "ui/windows/asset_list_window.h"

#include <iostream>

#include "misc/cpp/imgui_stdlib.h"

#include "wallet.h"
#include "ui/ui_manager.h"


AssetListWindow::AssetListWindow(const std::string& _name)
	: Window(_name) 
{
	wallet_ = &Wallet::GetInstance();
}

void AssetListWindow::Update()
{
	ImGui::Begin(name_.c_str());

	NewAssetSectionUpdate();
	ImGui::Separator();
	AssetListUpdate();

	ImGui::End();
}

void AssetListWindow::NewAssetSectionUpdate()
{
	if (!creatingNewAsset_)
	{
		if (ImGui::Button("new asset"))
			creatingNewAsset_ = true;
	}
	else
	{
		ImGui::Text("create a new asset:");
		ImGui::InputText("name", &newAsset_.name);
		ImGui::InputText("ISIN", &newAsset_.isin);
		ImGui::InputText("ticker", &newAsset_.ticker);
		ImGui::InputText("broker", &newAsset_.broker);

		if (ImGui::Button("cancel"))
		{
			creatingNewAsset_ = false;
			Asset asset;
			newAsset_ = asset;
		}

		ImGui::SameLine();
		
		if (ImGui::Button("save"))
		{
			// TODO: Check if new asset is valid
			
			creatingNewAsset_ = false;
			wallet_->AddNewAsset(newAsset_);
			Asset asset;
			newAsset_ = asset;
		}
	}
}

void AssetListWindow::AssetListUpdate()
{
	int assetToDelete = -1;

	const std::vector<Asset>& assets = wallet_->GetAssets();
	for (int i = 0; i < assets.size(); i++)
	{
		ImGui::Text("%s", assets[i].name.c_str());
		ImGui::Text("%s", assets[i].isin.c_str());
		ImGui::Text("%s", assets[i].ticker.c_str());
		ImGui::Text("%s", assets[i].broker.c_str());
		
		ImGui::PushID(i);
		if (ImGui::Button("X"))
			assetToDelete = i;
		ImGui::PopID();

		ImGui::Separator();
	}

	if (assetToDelete != -1)
		wallet_->DeleteAsset(assets[assetToDelete]);
}
