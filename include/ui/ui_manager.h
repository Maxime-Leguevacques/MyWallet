#pragma once

#include <unordered_map>
#include <memory>

#include "ui/window.h"


class UiManager
{
private:
	std::unordered_map<std::string, std::unique_ptr<Window>> windows_;
	int activeTab_ = 0;

private:
	UiManager() = default;
	~UiManager() = default;

	void BeginDockSpace();
	void EndDockSpace();
	void UpdateWindows();

	template <class T>
	void CreateWindow(const std::string& _name)
	{
		static_assert(std::is_base_of_v<Window, T>, "T must derive from window");
		windows_.emplace(_name, std::make_unique<T>(_name));
	}

public:
	UiManager(const UiManager&) = delete;
	UiManager& operator=(const UiManager&) = delete;
	static UiManager& GetInstance();

	void Init();
	void Update();
};
