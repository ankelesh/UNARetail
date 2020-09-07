#pragma once
#include <QWidget>
#include <qvalidator.h>

class LoginPassValidator : public QValidator
{
	Q_OBJECT
protected:
	virtual QValidator::State validate(QString& input, int& pos) const override;
public:
	LoginPassValidator(QObject* parent);
};

class QFormLayout;
class MegaIconButton;
class QLineEdit;
class LoginSettings : public QWidget
{
	Q_OBJECT
protected:
	QFormLayout* mainLayout;
	MegaIconButton* enableLoginSend;
	QLineEdit* loginField;
	QLineEdit* passwordField;

	void _setLoginPass();
public:
	LoginSettings(QWidget* parent);
	void extractAndSave();

protected slots:
	void loginEnableToggled();
};