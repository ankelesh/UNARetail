#pragma once
#include <QWidget>
#include <QLineEdit>
#include "dataproviders/ModesDescriptions.h"

class Capturer : public QLineEdit
{
	Q_OBJECT
protected:
	virtual void keyReleaseEvent(QKeyEvent* kev) override;
public:
	Capturer(QWidget* parent);
signals:
	void keyCaptured(int keycode);
};

class QFormLayout;
class QSpinBox;
class MegaIconButton;
class ScaningSettings : public QWidget
{
	Q_OBJECT
protected:
	QFormLayout* mainLayout;
	QSpinBox* prefixCapturer;
	QSpinBox* suffixCapturer;
	Capturer* scanButtonCapturer;
	MegaIconButton* additionInputElements;
	MegaIconButton* navigationElements;
	MegaIconButton* historyButton;
	Modes currentMode;
public:
	ScaningSettings(Modes cmode, QWidget* parent);
	void extractAndSave();
protected slots:
	void scanKeyPressed(int keycode);
};