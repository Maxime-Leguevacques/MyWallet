#include "ui/windows/asset_stats_window.h"


AssetStatsWindow::AssetStatsWindow(const std::string& _name)
	: Window(_name) {}

void AssetStatsWindow::Update()
{
	ImGui::Begin(name_.c_str());

	ImGui::Text("Asset stats");

	ImGui::End();
}
