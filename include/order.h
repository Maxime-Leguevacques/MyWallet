#pragma once

#include <string>


struct Order
{
	int unixTime;
	std::string isin;
	float price = 0.0f;
};
