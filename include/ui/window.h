#pragma once

#include <string>

// Include for all window classes that will probably need ImGui
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"


class Window
{
protected:
	std::string name_;

public:
	Window(const std::string& _name);
	virtual ~Window() = default;

	virtual void Update() = 0;

	std::string GetName();
};
