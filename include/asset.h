#pragma once

#include <string>


struct Asset
{
	std::string name;
	std::string isin;
	std::string ticker;
	std::string broker;
	float price;
};
