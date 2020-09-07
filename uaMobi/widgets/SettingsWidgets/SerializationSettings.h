#pragma once
#include "Datacore/DataEntities.h"
#include "dataproviders/ModesDescriptions.h"
#include <QWidget>
class QVBoxLayout;
class QListWidget;

class SerializationSettings : public QWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	QListWidget* fieldPicker;

	QStringList fieldsModel;
	Entity prototype;
	Modes currentMode;
public:
	SerializationSettings(Modes mode, Entity prototype, QWidget* parent);
	void extractAndSave();
};