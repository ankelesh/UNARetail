#pragma once
#include "submodules/UNAQtCommons/widgets/ControlsWidgets/abs_control.h"
/*
	This file is providing fabricators for controls. It tries to guess control type by legacy names, then constructs and returns control.
*/
class abs_control;
class QLayout;
class QFormLayout;
abs_control* fabricateControl(abs_control::ControlTypes control, QString name, QLayout* layout, QWidget* parent);
