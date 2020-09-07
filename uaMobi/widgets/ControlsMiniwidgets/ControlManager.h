#pragma once
#include "abs_control.h"
/*
	This file is providing fabricators for controls. It tries to guess control type by legacy names, then constructs and returns control.
*/
class abs_control;
class QLayout;
class QFormLayout;
abs_control* fabricateControl(InputControlEntity::ControlTypes control, QString name, QLayout* layout, QWidget* parent);
