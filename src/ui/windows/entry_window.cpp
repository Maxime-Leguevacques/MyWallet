#include "ui/windows/entry_window.h"

#include <vector>

#include "wallet.h"


EntryWindow::EntryWindow(const std::string& _name)
	: Window(_name) {}

void EntryWindow::Update()
{
	ImGui::Begin(name_.c_str());
	
	ImGui::Text("monthly investment");
	ImGui::SameLine();
	ImGui::InputFloat("€", &monthlyInvestment);
	//ImGui::Text("IUSQ | 100% | 517,89€");
	//ImGui::Text("IUSN | 100% | 517,89€");
	//ImGui::Text("CEBL | 100% | 517,89€");
	//ImGui::Text("XDWF | 100% | 517,89€");
	//ImGui::Text("XDW0 | 100% | 517,89€");
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
		std::vector<Asset> assets = Wallet::GetInstance().GetAssets();
		for (const Asset& asset : assets)
		{
			ImGui::Text("%s", asset.ticker.c_str());
		}
	}
}
