#include "tohttp.h"
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif
#include <QApplication>
#include <QtNetwork/QNetworkReply>
#include "dataFormats/formats.h"
#include <QTextDecoder>
#include <QTextCodec>
#include "widgets/utils/GlobalAppSettings.h"
#include "dataproviders/sqldataprovider.h"
#include "externalCommunication/communicationCore.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>


#include <QtConcurrent>
QLinkedList<ShortBarcode> extractProducts(QString input, int /*sessionCounter*/)
{
	QLinkedList<ShortBarcode> loaded;
	QStringList splitted = input.split(QChar(30));
	input.clear();
	for (int i = 0; i < splitted.count(); ++i)
	{
		loaded.push_back(ShortBarcodeEntity::extractFromLine(splitted.at(i)));
	}
	return loaded;
}
QPair<QStringList, QStringList> toHttp::_parsePlaceList()
{
	QPair<QStringList, QStringList> oplist;
	QStringList splitted = input.split(QChar(30));
	input.clear();
	QStringList buffer;
	for (int i = 0; i < splitted.count(); ++i)
	{
		buffer = splitted.at(i).split(',');
		if (buffer.count() != 2)
			continue;
		oplist.first << buffer.last();
		oplist.second << buffer.first();
		QApplication::instance()->processEvents();
	}
	return oplist;
}

QString toHttp::_loadDataToSend(sendingMode sendmode, int format)
{
	return AppData->uploadFullList(currentMode, sendmode, tr("current_lang"), format, int(Destinations::NetworkDestination));
}

void toHttp::_parseProductList()
{
	emit progressLeap(sessionCounter);
	threadResults.push_back(QtConcurrent::run(extractProducts, input, sessionCounter));
	input.clear();
}

bool toHttp::_checkAddress()
{
	return !address.isEmpty();
}

bool toHttp::_setReply(QNetworkReply* reply)
{
	if (awaitedReply != Q_NULLPTR)
	{
		awaitedReply->disconnect();
		awaitedReply->deleteLater();
	}
	awaitedReply = reply;
	return reply != Q_NULLPTR;
}

bool toHttp::_product_list_receiving_start()
{
	++page;
	sessionCounter = 0;
	emit downloadStateChanged(tr("Server response ok, receiving"));
	_send_get_products();
	return true;
}

bool toHttp::_product_list_receiving_block(QString /*input*/)
{
	emit downloadStateChanged(tr("Receiving block ") + QString::number(page));
	_parseProductList();
	++page;
	++sessionCounter;
	return _send_get_products();
}

bool toHttp::_product_list_receiving_end()
{
	page = 0;
	currentlyAwaiting = notAwaiting;
	QLinkedList<QFuture< QLinkedList< ShortBarcode> > >::iterator begin = threadResults.begin();
	int iter = 0;
	while (begin != threadResults.end())
	{
		while (!begin->isFinished())
		{
			qApp->processEvents();
			thread()->sleep(100);
		}
		loaded << begin->result();
		begin->result().clear();
		emit progressLeap((iter * 100) / threadResults.count());
		emit downloadStateChanged(tr("parsing block: ") + QString::number(iter));
		qApp->processEvents();
		++iter;
		++begin;
	}
	emit progressLeap(99);
	emit downloadStateChanged(tr("pushing data..." ) );
	AppData->recreateDownloadTable();
	AppData->pushIntoDownloaded(loaded);
	threadResults.clear();
	emit progressLeap(100);
	emit downloadStateChanged(QString());
	productListPostClean();
	return true;
}

bool toHttp::_send_get_products()
{
	if (!_checkAddress())
	{
		return false;
	}
	QString furl(address);
	furl.reserve(100);
	if (stored_place_code.isEmpty())
	{
		furl += "?c=download";
	}
	else
	{
		furl += "?c=list_products" + stored_place_code;
	}
	furl += "&page=" + QString::number(page);
	furl += "&call_id=" + QString::number(sessionId);
	if (!_setReply(communicationCore::sendGETRequest(furl)))
		return false;
	QObject::connect(awaitedReply, &QNetworkReply::finished, this, &toHttp::downloadResponce);
	return true;
}

toHttp::toHttp(Modes mode, QObject* parent)
	:QObject(parent), address(), currentMode(mode),  awaitedReply(Q_NULLPTR), li(0), lip(0), input(),
	loaded(), threadResults(), currentlyAwaiting(0),
	decoder(new QTextDecoder(QTextCodec::codecForName(AppSettings->getNetworkEncoding()))),
	page(0),
	sessionCounter(0),
	stored_place_code(),
	sessionId(0)
{
}

toHttp::~toHttp()
{
	delete decoder;
}

void toHttp::setAddress(QString newAddress)
{
	address = newAddress;
}



void toHttp::dropAwaiting()
{
	currentlyAwaiting = notAwaiting;
}

bool toHttp::getPlacelist()
{
	if (!_checkAddress())
	{
		return false;
	}
	if (!_setReply(communicationCore::sendGETRequest(address + "?c=list_places")))
		return false;
	QObject::connect(awaitedReply, &QNetworkReply::finished, this, &toHttp::downloadResponce);
	currentlyAwaiting = awaitingPlaces;
	return true;

}

