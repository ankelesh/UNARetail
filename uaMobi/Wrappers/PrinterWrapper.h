#pragma once
#include <QObject>
#include "Datacore/AbsEntity.h"
#include <QVector>



class PrinterWrapper
{
private:
	PrinterWrapper() {};
	static PrinterWrapper* _instance;
public:
	static PrinterWrapper& instance();
	bool printerReady();
	bool preparePrinter();
	bool printReceipt(QSharedPointer<QVector<Entity>> data);
};