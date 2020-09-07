#pragma once
#include "RoleBoundScaningWidget.h"



class PrintingReceiptScaningWidget : public RoleBoundScaningWidget
{
	Q_OBJECT
protected:
	MegaIconButton* printAndExitButton;
	virtual void barcodeReady() override;
public:
	PrintingReceiptScaningWidget(Modes mode, int counter_role, QVector<control_bind> bounds, Entity prototype, QWidget* parent);
protected slots:
	virtual void printAndAxitPressed();
	void okPressed() override;
signals:
	void receiptDataFinished(QSharedPointer<QVector<Entity> >);
};
