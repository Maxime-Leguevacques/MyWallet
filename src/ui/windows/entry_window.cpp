#include "ui/windows/entry_window.h"

#include <vector>
#include <algorithm>
#include <iostream>

#include "wallet.h"
#include "imgui/implot.h"


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
	NewOrderUpdate();
	ImGui::Separator();
	OrderListUpdate();

	ImGui::End();
}

void EntryWindow::AddNewOrder(const NewOrderData& _nod)
{	
	// add to wallet
	Order newOrder;
	newOrder.id = Wallet::GetInstance().GetOrders().size() + 1;
	newOrder.isin = Wallet::TickerToIsin(_nod.ticker);
	newOrder.price = _nod.price;
	newOrder.day = _nod.day;
	newOrder.month = _nod.month;
	newOrder.year = _nod.year;
	newOrder.hour = _nod.hour;
	newOrder.minute = _nod.minute;

	Wallet::GetInstance().AddOrder(newOrder);
}

void EntryWindow::NewOrderUpdate()
{
	if (!creatingNewOrder_)
	{
		if (ImGui::Button("new order"))
		{
			creatingNewOrder_ = true;
			NewOrderData nod;
			nod_ = nod;
		}
	}
	else
	{
		static std::string currentAsset;

		ImGui::SetNextItemWidth(115);
		if (ImGui::BeginCombo("##combo", currentAsset.empty() ? "Select asset" : currentAsset.c_str()))
		{
			for (const Asset& asset : Wallet::GetInstance().GetAssets())
			{
				bool isSelected = (currentAsset == asset.ticker);
				if (ImGui::Selectable(asset.ticker.c_str(), isSelected))
				{
					currentAsset = asset.ticker;
					nod_.ticker = currentAsset;
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::SetNextItemWidth(75);
		ImGui::InputFloat("price", &nod_.price);
			
		ImGui::Text("Date (DD/MM/YYYY)");

		ImGui::SetNextItemWidth(25);
		ImGui::InputInt("##day", &nod_.day, 0, 0);
		nod_.day = std::clamp(nod_.day, 1, 31);

		ImGui::SameLine();
		ImGui::Text("/");
		ImGui::SameLine();

		ImGui::SetNextItemWidth(25);
		ImGui::InputInt("##month", &nod_.month, 0, 0);
		nod_.month = std::clamp(nod_.month, 1, 12);

		ImGui::SameLine();
		ImGui::Text("/");
		ImGui::SameLine();
		
		ImGui::SetNextItemWidth(40);
		ImGui::InputInt("##year", &nod_.year, 0, 0);
		nod_.year = std::clamp(nod_.year, 1111, 9999);


		ImGui::Text("Time (HH/MM)");

		ImGui::SetNextItemWidth(25);
		ImGui::InputInt("##hour", &nod_.hour, 0, 0);
		nod_.hour = std::clamp(nod_.hour, 0, 23);

		ImGui::SameLine();
		ImGui::Text(":");
		ImGui::SameLine();

		ImGui::SetNextItemWidth(25);
		ImGui::InputInt("##minute", &nod_.minute, 0, 0);
		nod_.minute = std::clamp(nod_.minute, 0, 59);


		if (ImGui::Button("cancel"))
		{
			creatingNewOrder_ = false;
			currentAsset.clear();
			Order order;
		}

		ImGui::SameLine();

		if (ImGui::Button("add"))
		{
			creatingNewOrder_ = false;
			currentAsset.clear();
			AddNewOrder(nod_);
			Order order;
		}
	}
}

void EntryWindow::OrderListUpdate()
{
	constexpr ImGuiTableFlags orderListTableFlags = 
		ImGuiTableFlags_Borders |
		ImGuiTableFlags_RowBg |
		ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_Resizable;

	if (ImGui::BeginTable("Order List", 3, orderListTableFlags))
	{

		ImGui::TableSetupColumn("date & time", ImGuiTableColumnFlags_WidthStretch, 2.0f);
		ImGui::TableSetupColumn("asset", ImGuiTableColumnFlags_WidthStretch, 1.0f);
		ImGui::TableSetupColumn("price", ImGuiTableColumnFlags_WidthStretch, 1.0f);

		const std::vector<Order>& orders = Wallet::GetInstance().GetOrders();
		for (auto it = orders.rbegin(); it != orders.rend(); it++)
		{
			const Order& order = *it;

			ImGui::TableNextRow();
			
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%d/%d/%d %d:%d", order.day, order.month, order.year, order.hour, order.minute);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%s", Wallet::IsinToTicker(order.isin).c_str());

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%.2f", order.price);
		}

		ImGui::EndTable();
	}
}
