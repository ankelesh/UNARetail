#include "Abs_Redactor.h"
#include "widgets/utils/ElementsStyles.h"

#include "submodules/UNAQtCommons/widgets/UtilityElements/BigButtonsSpinbox.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h"
#include <QLabel>
#include <QFormLayout>


Abs_Redactor::Abs_Redactor(QWidget* parent)
	:QWidget(parent), mainLayout(new QFormLayout(this)),
	barcodeInfo(new QLabel(this)), additionalInfo(new QLabel(this))
{
	mainLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
	mainLayout->addWidget(barcodeInfo);
	mainLayout->addWidget(additionalInfo);
	barcodeInfo->setAlignment(Qt::AlignCenter);
	this->setFont(GENERAL_FONT);
	barcodeInfo->setFont(AppFonts->makeFont(3.5));
	additionalInfo->setFont(AppFonts->makeFont(1.5));
}
