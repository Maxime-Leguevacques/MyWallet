#include "ui/ui_manager.h"

#include <iostream>

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ui/windows/overview_window.h"
#include "ui/windows/entry_window.h"
#include "ui/windows/asset_list_window.h"
#include "ui/windows/asset_stats_window.h"


void UiManager::BeginDockSpace()
{
	bool dockSpaceOpen = true;
	constexpr bool optionFullScreenPersistant = true;

	ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None;

	if (optionFullScreenPersistant)
		dockSpaceFlags |= ImGuiDockNodeFlags_PassthruCentralNode;

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	if (optionFullScreenPersistant)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
	}

	ImGui::Begin("DockSpaceWindow", &dockSpaceOpen, windowFlags);

	// Update toolbar here if implemented
	if (ImGui::BeginTabBar("tabs"))
	{
		if (ImGui::BeginTabItem("dashboard"))
		{
			activeTab_ = 0;
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("assets"))
		{
			activeTab_ = 1;
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("insights"))
		{
			activeTab_ = 2;
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	if (optionFullScreenPersistant)
		ImGui::PopStyleVar(3);

	switch (activeTab_)
	{
	case 0:
		ImGui::DockSpace(ImGui::GetID("DashboardDockSpace"), ImVec2(0.0f, 0.0f), dockSpaceFlags);
		break;
	case 1:
		ImGui::DockSpace(ImGui::GetID("AssetDockSpace"), ImVec2(0.0f, 0.0f), dockSpaceFlags);
		break;
	case 2:
		ImGui::DockSpace(ImGui::GetID("InsightsDockSpace"), ImVec2(0.0f, 0.0f), dockSpaceFlags);
		break;
	default:
		ImGui::DockSpace(ImGui::GetID("DashboardDockSpace"), ImVec2(0.0f, 0.0f), dockSpaceFlags);
		break;
	}
	
	static bool isInit = false;
	if (!isInit)
	{	
		isInit = true;
		
		SetupDashboardDockSpace();
		SetupAssetDockSpace();
	}
}

void UiManager::EndDockSpace()
{
	ImGui::End();
}

void UiManager::UpdateWindows()
{	
	switch (activeTab_)
	{
	case 0:
		windows_.at("Overview")->Update();
		windows_.at("Entry")->Update();
		break;
	case 1:
		windows_.at("Asset List")->Update();
		windows_.at("Asset Stats")->Update();
		break;
	case 2:
		break;
	default:
		break;
	}
}

void UiManager::SetupDashboardDockSpace()
{
	const ImGuiID dockspaceId = ImGui::GetID("DashboardDockSpace");
	ImGui::DockBuilderRemoveNode(dockspaceId);
	ImGui::DockBuilderAddNode(dockspaceId);
	ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetMainViewport()->Size);

	ImGuiID id1, id2;
	ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.65f, &id1, &id2);		// left and right
	ImGui::DockBuilderDockWindow(windows_.at("Overview")->GetName().c_str(), id1);
	ImGui::DockBuilderDockWindow(windows_.at("Entry")->GetName().c_str(), id2);
	
	ImGui::DockBuilderFinish(dockspaceId);
}

void UiManager::SetupAssetDockSpace()
{
	const ImGuiID dockspaceId = ImGui::GetID("AssetDockSpace");
	ImGui::DockBuilderRemoveNode(dockspaceId);
	ImGui::DockBuilderAddNode(dockspaceId);
	ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetMainViewport()->Size);

	ImGuiID id1, id2;
	ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.5f, &id1, &id2);		// left and right
	ImGui::DockBuilderDockWindow(windows_.at("Asset List")->GetName().c_str(), id1);
	ImGui::DockBuilderDockWindow(windows_.at("Asset Stats")->GetName().c_str(), id2);

	ImGui::DockBuilderFinish(dockspaceId);
}

void UiManager::SetupInsightsDockSpace()
{

}

void UiManager::UpdateHelpPopup()
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(400, 400));

	ImGui::Begin("Help");

	ImGui::Text("- 'S' : save data");
	ImGui::Text("- 'L' : load data");

	ImGui::End();
}

UiManager& UiManager::GetInstance()
{
	static UiManager instance;
	return instance;
}

void UiManager::Init()
{
	CreateWindow<OverviewWindow>("Overview");
	CreateWindow<EntryWindow>("Entry");
	CreateWindow<AssetListWindow>("Asset List");
	CreateWindow<AssetStatsWindow>("Asset Stats");

	//SetupDashboardDockSpace();
	//SetupAssetDockSpace();
}

void UiManager::Update()
{
	BeginDockSpace();
	UpdateWindows();
	if (isHelpPopupActive_)
		UpdateHelpPopup();
	EndDockSpace();
}

void UiManager::ToggleHelpPopup()
{
	isHelpPopupActive_ = !isHelpPopupActive_;
}
