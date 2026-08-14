#include "ui/windows/overview_window.h"

#include <algorithm>

#include "implot.h"


void OverviewWindow::UpdatePortfolioAssetChart()
{
	static const char* labels[] =
	{
		"Dattes",
		"Bananas",
		"Pates",
		"Grapes"
	};

	static double values[] =
	{
		30.0,
		25.0,
		20.0,
		25.0
	};

	float width = ImGui::GetContentRegionAvail().x;

	// Because the plot uses ImPlotFlags_Equal, keep it square.
	float size = std::min(ImGui::GetContentRegionAvail().y, 600.0f);

	// Center the square plot horizontally.
	float offset = (width - size) * 0.5f;

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);

	if (ImPlot::BeginPlot(
		"Fruit Distribution",
		ImVec2(size, size),
		ImPlotFlags_Equal
	))
	{
		ImPlot::SetupAxes(
			nullptr,
			nullptr,
			ImPlotAxisFlags_NoDecorations,
			ImPlotAxisFlags_NoDecorations
		);

		ImPlot::SetupAxesLimits(0, 1, 0, 1);

		ImPlot::PlotPieChart(
			labels,
			values,
			4,
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
	static const double x[] =
	{
		0.0, 1.0, 2.0, 3.0, 4.0,
		5.0, 6.0, 7.0, 8.0, 9.0
	};

	static const double y[] =
	{
		1000.0, 1050.0, 1025.0, 1120.0, 1180.0,
		1150.0, 1275.0, 1350.0, 1320.0, 1450.0
	};

	const int count = sizeof(x) / sizeof(x[0]);

	float width = ImGui::GetContentRegionAvail().x;
	float height = std::min(ImGui::GetContentRegionAvail().y, 400.0f);

	if (ImPlot::BeginPlot(
		"Banane Growth",
		ImVec2(width, height)
	))
	{
		ImPlot::SetupAxes(
			"Time",
			"Banana Value",
			ImPlotAxisFlags_None,
			ImPlotAxisFlags_None
		);

		ImPlot::PlotLine(
			"Banana",
			x,
			y,
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

