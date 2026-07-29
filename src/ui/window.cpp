#include "ui/window.h"


Window::Window(const std::string& _name)
	: name_(_name) {}

std::string Window::GetName()
{
	return name_;
}
