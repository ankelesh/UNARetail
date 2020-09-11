#include "PrinterSettings.h"
#include "widgets/utils/MegaIconButton.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include "widgets/utils/ElementsStyles.h"
#include "widgets/utils/GlobalAppSettings.h"

PrinterSettings::PrinterSettings(QWidget* parent) : QWidget(parent),
    mainLayout(new QFormLayout(this)),
    printerIp(new QLineEdit(this)),
    printerPort(new QSpinBox(this)),
    userName(new QLineEdit(this)),
    printerPassword(new QLineEdit(this)),
    operatorNumber(new QSpinBox(this)),
    operatorPassword(new QLineEdit(this)),
    toFile(new MegaIconButton(this)),
    filePath(new QLineEdit(this))
{
    mainLayout->addRow(tr("Printer IP address or Tremol Server address"), printerIp);
    mainLayout->addRow(tr("Printer port or tremol server port"), printerPort);
    mainLayout->addRow(tr("Receipt user identification"), userName);
    mainLayout->addRow(tr("Printer password"), printerPassword);
    mainLayout->addRow(tr("Operator number"), operatorNumber);
    mainLayout->addRow(tr("Operator password"), operatorPassword);
    mainLayout->addRow(tr("Print to file only"), toFile);
    mainLayout->addRow(tr("file path"), filePath);
    mainLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
    printerPort->setMaximum(55656);
    printerIp->setText(AppSettings->printerIp);
    printerPort->setValue(AppSettings->printerPort);
    userName->setText(AppSettings->printerUsername);
    printerPassword->setText(AppSettings->printerPassword);
    operatorNumber->setValue(AppSettings->operatorNumber);
    operatorPassword->setText(AppSettings->operatorPassword);
    filePath->setText(AppSettings->toFilePrintFilepath);
    operatorNumber->setMaximum(2020);
    printerPassword->setEchoMode(QLineEdit::Password);
    operatorPassword->setEchoMode(QLineEdit::Password);
    toFile->setCheckable(true);
    toFile->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
    toFile->setIcon(QIcon(":/res/viaFile.png"));
    toFile->setChecked(AppSettings->printOnlyToFile);
}

void PrinterSettings::extractAndSave()
{
    AppSettings->printerIp = printerIp->text();
    AppSettings->printerPassword = printerPassword->text();
    AppSettings->printerPort = printerPort->value();
    AppSettings->printerUsername = userName->text();
    AppSettings->operatorNumber = operatorNumber->value();
    AppSettings->operatorPassword = operatorPassword->text();
    AppSettings->printOnlyToFile = toFile->isChecked();
    AppSettings->toFilePrintFilepath = filePath->text();
}
