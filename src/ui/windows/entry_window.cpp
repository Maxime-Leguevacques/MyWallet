#include "ui/windows/entry_window.h"

#include <vector>

#include "wallet.h"


EntryWindow::EntryWindow(const std::string& _name)
	: Window(_name) {}

void EntryWindow::Update()
{
	Wallet& wallet = Wallet::GetInstance();

	ImGui::Begin(name_.c_str());
	
	ImGui::Text("monthly investment");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(75);
	ImGui::InputFloat("€", &wallet.entryOverview.monthlyInvestment, 0.0f, 0.0f, "%.2f");
	
	const std::vector<Asset>& assets = wallet.GetAssets();
	std::unordered_map<std::string, float>& assetsPercentage = wallet.entryOverview.assetsPercentage;

	for (const Asset& asset : assets)
	{
		float& percentage = assetsPercentage[asset.isin];
		ImGui::Text("%s", asset.ticker.c_str());
		ImGui::SameLine();
		ImGui::PushID(asset.ticker.c_str());
		ImGui::SetNextItemWidth(75);
		ImGui::InputFloat("%", &percentage, 0.0f, 0.0f, "%.2f");
		ImGui::SameLine();
		float investment = percentage / 100.0f * wallet.entryOverview.monthlyInvestment;
		ImGui::Text(" ->  %.2f€", investment);
		ImGui::PopID();
	}

	ImGui::Separator();
	NewOrderSectionUpdate();

	ImGui::End();
}

void EntryWindow::NewOrderSectionUpdate()
{
	if (!creatingNewOrder_)
	{
		if (ImGui::Button("new order"))
			creatingNewOrder_ = true;
	}
	else
	{
		static const std::string* currentItem = nullptr;

		if (ImGui::Button("cancel"))
		{
			creatingNewOrder_ = false;
			currentItem = nullptr;
			Order order;
			newOrder_ = order;
		}



		std::vector<std::string> items;
		for (const Asset& asset : Wallet::GetInstance().GetAssets())
			items.push_back(asset.ticker);

		if (ImGui::BeginCombo("##combo", currentItem ? currentItem->c_str() : "Select an asset"))
		{
			for (const std::string& item : items)
			{
				bool isSelected = (currentItem == &item);
				if (ImGui::Selectable(item.c_str(), isSelected))
					currentItem = &item;
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::InputFloat("price", &newOrder_.price);
	}
}
