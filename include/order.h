#pragma once

#include <string>


struct Order
{
	int id = 0;
	std::string isin;
	float quantity = 0.0f;
	int day;
	int month;
	int year;
	int hour;
	int minute;
};
