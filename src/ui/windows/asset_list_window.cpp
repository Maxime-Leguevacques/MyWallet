#include "ui/windows/asset_list_window.h"

#include "wallet.h"
#include "order_manager.h"


AssetListWindow::AssetListWindow(const std::string& _name)
	: Window(_name), wallet_(Wallet::GetInstance())
{}

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

		if (!validationError_.empty())
			ImGui::TextColored(ImVec4(1.0f, 0.45f, 0.45f, 1.0f), "%s", validationError_.c_str());

		if (ImGui::Button("cancel"))
		{
			creatingNewAsset_ = false;
			validationError_.clear();
			newAsset_ = Asset{};
		}

		ImGui::SameLine();
		
		if (ImGui::Button("add"))
		{
			// TODO: Check if new asset is valid
			if (wallet_.CanAddAsset(newAsset_))
			{			
				creatingNewAsset_ = false;
				validationError_.clear();
				wallet_.AddAsset(newAsset_);
				newAsset_ = Asset{};
			}
			else if (newAsset_.name.empty()
				|| newAsset_.isin.empty()
				|| newAsset_.ticker.empty()
				|| newAsset_.broker.empty())
				validationError_ = "All fields are required";
			else
				validationError_ = "Asset ISIN or ticker already exists";
		}
	}
}

void AssetListWindow::AssetListUpdate()
{
	int assetToDelete = -1;

	const std::vector<Asset>& assets = wallet_.GetAssets();
	for (int i = 0; i < assets.size(); i++)
	{
		ImGui::Text("name      : %s", assets[i].name.c_str());
		ImGui::Text("ISIN      : %s", assets[i].isin.c_str());
		ImGui::Text("ticker    : %s", assets[i].ticker.c_str());
		ImGui::Text("broker    : %s", assets[i].broker.c_str());
		ImGui::Text("######################");
		const std::vector<std::unordered_map<std::string, float>>& walletPositions = OrderManager::GetInstance().GetWalletPositions();
		if (walletPositions.empty())
			ImGui::Text("position  : 0.0f");
		else
		{
			std::unordered_map<std::string, float> currentPositions = walletPositions[walletPositions.size() - 1];
			ImGui::Text("position  : %.2f", currentPositions[assets[i].isin]);
		}

		ImGui::PushID(i);
		if (ImGui::Button("X"))
			assetToDelete = i;
		ImGui::PopID();

		ImGui::Separator();
	}

	if (assetToDelete != -1)
		wallet_.DeleteAsset(assets[assetToDelete]);
}
