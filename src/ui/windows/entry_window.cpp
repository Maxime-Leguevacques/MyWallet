#include "ui/windows/entry_window.h"


EntryWindow::EntryWindow(const std::string& _name)
	: Window(_name) {}

void EntryWindow::Update()
{
	ImGui::Begin(name_.c_str());

	ImGui::Text("entry window");

	ImGui::End();
}
