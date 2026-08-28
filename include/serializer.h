#pragma once

#include <filesystem>

#include "nlohmann/json.hpp"


class Serializer
{
private:
	std::filesystem::path saveFile_;

public:
	Serializer(const Serializer&) = delete;
	Serializer& operator=(const Serializer&) = delete;
	static Serializer& GetInstance();

	static void SaveData();
	static void LoadData();

private:
	Serializer() = default;
	~Serializer() = default;

	static void SaveDashboard(nlohmann::json& _json);
	static void SaveAssets(nlohmann::json& _json);

	static void LoadDashboard(const nlohmann::json& _json);
	static void LoadAssets(const nlohmann::json& _json);
};
