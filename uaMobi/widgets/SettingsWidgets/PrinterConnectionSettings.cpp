#include "PrinterConnectionSettings.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include "widgets/utils/ElementsStyles.h"
#include "widgets/utils/GlobalAppSettings.h"

PrinterConnectionSettings::PrinterConnectionSettings(QWidget* parent)
	:QWidget(parent),
	mainLayout(new QFormLayout(this)),
	printerIp(new QLineEdit(this)),
	printerPort(new QSpinBox(this)),
	printerPassword(new QLineEdit(this)),
	printerServerAddress(new QLineEdit(this)),
	comInsteadTcp(new MegaIconButton(this))
{
    mainLayout->addRow(tr("Printer IP address or Tremol Server address"), printerIp);
    mainLayout->addRow(tr("Printer port or tremol server port"), printerPort);
    mainLayout->addRow(tr("Printer password"), printerPassword);
    mainLayout->addRow(tr("printer server address"), printerServerAddress);
    mainLayout->addRow(tr("use port as com port signature"), comInsteadTcp);
    mainLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
    printerPort->setMaximum(55656);
    printerIp->setText(AppSettings->printerIp);
    printerPort->setValue(AppSettings->printerPort);
    printerPassword->setText(AppSettings->printerPassword);
    printerServerAddress->setText(AppSettings->printerServerPath);
    printerPassword->setEchoMode(QLineEdit::Password);
    comInsteadTcp->setCheckable(true);
    comInsteadTcp->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
    comInsteadTcp->setIcon(QIcon(":/resources/viaFile"));
    comInsteadTcp->setChecked(AppSettings->useComInsteadOfIp);

}

void PrinterConnectionSettings::extractAndSave()
{
    AppSettings->printerIp = printerIp->text();
    AppSettings->printerPassword = printerPassword->text();
    AppSettings->printerPort = printerPort->value();
    AppSettings->printerServerPath = printerServerAddress->text();
    AppSettings->useComInsteadOfIp = comInsteadTcp->isChecked();
}
