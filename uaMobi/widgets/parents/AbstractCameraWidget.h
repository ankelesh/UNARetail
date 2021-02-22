#pragma once
#ifdef CAMERA_SUPPORT
#include "submodules/UNAQtCommons/widgets/parents/inframedWidget.h"

/*
		This widget is core one for anything than interfaces with camera - it holds a view
		for QML anything - pass url to it. Also it provides layouts etc - just place there additional
		interface elements.

		Only slot here is used to emit backRequired signal to notify parent widget that user wants to
		close this widget.
			Global settings here are renundant, but they are lightweight and sometimes they will be
			in handy.

			Show is pure virtual! Check inheritors to see why.

*/
class QVBoxLayout;
class QQuickView;
class QHBoxLayout;
class MegaIconButton;
class AbstractCameraWidget : public inframedWidget
	// Abstract parent of any camera widget, which provides layout and qml handling
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	QWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QHBoxLayout* buttonLayout;
	QQuickView* qmlCamera;		//	This widget is only wrapper to qml realization of camera
	QWidget* viewHolder;
	MegaIconButton* backButton;


public:
	AbstractCameraWidget(QString qmlcamPath, QWidget* parent);
	virtual void show() = 0;
private slots:
	void backRequire();
};
#endif
