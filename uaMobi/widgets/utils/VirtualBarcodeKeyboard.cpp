#include "VirtualBarcodeKeyboard.h"
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif
#include "widgets/utils/ElementsStyles.h"
#include <QPushButton>
#include <qgridlayout.h>
#include <QLineEdit>


VirtualBarcodeKeyboard::VirtualBarcodeKeyboard(QWidget* parent)
	: inframedWidget(parent),
	mainLayout(new QGridLayout(this)), numberButtons(),
	eraseButton(new QPushButton(this)), clearButton(new QPushButton(this)),
	backButton(new QPushButton(this)), okButton(new QPushButton(this)),
	barcodeLine(new QLineEdit(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(barcodeLine, 0, 0, 2, 0);

	QSizePolicy bsp(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding); // button
	//																			size policy
	int row = 2;
	int col = 0;
	numberButtons.resize(10);
	for (int i = 0; i < 9; ++i)
	{
		numberButtons[i] = new QPushButton(this);
		numberButtons[i]->setSizePolicy(bsp);
		numberButtons[i]->setText(QString::number(i + 1));
		mainLayout->addWidget(numberButtons[i], row, col);
#ifdef QT_VERSION5X
		QObject::connect(numberButtons[i], &QPushButton::pressed, this, &VirtualBarcodeKeyboard::numberPressed);
#else
		QObject::connect(numberButtons[i], SIGNAL(pressed()), this, SLOT(numberPressed()));
#endif
		++col;
		if (col == 3)
		{
			col = 0;
			++row;
		}
	}
	numberButtons[9] = new QPushButton(this);
	numberButtons[9]->setSizePolicy(bsp);
	numberButtons[9]->setText("0");
#ifdef QT_VERSION5X
	QObject::connect(numberButtons[9], &QPushButton::pressed, this, &VirtualBarcodeKeyboard::numberPressed);
#else
	QObject::connect(numberButtons[9], SIGNAL(pressed()), this, SLOT(numberPressed()));
#endif
	mainLayout->addWidget(numberButtons[9], 5, 1);
	mainLayout->addWidget(clearButton, 5, 0);
	mainLayout->addWidget(eraseButton, 5, 2);
	mainLayout->addWidget(okButton, 6, 0, 1, 2);
	mainLayout->addWidget(backButton, 6, 2);

	clearButton->setText("C");
	eraseButton->setText("<<");
	okButton->setIcon(QIcon(":/resources/submit"));
	backButton->setIcon(QIcon(":/resources/back"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	clearButton->setStyleSheet(DELETE_BUTTONS_STYLESHEET);
	eraseButton->setStyleSheet(CHANGE_BUTTONS_STYLESHEET);

	clearButton->setSizePolicy(bsp);
	eraseButton->setSizePolicy(bsp);
	okButton->setMinimumHeight(calculateAdaptiveHeight());
	backButton->setMinimumHeight(calculateAdaptiveHeight());
	setFocus();
	_captureNumbers();
#ifdef QT_VERSION5X
	QObject::connect(clearButton, &QPushButton::pressed, barcodeLine, &QLineEdit::clear);
	QObject::connect(eraseButton, &QPushButton::pressed, barcodeLine, &QLineEdit::backspace);
	QObject::connect(okButton, &QPushButton::pressed, this, &VirtualBarcodeKeyboard::okPressed);
	QObject::connect(backButton, &QPushButton::pressed, this, &VirtualBarcodeKeyboard::backPressed);
#else
	QObject::connect(clearButton, SIGNAL(pressed()), barcodeLine, SLOT(clear()));
	QObject::connect(eraseButton, SIGNAL(pressed()), barcodeLine, SLOT(backspace()));
	QObject::connect(okButton, SIGNAL(pressed()), this, SLOT(okPressed()));
	QObject::connect(backButton, SIGNAL(pressed()), this, SLOT(backPressed()));
#endif
}

void VirtualBarcodeKeyboard::_numberReaction(int contr)
{
	if (contr > 0 && contr < 10)
	{
		barcodeLine->setText(barcodeLine->text() + QString::number(contr));
	}
}

void VirtualBarcodeKeyboard::okPressed()
{
	emit valueConfirmed(barcodeLine->text());
	emit backRequired();
}

void VirtualBarcodeKeyboard::backPressed()
{
	emit backRequired();
}

void VirtualBarcodeKeyboard::setStartingText(QString& str, VirtualBarcodeKeyboard::inputphases phs)
{
	phase = phs;
	barcodeLine->clear();
	barcodeLine->setText(str);
}


void VirtualBarcodeKeyboard::numberPressed()
{
	QObject* buttonsent = sender();
	if (buttonsent != Q_NULLPTR)
	{
		QPushButton* castedPointer = static_cast<QPushButton*>(buttonsent);
		if (castedPointer == Q_NULLPTR)
			return;
		barcodeLine->setText(barcodeLine->text() + castedPointer->text());
	}
}