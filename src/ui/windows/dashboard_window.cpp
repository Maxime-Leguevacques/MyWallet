#include "ui/windows/dashboard_window.h"


DashboardWindow::DashboardWindow(const std::string& _name)
	: Window(_name) {}

void DashboardWindow::Update()
{
	ImGui::Begin(name_.c_str());

	ImGui::Text("Hi");

	ImGui::End();
}
