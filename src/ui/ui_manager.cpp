#include "ui/ui_manager.h"

#include <iostream>

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ui/windows/overview_window.h"
#include "ui/windows/entry_window.h"


UiManager& UiManager::GetInstance()
{
	static UiManager instance;
	return instance;
}

void UiManager::Init()
{
	windows_.insert(new OverviewWindow("Overview"));
	windows_.insert(new EntryWindow("Entry"));
}

void UiManager::BeginDockSpace()
{
	std::cout << "current tab: " << activeTab_ << std::endl;

#pragma region setup_dockspace
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
#pragma endregion Setup_dockspace

	ImGui::Begin("DockSpace_Window", &dockSpaceOpen, windowFlags);

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

#pragma region Create_dockspace
	const ImGuiID dockspaceId = ImGui::GetID("DockSpace");
	ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockSpaceFlags);

	// Dock control space
	static bool isInit = false;
	if (!isInit)
	{	
		isInit = true;

		ImGui::DockBuilderRemoveNode(dockspaceId);
		ImGui::DockBuilderAddNode(dockspaceId);
		ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetMainViewport()->Size);

		// Create dockspace layout here
		ImGuiID dockIdLeft, dockIdRight;
		ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.7f, &dockIdLeft, &dockIdRight);

		// Setup dockspace here
		ImGui::DockBuilderDockWindow(GetWindowByName("Overview")->GetName().c_str(), dockIdLeft);
		ImGui::DockBuilderDockWindow(GetWindowByName("Entry")->GetName().c_str(), dockIdRight);

		ImGui::DockBuilderFinish(dockspaceId);
	}
#pragma endregion Create_dockspace
}

void UiManager::EndDockSpace()
{
	ImGui::End();
}

void UiManager::UpdateWindows()
{
	for (const auto& window : windows_)
		window->Update();
}

Window* UiManager::GetWindowByName(const std::string& _name) const
{
	for (Window* window : windows_)
	{
		if (window->GetName() == _name)
			return window;
	}

	return nullptr;
};
