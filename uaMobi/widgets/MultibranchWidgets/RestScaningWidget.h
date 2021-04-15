#pragma once
#include "UpdateableScaningWidget.h"

class MultistateClickableCounterLabel;
class RestScaningWidget : public UpdateableScaningWidget
{
	Q_OBJECT
protected:
	QString itemCode;
	MultistateClickableCounterLabel* restLabel;
	QString requestTemplate;
	virtual void _emplaceBarcode(QString barcode, ShortBarcode info) override;
	virtual void operateOverResponse(QStringList parsedResponse) override;
public:
	RestScaningWidget(Modes mode, QWidget* parent);
protected slots:;
	void sendRestRequest();
};