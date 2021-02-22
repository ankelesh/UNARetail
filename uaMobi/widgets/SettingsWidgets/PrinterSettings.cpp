#include "PrinterSettings.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include "widgets/utils/ElementsStyles.h"
#include "widgets/utils/GlobalAppSettings.h"

PrinterSettings::PrinterSettings(QWidget* parent) : QWidget(parent),
    mainLayout(new QFormLayout(this)),
    userName(new QLineEdit(this)),
    operatorNumber(new QSpinBox(this)),
    operatorPassword(new QLineEdit(this)),
    toFile(new MegaIconButton(this)),
    filePath(new QLineEdit(this))
{
    mainLayout->addRow(tr("Receipt user identification"), userName);
    mainLayout->addRow(tr("Operator number"), operatorNumber);
    mainLayout->addRow(tr("Operator password"), operatorPassword);
    mainLayout->addRow(tr("Print to file only"), toFile);
    mainLayout->addRow(tr("file path"), filePath);
    mainLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
    userName->setText(AppSettings->printerUsername);
    operatorNumber->setValue(AppSettings->operatorNumber);
    operatorPassword->setText(AppSettings->operatorPassword);
    filePath->setText(AppSettings->toFilePrintFilepath);
    operatorNumber->setMaximum(2020);
    operatorPassword->setEchoMode(QLineEdit::Password);
    toFile->setCheckable(true);
    toFile->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
    toFile->setIcon(QIcon(":/res/viaFile.png"));
    toFile->setChecked(AppSettings->printOnlyToFile);
}

void PrinterSettings::extractAndSave()
{
   
    AppSettings->printerUsername = userName->text();
    AppSettings->operatorNumber = operatorNumber->value();
    AppSettings->operatorPassword = operatorPassword->text();
    AppSettings->printOnlyToFile = toFile->isChecked();
    AppSettings->toFilePrintFilepath = filePath->text();
}
