#pragma once

#include <string>
#include <vector>


struct Asset
{
	std::string name;
	std::string isin;
	std::string ticker;
	std::string broker;
	std::vector<float> positions;
};
