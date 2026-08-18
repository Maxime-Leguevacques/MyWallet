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
		{
			creatingNewAsset_ = true;
		}
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
			int a;
	}
}

void AssetListWindow::AssetListUpdate()
{
	const std::vector<Asset>& assets = wallet_->GetAssets();
	for (const Asset asset : assets)
	{
		ImGui::Text("asset");
		ImGui::Separator();
	}
}
