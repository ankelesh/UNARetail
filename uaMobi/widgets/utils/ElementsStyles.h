#pragma once
#include <QApplication>
#include <QString>
#include "submodules/UNAQtCommons/widgets/Scaling/ScalingUtilities.h"
/*
	This file contains constants which are defining buttons stylesheets as a C-strings. all
	new styles must be added ONLY here to allow quick change if necessary.
	Naming: \state\_\element\_STYLESHEET

	Update:
		Now this file also stores additional inline functions for calculating adaptive sizes
		of interface elements. Their defaults are usually correspond most used values.
	Update:
		now theese functions are using actual screen size API instead of deprecated desktop()
*/

extern const QString OK_BUTTONS_STYLESHEET;
// All buttons which are made for confirmation (commit button is separated from simple confirm)

extern const QString BACK_BUTTONS_STYLESHEET;
// Back buttons must be same-styled through all application

extern const QString COMMIT_BUTTONS_STYLESHEET;
// Commit is more saturated

extern const QString DELETE_BUTTONS_STYLESHEET;
// Delete buttons must have this style and provide a warning messagebox

extern const QString CHANGE_BUTTONS_STYLESHEET;
// All buttons which are allowing to change barcode must be same-styled so the user always knew it's functuon by color

extern const QString CHECKED_BUTTONS_STYLESHEET;
// This style is specified to checked state of toggled buttons. use it for settings flags

extern const QString UP_SPINBOX_STYLESHEET;
// up buttons of BigButtonsSpinbox

extern const QString DOWN_SPINBOX_STYLESHEET;
// down buttons of BigButtonsSpinbox

extern const QString LARGE_BUTTON_STYLESHEET;
// Larger buttons

extern QString BETTER_CALENDAR_STYLESHEET();
// Enhanced calendar

extern const QString ERROR_LINEEDIT_STYLESHEET;

extern const QString COUNTERS_LABEL_STYLESHEET;
// bold big letters and borders

extern const QString DOWNLOAD_BUTTONS_STYLESHEET;

extern const QString ENABLED_COUNTERS_STYLESHEET;
extern const QString FOCUSED_DATETIMEEDIT_STYLESHEET;
extern const QString FOCUSED_SPINBOX_STYLESHEET;
