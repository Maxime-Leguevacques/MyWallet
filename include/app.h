#pragma once

#include <string>

#include "GLFW/glfw3.h"


struct SVector2Int
{
	int x = 1280;
	int y = 720;
};


class App
{
private:
	SVector2Int screenSize_;
	std::string windowName_ = "MyWallet";
	GLFWwindow* mainWindow_ = nullptr;
	bool isInitialized_ = false;
	bool isImGuiInitialized_ = false;

private:
	App() = default;
	~App() = default;

	void SetColorStyles();

public:
	App(const App&) = delete;
	App& operator=(const App&) = delete;
	static App& GetInstance();

	bool Init();
	void Shutdown();
	void Run();
};
