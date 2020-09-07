#pragma once
#include "widgets/parents/inframedWidget.h"
#include "dataproviders/ModesDescriptions.h"
#include "Datacore/AbsEntity.h"
class QVBoxLayout;
class QTabWidget;
class QuickSendSettings;
class SerializationSettings;
class ExtraSendSettings;
class LoginSettings;
class MegaIconButton;
class SendSettings : public inframedWidget
{
	Q_OBJECT
private:
	QVBoxLayout* mainLayout;
	QTabWidget* innerWidget;
	QuickSendSettings* quickSendSettings;
	SerializationSettings* serializationSettings;
	ExtraSendSettings* extraSettings;
	LoginSettings* loginSettings;
	MegaIconButton* backButton;
	Modes currentMode;


public:
	SendSettings(Modes cmode, Entity prototype, QWidget* parent);
	void showExtraSettings();
private slots:
	void saveAndExit();
};
