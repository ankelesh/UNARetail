#include "BarcodeObserver.h"
#include <qapplication.h>
#include "widgets/utils/GlobalAppSettings.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif
#ifdef Q_OS_ANDROID
#include "Wrappers/QBroadcastCatcher.h"
#endif
#include "Wrappers/SoundWrappers/SoundEffectPlayer.h"
QKeySequence _initiateSequence(QChar ch)
{
	if (ch.isLetterOrNumber())
		return QKeySequence(ch);
	if (ch == '\n')
	{
        return QKeySequence(Qt::Key_Return);
	}
	else
        return QKeySequence(Qt::Key((ch.unicode())));
}


bool BarcodeObserver::eventFilter(QObject* object, QEvent* event)
{
	if (!active)
	{
		return QObject::eventFilter(object, event);
	}
	if (event->type() == QEvent::KeyPress)
	{
        QKeyEvent* temp = static_cast<QKeyEvent*>(event);
		if (prefixFound)
		{
			if (temp->key() == suffix[0])
			{
				prefixFound = false;
                _handleBarcodeCatch(buffer);
				emit suffixCaught();
				event->accept();
				buffer.clear();
				return true;
			}
			else
			{
				buffer += temp->text();
				event->accept();
				return true;
			}
		}
		else
		{
			if (temp->key() == prefix[0])
			{
				prefixFound = true;
				event->accept();
				emit prefixCaught();
				return true;
			}
			else if (scanButton.matches( temp->key()))
			{
                QString a = scanButton.toString();
				emit scanButtonPress();
			}
			else
			{
				return QObject::eventFilter(object, event);
			}
			
		}

	}
	else if (event->type() == QEvent::KeyRelease)
	{
		if (prefixFound)
		{
			event->accept();
			return true;
		}
		else
		{
			QKeyEvent* temp = static_cast<QKeyEvent*>(event);
			if (temp->key() == Qt::Key_Escape || temp->key() == Qt::Key_Back)
			{
				emit backButtonPress();
				event->accept();
				return true;
			}
		}
	}
	return QObject::eventFilter(object, event);
}

BarcodeObserver::BarcodeObserver(QChar pref, QChar suff, QChar scanb ,QObject* parent)
	: QObject(parent), prefix(_initiateSequence(pref)), 
	suffix(_initiateSequence(suff)), scanButton(_initiateSequence(scanb)),
	buffer(), prefixFound(false), active(false)
{
#ifdef Q_OS_ANDROID
    // datalogic defaults
    QBroadcastCatcher::listenForBroadcast("com.datalogic.decodewedge.decode_action",
                                          "com.datalogic.decode.intentwedge.barcode_string",
                                          "com.datalogic.decodewedge.decode_category" );
    // newland defaults
    QBroadcastCatcher::listenForBroadcast("nlscan.action.SCANNER_RESULT",
                                          "SCAN_BARCODE1",
                                          "" );
   QObject::connect(QBroadcastCatcher::instanse(), &QBroadcastCatcher::intentCaptured, this, &BarcodeObserver::handleIntentBarcode);
#endif
}

void BarcodeObserver::_handleBarcodeCatch(QString bc)
{
    if (active)
    {
        emit barcodeCaught(bc);
    }
}

void BarcodeObserver::resetCapture(QChar pref, QChar suff, int scanb)
{
	prefix = _initiateSequence(pref);
	suffix = _initiateSequence(suff);
	scanButton = QKeySequence(scanb);
	buffer.clear();
	prefixFound = false;
}

void BarcodeObserver::activate()
{
	active = true;
	qApp->installEventFilter(instanse());
}

void BarcodeObserver::deactivate()
{
	active = false;
	qApp->removeEventFilter(instanse());
}
bool BarcodeObserver::activated()
{
	return active;
}
BarcodeObserver* BarcodeObserver::_instanse = Q_NULLPTR;
BarcodeObserver* BarcodeObserver::instanse()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new BarcodeObserver(QChar(AppSettings->scanPrefix), 
			QChar(AppSettings->scanSuffix), QChar(AppSettings->scanButtonCode), Q_NULLPTR);
	}
    return _instanse;
}

void BarcodeObserver::handleIntentBarcode(uint nhash, QString iname, QString barcode)
{
#ifdef DEBUG
    detrace_METHEXPL("obtained barcode by intent " << iname << " : " << barcode);
#endif
    _handleBarcodeCatch(barcode);
}
