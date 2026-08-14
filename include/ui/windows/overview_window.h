#pragma once

#include "ui/window.h"


class OverviewWindow : public Window
{
private:
	void UpdatePortfolioAssetChart();
	void UpdatePortfolioGrowthChart();

public:
	OverviewWindow(const std::string& _name);

	void Update() override;
};
