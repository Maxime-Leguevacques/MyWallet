#include "ui/windows/asset_list_window.h"


AssetListWindow::AssetListWindow(const std::string& _name)
	: Window(_name) {}

void AssetListWindow::Update()
{
	ImGui::Begin(name_.c_str());

	ImGui::Text("asset list window");

	ImGui::End();
}
