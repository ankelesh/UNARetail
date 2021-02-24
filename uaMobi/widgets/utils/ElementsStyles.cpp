#include "ElementsStyles.h"
#include "GlobalAppSettings.h"

/*
	This file contains stylesheet definitions
*/

const QString OK_BUTTONS_STYLESHEET("QPushButton { "
	"background-color: #a2deae;"
	"border: 1px solid gray;"
	"}");

const QString BACK_BUTTONS_STYLESHEET("QPushButton { "
	"background-color: #8ebde6;"
	"border: 1px solid gray;"
	"}");

const QString COMMIT_BUTTONS_STYLESHEET("QPushButton { "
	"background-color: #a0e899;"
	"border: 1px solid gray;"
	"}");

const QString DELETE_BUTTONS_STYLESHEET("QPushButton { "
	"background-color: #d16860;"
	"border: 1px solid gray;"
	"}");

const QString CHANGE_BUTTONS_STYLESHEET("QPushButton {"
	"background-color: #dfe687;"
	"border: 1px solid gray"
	";}");

const QString CHECKED_BUTTONS_STYLESHEET("QPushButton:checked {"
	"background-color: #bdbfa3;"
	"border: 2px solid black;"
	"}");

const QString UP_SPINBOX_STYLESHEET("QPushButton {	"
	"border: 1px solid #e3dac3;"
	"border-top-left-radius: 50px;"
	"border-bottom-left-radius: 50px;"
	"background-color: #61ba68;"
	"}");

const QString DOWN_SPINBOX_STYLESHEET("QPushButton {"
	"border: 1px solid #e3dac3;"
	"border-top-right-radius: 50px;"
	"border-bottom-right-radius: 50px;"
	"background-color: #d6875c;"
	"}");

const QString LARGE_BUTTON_STYLESHEET("QDateEdit{	"
	"border-radius: 5px;"
	"padding: 1px 10px 1px 5px;"
	"border: 1px solid black;"
	"min-height: 30px;"
	"}"
	"QDateEdit::drop-down{"
	"subcontrol-origin: padding;"
	"subcontrol-position: top right;"
	"border-left-width: 1px;"
	"width: 55px;"
	"border-left-style: solid;"
	"border-top-right-radius: 3px;"
	"border-bottom-right-radius: 3px;"
	"border-left-color: black;"
	"image: url(:/res/downarrow.png);"
	"}"
	"QDateEdit::down-arrow{"
	"height:30px;"
	"width:30px;"
	"}");

QString BETTER_CALENDAR_STYLESHEET()
{
	return QString() + "QCalendarWidget QToolButton{"
		+ " height:" + QString::number(calculateAdaptiveHeight(0.05)) + "px;"
		+ " width:" + QString::number(calculateAdaptiveWidth(0.8)) + "px;"
		"color: white;"
		"font-size: " + QString::number(calculateAdaptiveWidth(0.05)) + "px;"
		"icon-size: " + QString::number(calculateAdaptiveWidth(0.116)) + "px, " + 
			QString::number(calculateAdaptiveWidth(0.116)) + "px;"
		"background-color: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, stop : 0 #cccccc, stop: 1 #333333);"
		"}"
		"QCalendarWidget QMenu{"
		+ " width:" + QString::number(calculateAdaptiveWidth(0.9)) + "px;"
		"left: 20px;"
		"color: white;"
		"font-size: " + QString::number(calculateAdaptiveWidth(0.03)) + "px;"
		"background-color: rgb(100, 100, 100);"
		"}"
		"QCalendarWidget QSpinBox{"
		+ " width:" + QString::number(calculateAdaptiveWidth(0.8)) + "px;"
		"font-size:" + QString::number(calculateAdaptiveWidth(0.05)) + "px;"
		"color: white;"
		"background-color: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, stop : 0 #cccccc, stop: 1 #333333);"
		"selection-background-color: rgb(136, 136, 136);"
		"selection-color: rgb(255, 255, 255);"
		"}"
		"QCalendarWidget QSpinBox::up-button{"
		"subcontrol-origin: border;"
		"subcontrol-position: top right;"
		+ " width:" + QString::number(calculateAdaptiveWidth(0.13)) + "px;"
		"}"
		"QCalendarWidget QSpinBox::down-button{"
		"subcontrol-origin: border;"
		"subcontrol-position: bottom right;"
		+ " width:" + QString::number(calculateAdaptiveWidth(0.116)) + "px;"
		"}"
		"QCalendarWidget QSpinBox::up-arrow{"
		+ " width:" + QString::number(calculateAdaptiveWidth(0.116)) + "px;"
		+ " height:" + QString::number(calculateAdaptiveHeight(0.116)) + "px;"
		"}"
		"QCalendarWidget QSpinBox::down-arrow{"
		+ " width:" + QString::number(calculateAdaptiveWidth(0.116)) + "px;"
		+ " height:" + QString::number(calculateAdaptiveHeight(0.116)) + "px;"
		"}"
		"QCalendarWidget QWidget{"
		+ " width:" + QString::number(calculateAdaptiveWidth(0.25))+ "px;"
		+ " height:"+ QString::number(calculateAdaptiveHeight(0.2)) + "px;"
		"alternate-background-color: rgb(128, 128, 128);"
		"}"
		"QCalendarWidget QAbstractItemView : enabled"
		"{"
		"font-size:"+ QString::number(calculateAdaptiveWidth(0.05))+ "px;"
		"color: rgb(180, 180, 180);"
		"background-color: black;"
		"selection-background-color: rgb(64, 64, 64);"
		"selection-color: rgb(0, 255, 0);"
		"}"
		"QCalendarWidget QWidget#qt_calendar_navigationbar"
		"{"
		"background-color: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, stop : 0 #cccccc, stop: 1 #333333);"
		"}"
		"QCalendarWidget QAbstractItemView : disabled"
		"{"
		"color: rgb(64, 64, 64);"
		"}";
}
const QString COUNTERS_LABEL_STYLESHEET = "QLabel"
"{"
"border: 2px solid black;"
"}";
const QString ENABLED_COUNTERS_STYLESHEET = "QLabel"
"{"
"border: 4px solid #b8dedd;"
"}";
const QString DOWNLOAD_BUTTONS_STYLESHEET("QPushButton{ "
	"background-color: #a0e899;"
	"border: 1px solid gray;"
	"} "
	"QPushButton:disabled {"
	"background-color:#4a7039;"
	"}");

const QString ERROR_LINEEDIT_STYLESHEET(
        "QLineEdit { border: 2px solid red; background-color: #f7d0cd;}");


const QString FOCUSED_DATETIMEEDIT_STYLESHEET(
	QStringLiteral(
		"QAbstractSpinBox:focus"
		" {"
		"background-color: #d7effa;"
		"border: 2px solid #7ec0de; } "
		"QDateEdit::drop-down {"
		"subcontrol-origin: padding;"
		"subcontrol-position: center right;"
		"width: %1 px;"
		"}"
	)
);
const QString FOCUSED_SPINBOX_STYLESHEET(
	QStringLiteral(
		"QAbstractSpinBox:focus"
		" {"
		"background-color: #d7effa;"
		"border: 2px solid #7ec0de; }"
	)
);
