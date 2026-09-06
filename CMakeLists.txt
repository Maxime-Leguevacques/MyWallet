cmake_minimum_required(VERSION 3.20)

project(MyImGuiApp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MYWALLET_BUILD_APP "Build MyWallet GUI app" ON)
option(MYWALLET_BUILD_TESTS "Build MyWallet tests" ON)

add_library(mywallet_core
	src/wallet.cpp
	src/order_manager.cpp
	src/serializer.cpp
)

target_include_directories(mywallet_core PUBLIC
	external
	include
)

add_library(mywallet_warnings INTERFACE)
if (MSVC)
	target_compile_options(mywallet_warnings INTERFACE /W4 /WX)
else()
	target_compile_options(mywallet_warnings INTERFACE -Wall -Wextra -Wpedantic -Wconversion -Werror)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
	target_compile_options(mywallet_warnings INTERFACE
		$<$<CONFIG:Debug>:-fno-omit-frame-pointer -fsanitize=address,undefined>
	)
	target_link_options(mywallet_warnings INTERFACE
		$<$<CONFIG:Debug>:-fsanitize=address,undefined>
	)
endif()

target_link_libraries(mywallet_core PRIVATE mywallet_warnings)

if (MYWALLET_BUILD_APP)
	add_subdirectory(
		external/glfw
	)

	# OpenGL
	find_package(OpenGL REQUIRED)

	# Make an ImGui library
	add_library(imgui
		external/imgui/imgui.cpp
		external/imgui/imgui_draw.cpp
		external/imgui/imgui_tables.cpp
		external/imgui/imgui_widgets.cpp
	
		external/imgui/backends/imgui_impl_glfw.cpp
		external/imgui/backends/imgui_impl_opengl3.cpp
	
		external/imgui/misc/cpp/imgui_stdlib.cpp
	
		external/imgui/implot.cpp
		external/imgui/implot_items.cpp
	)
	
	target_include_directories(imgui PUBLIC
		external
		external/imgui
		external/imgui/backends
		external/glfw/include
		include
	)

	# App
	add_executable(MyWallet
		src/main.cpp
		src/app.cpp
		src/ui/ui_manager.cpp
		src/ui/window.cpp
		src/ui/windows/overview_window.cpp
		src/ui/windows/entry_window.cpp
		src/ui/windows/asset_list_window.cpp
		src/ui/windows/asset_stats_window.cpp
	)

	target_link_libraries(MyWallet
		mywallet_core
		imgui
		glfw
		OpenGL::GL
		mywallet_warnings
	)
endif()

if (MYWALLET_BUILD_TESTS)
	enable_testing()

	add_executable(mywallet_tests
		tests/mywallet_tests.cpp
	)

	target_link_libraries(mywallet_tests
		mywallet_core
		mywallet_warnings
	)

	add_test(NAME mywallet_tests COMMAND mywallet_tests)
endif()
