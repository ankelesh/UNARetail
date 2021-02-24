#include "EmailSettings.h"
#include <QFormLayout>
#include "submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h"
#include <QLineEdit>
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/utils/ElementsStyles.h"
EmailSettings::EmailSettings(QWidget *parent) : QWidget(parent),
    mainLayout(new QFormLayout(this)), enableEmailingButton(new MegaIconButton(this)),
    enableMessageButton(new MegaIconButton(this)),
    targetAdresses(new QLineEdit(this))
{
    mainLayout->addRow(tr("Enable sending receipt to email by intent"), enableEmailingButton);
    mainLayout->addRow(tr("Send as message by intent"), enableMessageButton);
    enableEmailingButton->setIcon(QIcon(":/resources/wifi"));
    enableEmailingButton->setCheckable(true);
    enableEmailingButton->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
    enableEmailingButton->setChecked(AppSettings->sendToEmailByIntent);
    enableMessageButton->setCheckable(true);
    enableMessageButton->setIcon(QIcon(":/res/nextpage.png"));
    enableMessageButton->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
    enableMessageButton->setChecked(AppSettings->sendAsMessageByIntent);
    targetAdresses->setText(AppSettings->emailDestinations.join(';') + ';');
    mainLayout->addRow(tr("Destination email. Use ';' to separate"), targetAdresses);
}

void EmailSettings::extractAndSave()
{
    AppSettings->sendToEmailByIntent = enableEmailingButton->isChecked();
    AppSettings->sendAsMessageByIntent = enableMessageButton->isChecked();
    QStringList temp = targetAdresses->text().split(';');
    AppSettings->emailDestinations.clear();
    for (QStringList::iterator line = temp.begin(); line < temp.end(); ++line)
    {
        if (line->contains('@'))
            AppSettings->emailDestinations << *line;
    }
}
