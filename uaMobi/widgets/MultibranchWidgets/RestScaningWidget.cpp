#include "RestScaningWidget.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "externalCommunication/AsyncRequestEngine.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/ExtendedLabels.h"


void RestScaningWidget::_emplaceBarcode(QString barcode, ShortBarcode info)
{
    ScaningWidget::_emplaceBarcode(barcode, info);
	if (info != Q_NULLPTR)
	{
		itemCode = QString::number(info->code);
		restLabel->setNotFreshValue(info->count.toDouble());
	}
    sendRestRequest();
}

void RestScaningWidget::operateOverResponse(QStringList parsedResponse)
{
	if (parsedResponse.isEmpty())
		return;
	bool ok;
	double rest = parsedResponse.first().toDouble(&ok);
	if (ok)
	{
		restLabel->setFreshValue(rest);
	}
	else
	{
		restLabel->setFreshValue(NAN);
	}
}

RestScaningWidget::RestScaningWidget(Modes mode, QWidget* parent)
	: UpdateableScaningWidget(mode,parent), itemCode(), 
	restLabel(new MultistateClickableCounterLabel(tr("Rest"), NAN,this)),
	requestTemplate("?c=get_sld&username=%1&barcode=%2&code=%3&place=%4&sysfid=%5")
{
	counterLayout->addWidget(restLabel);
	QObject::connect(restLabel, &MultistateClickableCounterLabel::clicked, this, &RestScaningWidget::sendRestRequest);
}

void RestScaningWidget::sendRestRequest()
{
	datarefreshEngine->clearAndRequest(
		requestTemplate.arg(
			AppSettings->userLogin
		).arg(
			pendingBarcode->barcode
		).arg(
			itemCode
		).arg(
			AppSettings->placeAsCode
		).arg(
			AppSettings->getModeDescription(currentMode).getSysfeed()
		)
	);
}
