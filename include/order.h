#pragma once

#include <string>
#include <vector>


struct Order
{
	std::string isin;
	float quantity;
	float positionAfterTrade;
	int day;
	int month;
	int year;
	int hour;
	int minute;
};
