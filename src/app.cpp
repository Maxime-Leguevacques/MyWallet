#include "app.h"

#include <iostream>

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

#include "ui/ui_manager.h"
#include "serializer.h"


App& App::GetInstance()
{
	static App instance;
	return instance;
}

void App::SetColorStyles()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	// Define your purple palette
	constexpr ImVec4 color1 = ImVec4(0.13f, 0.10f, 0.18f, 1.0f); // Very dark purple
	constexpr ImVec4 color2 = ImVec4(0.25f, 0.18f, 0.40f, 1.0f); // Dark purple
	constexpr ImVec4 color3 = ImVec4(0.38f, 0.27f, 0.55f, 1.0f); // Mid purple
	constexpr ImVec4 color4 = ImVec4(0.56f, 0.45f, 0.78f, 1.0f); // Light purple
	constexpr ImVec4 color5 = ImVec4(0.72f, 0.62f, 0.95f, 1.0f); // Lavender (lightest)

	// Muted background to prevent eye strain
	constexpr ImVec4 backgroundColor = ImVec4(0.08f, 0.06f, 0.12f, 1.0f);

	// Backgrounds
	colors[ImGuiCol_WindowBg]            = backgroundColor;
	colors[ImGuiCol_ChildBg]             = backgroundColor;
	colors[ImGuiCol_PopupBg]             = color1;

	// Borders
	colors[ImGuiCol_Border]              = color2;
	colors[ImGuiCol_BorderShadow]        = backgroundColor;

	// Text
	colors[ImGuiCol_Text]                = color5;
	colors[ImGuiCol_TextDisabled]        = color3;

	// Headers
	colors[ImGuiCol_Header]              = color2;
	colors[ImGuiCol_HeaderHovered]       = color3;
	colors[ImGuiCol_HeaderActive]        = color4;

	// Buttons
	colors[ImGuiCol_Button]              = color2;
	colors[ImGuiCol_ButtonHovered]       = color3;
	colors[ImGuiCol_ButtonActive]        = color4;

	// Frame
	colors[ImGuiCol_FrameBg]             = color2;
	colors[ImGuiCol_FrameBgHovered]      = color3;
	colors[ImGuiCol_FrameBgActive]       = color4;

	// Tabs
	colors[ImGuiCol_Tab]                 = color2;
	colors[ImGuiCol_TabHovered]          = color3;
	colors[ImGuiCol_TabActive]           = color4;
	colors[ImGuiCol_TabUnfocused]        = color1;
	colors[ImGuiCol_TabUnfocusedActive]  = color2;

	// Title
	colors[ImGuiCol_TitleBg]             = color1;
	colors[ImGuiCol_TitleBgActive]       = color2;
	colors[ImGuiCol_TitleBgCollapsed]    = color1;

	// Resize Grip
	colors[ImGuiCol_ResizeGrip]          = color3;
	colors[ImGuiCol_ResizeGripHovered]   = color4;
	colors[ImGuiCol_ResizeGripActive]    = color5;

	// Scrollbar
	colors[ImGuiCol_ScrollbarBg]         = backgroundColor;
	colors[ImGuiCol_ScrollbarGrab]       = color2;
	colors[ImGuiCol_ScrollbarGrabHovered]= color3;
	colors[ImGuiCol_ScrollbarGrabActive] = color4;

	// Checkmark, sliders
	colors[ImGuiCol_CheckMark]           = color5;
	colors[ImGuiCol_SliderGrab]          = color4;
	colors[ImGuiCol_SliderGrabActive]    = color5;

	// Plots
	colors[ImGuiCol_PlotLines]           = color4;
	colors[ImGuiCol_PlotLinesHovered]    = color5;
	colors[ImGuiCol_PlotHistogram]       = color4;
	colors[ImGuiCol_PlotHistogramHovered]= color5;

	// Table
	colors[ImGuiCol_TableHeaderBg]       = color2;
	colors[ImGuiCol_TableBorderStrong]   = color3;
	colors[ImGuiCol_TableBorderLight]    = color2;

	// Separator
	colors[ImGuiCol_Separator]           = color3;
	colors[ImGuiCol_SeparatorHovered]    = color4;
	colors[ImGuiCol_SeparatorActive]     = color5;

	// Navigation
	colors[ImGuiCol_NavHighlight]           = color4;
	colors[ImGuiCol_NavWindowingHighlight]  = color5;
	colors[ImGuiCol_NavWindowingDimBg]      = backgroundColor;

	// Optional: tweak rounding and padding
	style.FrameRounding = 5.0f;
	style.WindowRounding = 5.0f;
	style.GrabRounding = 3.0f;
	style.ScrollbarRounding = 5.0f;
}


void App::Init()
{
#pragma region init_glfw
	// Init window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	// Get primary monitor
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	// monitor dimensions
	screenSize_ = { .x = mode->width, .y = mode->height };

	mainWindow_ = glfwCreateWindow(screenSize_.x, screenSize_.y, windowName_.c_str(), nullptr, nullptr);
	if (mainWindow_ == nullptr)
	{
		std::cerr << "Failed to create GLFW window: " << windowName_ << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(mainWindow_); 
#pragma endregion init_glfw


#pragma region init_imgui
	// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;    // Erase previous potentially saved configs
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// Setup style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		SetColorStyles();
		style.WindowRounding = 4;
		style.FrameRounding = 4;
		style.GrabRounding = 3;
		style.ScrollbarSize = 7;
		style.ScrollbarRounding = 0;
	}

	ImGui_ImplGlfw_InitForOpenGL(mainWindow_, true);
	ImGui_ImplOpenGL3_Init("#version 460");
#pragma endregion init_imgui

	UiManager::GetInstance().Init();
}

void App::Run()
{
	const ImGuiIO& io = ImGui::GetIO();
	glViewport(0, 0, screenSize_.x, screenSize_.y);

	Serializer::GetInstance().LoadData();

	while (!glfwWindowShouldClose(mainWindow_))
	{
		glfwPollEvents();

		glClearColor(0.09f, 0.14f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		UiManager::GetInstance().Update();
		if (ImGui::IsKeyPressed(ImGuiKey_H) && !ImGui::GetIO().WantTextInput)
			UiManager::GetInstance().ToggleHelpPopup();
		// serialization
		if (ImGui::IsKeyPressed(ImGuiKey_S) && !ImGui::GetIO().WantTextInput)
			Serializer::GetInstance().SaveData();
		if (ImGui::IsKeyPressed(ImGuiKey_L) && !ImGui::GetIO().WantTextInput)
			Serializer::GetInstance().LoadData();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backupCurrentContext);
		}

		glfwSwapBuffers(mainWindow_);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}
