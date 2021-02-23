#pragma once
#include "submodules/UNAQtCommons/widgets/ControlsWidgets/abs_control.h"


struct control_bind
{
	abs_control::ControlTypes type;
	int role;
	bool is_writeable;
	QString control_name;
	control_bind(abs_control::ControlTypes Type = abs_control::ControlTypes::None,
		int Role = 0, bool is_w = false,
		QString Control_name = QString()) : type(Type), role(Role), is_writeable(is_w), control_name(Control_name) {}
};