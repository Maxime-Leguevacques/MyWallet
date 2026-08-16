cmake_minimum_required(VERSION 3.20)

project(MyImGuiApp)

set(CMAKE_CXX_STANDARD 20)


add_subdirectory(
    external/glfw
)


# Make an ImGui library
add_library(imgui
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp

    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp

    external/imgui/implot.cpp
    external/imgui/implot_items.cpp
)

target_include_directories(imgui PUBLIC
    external/imgui
    external/imgui/backends
    external/glfw/include
    include
)


# OpenGL
find_package(OpenGL REQUIRED)


# App
add_executable(MyWallet
    src/main.cpp
    src/app.cpp
    src/wallet.cpp
    src/ui/ui_manager.cpp
    src/ui/window.cpp
    src/ui/windows/overview_window.cpp
    src/ui/windows/entry_window.cpp
    src/ui/windows/asset_list_window.cpp
    src/ui/windows/asset_stats_window.cpp
)


target_link_libraries(MyWallet
    imgui
    glfw
    OpenGL::GL
)
