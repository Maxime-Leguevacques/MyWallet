#include "ui/windows/overview_window.h"


OverviewWindow::OverviewWindow(const std::string& _name)
	: Window(_name) {}

void OverviewWindow::Update()
{
	ImGui::Begin(name_.c_str());

	ImGui::Text("overview window");

	ImGui::End();
}
