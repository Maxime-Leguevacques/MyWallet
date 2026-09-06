#include "ui/windows/overview_window.h"

#include <algorithm>
#include <vector>

#include "implot.h"

#include "wallet.h"
#include "order_manager.h"


void OverviewWindow::UpdatePortfolioAssetChart()
{
	std::vector<std::string> labelsStorage;
	std::vector<const char*> labels;
	std::vector<float> values;
	
	const auto* cp = OrderManager::GetInstance().GetCurrentWalletPositions();    // current positions

	if (!cp || cp->empty())
		return;

	labelsStorage.reserve(cp->size());
	labels.reserve(cp->size());
	values.reserve(cp->size());

	for (const auto& [isin, position] : *cp)
	{
		labelsStorage.push_back(Wallet::IsinToTicker(isin));
		labels.push_back(labelsStorage.back().c_str());
		values.push_back(static_cast<double>(position));
	}

	float width = ImGui::GetContentRegionAvail().x;

	// Because the plot uses ImPlotFlags_Equal, keep it square.
	float size = std::min(ImGui::GetContentRegionAvail().y, 600.0f);

	// Center the square plot horizontally.
	float offset = (width - size) * 0.5f;

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);

	if (ImPlot::BeginPlot("wallet distribution", ImVec2(size, size), ImPlotFlags_Equal))
	{
		ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);

		ImPlot::SetupAxesLimits(0, 1, 0, 1);

		ImPlot::PlotPieChart(
			labels.data(),
			values.data(),
			static_cast<int>(values.size()),
			0.5,
			0.5,
			0.4,
			"%.0f€",
			ImPlotPieChartFlags_Normalize
		);

		ImPlot::EndPlot();
	}
}

void OverviewWindow::UpdatePortfolioGrowthChart()
{
	std::vector<float> positions;
	std::vector<float> indices;

	const auto& wp = OrderManager::GetInstance().GetWalletPositions();

	if (wp.empty()) 
		return;

	for (int i = 0; i < wp.size(); i++)
	{
		float totalPosition = 0.0f;
		for (const auto& [isin, position] : wp[i])
			totalPosition += position;
		positions.push_back(totalPosition);
		indices.push_back(static_cast<float>(i));
	}


	const int count = static_cast<int>(positions.size());


	float width = ImGui::GetContentRegionAvail().x;
	float height = std::min(ImGui::GetContentRegionAvail().y, 400.0f);

	if (ImPlot::BeginPlot(
		"Banane Growth",
		ImVec2(width, height)
	))
	{
		ImPlot::SetupAxes(
			"Time",
			"Wallet value",
			ImPlotAxisFlags_None,
			ImPlotAxisFlags_None
		);

		ImPlot::PlotLine(
			"value",
			indices.data(),
			positions.data(),
			count
		);

		ImPlot::EndPlot();
	}
}

OverviewWindow::OverviewWindow(const std::string& _name)
	: Window(_name) {}

void OverviewWindow::Update()
{
	ImGui::Begin(name_.c_str());

	ImGui::Text("overview window");

	UpdatePortfolioAssetChart();
	UpdatePortfolioGrowthChart();

	ImGui::End();
}
