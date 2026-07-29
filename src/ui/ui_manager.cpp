#include "ui/ui_manager.h"

#include <iostream>

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ui/windows/dashboard_window.h"


UiManager& UiManager::GetInstance()
{
	static UiManager instance;
	return instance;
}

void UiManager::Init()
{
	windows_.insert(new DashboardWindow("MyDashboard"));
}

void UiManager::BeginDockSpace()
{
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

		// Setup dockspace here
		ImGui::DockBuilderDockWindow(GetWindowByName("MyDashboard")->GetName().c_str(), dockspaceId);

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
