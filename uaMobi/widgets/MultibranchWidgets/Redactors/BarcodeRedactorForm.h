#pragma once
#include "Abs_Redactor.h"
#include "Datacore/BarcodeEntity.h"

class QuantityControl;

class BarcodeRedactorForm : public Abs_Redactor
{
	Q_OBJECT
protected:
	QuantityControl* quantityEdit;

	Barcode redacted;
public:
	BarcodeRedactorForm(QWidget* parent);
	virtual bool setRedacted(Entity e) override;
	virtual void extractAndSave() override;

};