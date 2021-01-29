#pragma once
#include <QtCore/QString>
#include <QtCore/qurl.h>
#include <QtCore/QTranslator>
#include <QtCore/QTextStream>
#include <QVector>
#include "dataproviders/ModesDescriptions.h"
/*
		This class is responsible for global settings.
*/

extern const float VERSION;
extern const char* SUFFIX;

#define AppSettings GlobalAppSettings::instanse()
extern QString rootFilePath;
class GlobalAppSettings	//	Holds main settings for data exchange. DO NOT MAKE MULTIPLE
{
private:
	static GlobalAppSettings* _instanse;
	QVector<ModeDescription> modes;
	QByteArray netEncoding;
	explicit GlobalAppSettings();		//	Explicit default constructor. All data is obtained automatically via file
public:
	QString localfile;		//	file for tolocalmemory
	QUrl httpIn;			//	address for tohttp download
	QString localDatabase;
	bool additionalControlElements;	//	controls panel with camera\ keyboard shortcuts
	bool navigationElements;

	bool autoSearch;				//	enables search on new barcode
	bool simpleSending;
	int sendingDirection;
	int sendingFormat;

	int scanPrefix;
	int scanSuffix;
	int scanButtonCode;
//	QList<QList<int>> serializationOrder;
	QString language;					//	language of the application
	QTranslator qt_translator;			//	global translator is stored here
	int	fontMinHeight;
	int fontMaxHeight;
	double fontPercent;


	QChar separatorCode;

	QList<int> deserializationOrder;
	QList<int> deserializationPoints;

	QString placeAsItem;
	QString placeAsCode;

//	QList<int> sysfeed;

//	QList<bool> floatControl;
	QString extrasearchPrefix;
	bool clearScanBuffer;

	bool sendLogin;
	QString userLogin;
	QString userPass;

	bool autoFillQuantity;
	QString taxInvoiceTemporary;

    QString printerIp;
    int printerPort;
    QString printerPassword;
    QString printerUsername;
    int operatorNumber;
    QString operatorPassword;
    bool printOnlyToFile;
    QString toFilePrintFilepath;
	QString printerServerPath;
	bool useComInsteadOfIp;

    bool sendToEmailByIntent;
    QStringList emailDestinations;
    bool sendAsMessageByIntent;

	// label printing
	QString labelPrinterTemplateText;
	long long int labelPrinterTemplateGUID;
	QString labelPrinterName;
	QString lastPrinterBTMAC;
	QString lastPrinterBTUUID;

    unsigned int notificationsVolume;

	void SetTranslator();				//	Sets translator. Can be used any time
	void Save();						//	Forse save
	ModeDescription& getModeDescription(Modes m);
	ModeDescription& getModeDescription(int m);
	const QByteArray& getNetworkEncoding();
	void setNetworkEncoding(const QString&);
	ModeDescription& operator[](Modes m);
	ModeDescription& operator[](int m);
	~GlobalAppSettings();				//	Dumping destructor. Saves state before exit
	static GlobalAppSettings* instanse();
};
template <class LItem>
QStringList serializeLists(const QList < QList< LItem> > & list)
{
	QStringList temp;
	temp.reserve(list.count());
	for (int i = 0; i < list.count(); ++i)
	{
		QString out;
		QTextStream stream(&out);
		out.clear();
		for (int j = 0; j < list[i].count(); ++j)
		{
			stream << list[i][j] << "|";
		}
		stream.flush();
	
		temp.push_back(out);
	}
	return temp;
}
