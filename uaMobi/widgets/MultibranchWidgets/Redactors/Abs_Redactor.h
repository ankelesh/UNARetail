#pragma once
#include <QWidget>
#include "Datacore/AbsEntity.h"

class QFormLayout;
class QLabel;
class Abs_Redactor : public QWidget
{
	Q_OBJECT
protected:
	QFormLayout* mainLayout;
	QLabel* barcodeInfo;
	QLabel* additionalInfo;
	
public:
	Abs_Redactor(QWidget* parent);
	virtual bool setRedacted(Entity e) = 0; 
	virtual void extractAndSave() = 0;
signals:
	void done();
};