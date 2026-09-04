#pragma once

#include <string>
#include <vector>


struct Order
{
	std::string isin;
	float quantity = 0.0f;
	int day;
	int month;
	int year;
	int hour;
	int minute;
};

struct Asset
{
	std::string name;
	std::string isin;
	std::string ticker;
	std::string broker;
	float position = 0.0f;
	std::vector<float> positions;
	std::vector<Order> orders;
};
