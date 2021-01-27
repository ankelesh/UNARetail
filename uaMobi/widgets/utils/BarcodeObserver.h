#pragma once
#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include <QKeySequence>

#define BarcodeObs BarcodeObserver::instanse()

class BarcodeObserver : public QObject
{
	Q_OBJECT
protected:
	QKeySequence prefix;
	QKeySequence suffix;
	QKeySequence scanButton;
	QString buffer;
	bool prefixFound;

	bool active;

	static BarcodeObserver* _instanse;

	bool eventFilter(QObject* object, QEvent* event);
	BarcodeObserver(QChar pref, QChar suff, QChar scanb,  QObject* parent = Q_NULLPTR);
    void _handleBarcodeCatch(QString bc);
public:
	void resetCapture(QChar pref, QChar suff, int scanb);
	void activate();
	void deactivate();
	bool activated();
	static BarcodeObserver* instanse();
signals:
	void barcodeCaught(QString);
	void prefixCaught();
	void suffixCaught();
	void scanButtonPress();
	void backButtonPress();
protected slots:
    void handleIntentBarcode(uint nhash, QString iname, QString barcode);
};





