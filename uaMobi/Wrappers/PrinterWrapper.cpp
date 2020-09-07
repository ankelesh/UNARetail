#include "PrinterWrapper.h"

#ifdef DEBUG
#include "debugtrace.h"
#endif

PrinterWrapper* PrinterWrapper::_instance = Q_NULLPTR;

PrinterWrapper& PrinterWrapper::instance()
{
	if (_instance == Q_NULLPTR)
	{
		_instance = new PrinterWrapper();
	}
	return *_instance;
}

bool PrinterWrapper::printerReady()
{
#ifdef DEBUG
	detrace_METHCALL("PrinterWrapper::printerReady");
	return true;
#endif
	return false;
}

bool PrinterWrapper::preparePrinter()
{
#ifdef DEBUG
	detrace_METHCALL("PrinterWrapper::preparePrinter");
	return true;
#endif
	return false;
}

bool PrinterWrapper::printReceipt(QSharedPointer<QVector<Entity>> data)
{
#ifdef DEBUG
	detrace_METHCALL("PrinterWrapper::printReceipt");
#endif
	for (EntityList::iterator ent = data->begin(); ent != data->end(); ++ent)
	{
#ifdef DEBUG
		detrace_METHEXPL("printing entity of type " << (*ent)->myType() << " with data " << (*ent)->formatedView("|", ""));
#endif
	}
	return true;
}
