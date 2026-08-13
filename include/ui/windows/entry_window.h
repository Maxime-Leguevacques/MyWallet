#pragma once

#include "ui/window.h"


class EntryWindow : public Window
{
public:
	EntryWindow(const std::string& _name);

	void Update() override;
};