bool toHttp::getProductList(QString place_code)
{
	stored_place_code = place_code;
	page = 0;
	sessionId = ShortBarcodeEntity::makeGUID();
	_send_get_products();
	currentlyAwaiting = awaitingProducts;
	return true;
}

void toHttp::clear()
{
	loaded.clear();
	input.clear();
	if (awaitedReply != Q_NULLPTR)
	{
		awaitedReply->deleteLater();
		awaitedReply = Q_NULLPTR;
	}
	currentlyAwaiting = notAwaiting;
}

void toHttp::productListPostClean()
{
	if (sessionId != 0)
	{
		QString furl(address);
		furl.reserve(100);

		if (stored_place_code.isEmpty())
		{
			furl += "?c=download";
		}
		else
		{
			furl += "?c=list_products" + stored_place_code;
		}
		furl += "&page=1000";
		furl += "&call_id=" + QString::number(sessionId);
		QNetworkReply* reply = communicationCore::sendGETRequest(furl);
		reply->deleteLater();
		sessionId = 0;
	}
}

int toHttp::count()
{
	return loaded.count();
}

void toHttp::downloadResponce()
// This slot is receiving data blocks
{
	if (awaitedReply->error() != 0)
	{
		emit errorReceived();
	}
	else
	{
		if (awaitedReply->size() != 0)
		{

			li = awaitedReply->size();
			lip = 0;
			input.clear();
			input = decoder->toUnicode(awaitedReply->readAll());
#ifdef DEBUG
			detrace_NETRESPARR(input, "", "searchDatabaseLoad");
#endif
			switch (currentlyAwaiting)
			{
			case awaitingPlaces:
			{
				QPair<QStringList, QStringList> data = _parsePlaceList();
				emit placelistReceived(data.first, data.second);
			}
			break;
			case awaitingProducts:
			{
				switch (input.at(0).toLatin1())
				{
				case 0x06:
					_product_list_receiving_start();
					break;
				case 0x017:
					_product_list_receiving_end();
					break;
				default:
					_product_list_receiving_block(input);
				}
			}
			break;
			}
		}
	}
}


bool toHttp::send(sendingMode mode, int fileFormat)
{

	QString dataToSend = _loadDataToSend(mode, fileFormat);
	if (dataToSend.isEmpty())
		return false;
	if (address.isEmpty())
	{
		return false;
	};

	QTextEncoder enc(QTextCodec::codecForName(AppSettings->getNetworkEncoding()));
    QByteArray boundary;
	QByteArray datas(QString("--" + boundary + "\r\n").toLatin1());
	datas += "Content-Disposition: multipart/form-data; name=\"file\"; filename=\"UNARetailExport"
		+ enc.fromUnicode(QDateTime::currentDateTime().toString())  + "\"\r\n";
	datas += "Content-Type: multipart/form-data\r\n\r\n";
	datas += enc.fromUnicode(dataToSend);
	datas += "\r\n";
	datas += QString("--" + boundary + "\r\n\r\n").toLatin1();
	datas += "Content-Disposition: form-data; name=\"upload\"\r\n\r\n";
	datas += "Uploader\r\n";
	datas += QString("--" + boundary + "--\r\n").toLatin1();

	QNetworkRequest req;
	req.setUrl(address + "?c=upload");
	req.setRawHeader("Content-Type", "multipart/form-data; boundary=" + boundary);
#ifdef DEBUG
	detrace_NETREQSENT("send", req.url().toString(), QString::fromUtf8(datas));
#endif
	if (!_setReply(communicationCore::sendPOSTRequest(req, datas)))
		return false;
	QObject::connect(awaitedReply, &QNetworkReply::finished, this, &toHttp::uploadResponce);
	return true;
}

void toHttp::uploadResponce()
//	This slot is responding to upload reply. It clears everything and then emits operationDone
{
	currentlyAwaiting = notAwaiting;
	QString data = decoder->toUnicode(awaitedReply->readAll());
	if (data.isEmpty())
	{
#ifdef DEBUG
		QString s = awaitedReply->errorString();
		detrace_NETERROR(awaitedReply->errorString(), "during upload");
#endif
		emit errorReceived(QString(), tr("empty response"), 404);
	}
	else
	{
		QString stack;
		int code = 0;
		QStringList state = data.split(QChar(30));
		switch (state.count())
		{
		default:
		case 3:
		{
			bool ok;
			long long int pDoc = state.at(2).toLongLong(&ok);
			if (ok)
				AppSettings->getModeDescription(currentMode).setPreviousDocument(pDoc);
		}
		case 2:
			stack = state.at(1);
			if (state.count() > 2)
				stack += state.at(2);
		case 1:
			if (state.first().startsWith("status") && !state.first().isEmpty())
			{
				bool ok;
				code = state.first().right(3).toInt(&ok);
				if (ok)
				{
					switch (code)
					{
					case 200:
						emit sendSuccesfull(stack);
						return;
					case 404:
						emit errorReceived(stack, tr("server error"), code);
						return;
					case 405:
						emit errorReceived(stack, tr("server timeout"), code);
						return;
					case 401:
						emit errorReceived(stack, tr("login failure"), code);
						return;
					default:
						break;
					}
				}
			}
		}
	}
	emit errorReceived(data, tr("error response"), 404);
}
