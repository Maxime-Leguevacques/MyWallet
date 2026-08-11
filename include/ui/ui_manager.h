#pragma once

#include <unordered_set>

#include "ui/window.h"


class UiManager
{
private:
	std::unordered_set<Window*> windows_;
	int activeTab_ = 0;

private:
	UiManager() = default;
	~UiManager() = default;

public:
	UiManager(const UiManager&) = delete;
	UiManager& operator=(const UiManager&) = delete;
	static UiManager& GetInstance();

	void Init();

	void BeginDockSpace();
	void EndDockSpace();
	void UpdateWindows();

	Window* GetWindowByName(const std::string& _name) const;
};
