#include "ui/windows/asset_list_window.h"

#include "wallet.h"


AssetListWindow::AssetListWindow(const std::string& _name)
	: Window(_name) 
{
	wallet_ = &Wallet::GetInstance();
}

void AssetListWindow::Update()
{
	ImGui::Begin(name_.c_str());

	NewAssetButtonUpdate();
	ImGui::Separator();
	AssetListUpdate();

	ImGui::End();
}

void AssetListWindow::NewAssetButtonUpdate()
{
	if (ImGui::Button("new asset"))
	{
		wallet_->AddNewAsset();
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
