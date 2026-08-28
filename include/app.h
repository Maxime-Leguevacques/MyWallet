#pragma once

#include <string>

#include "GLFW/glfw3.h"
#include "ui/ui_manager.h"


struct SVector2Int
{
	int x, y;
};


class App
{
private:
	SVector2Int screenSize_;
	std::string windowName_;
	GLFWwindow* mainWindow_ = nullptr;

	UiManager* uiManager_ = nullptr;

private:
	App() = default;
	~App() = default;

	void SetColorStyles();

public:
	App(const App&) = delete;
	App& operator=(const App&) = delete;
	static App& GetInstance();

	void Init();
	void Run();
};
