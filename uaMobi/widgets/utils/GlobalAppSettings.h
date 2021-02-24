#pragma once
#include "submodules/UNAQtCommons/settings/CommonAppSettings.h"
#include <QtCore/qurl.h>

#include <QtCore/QTextStream>
#include <QVector>
#include "dataproviders/ModesDescriptions.h"
/*
		This class is responsible for global settings.
*/

extern const float VERSION;
extern const char* SUFFIX;

#define AppSettings GlobalAppSettings::instanse()
class GlobalAppSettings	: public CommonAppSettings  //	Holds main settings for data exchange
{
private:
	static GlobalAppSettings* _instanse;
	QVector<ModeDescription> modes;
	explicit GlobalAppSettings();		//	Explicit default constructor. All data is obtained automatically via file
protected:
	virtual p_QSettings _getSettingsFile() override;
	virtual void _save(p_QSettings) override;
	virtual void _load(p_QSettings) override;
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

	int scanButtonCode;

	QChar separatorCode;

	QList<int> deserializationOrder;
	QList<int> deserializationPoints;

	QString placeAsItem;
	QString placeAsCode;

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

	ModeDescription& getModeDescription(Modes m);
	ModeDescription& getModeDescription(int m);
	ModeDescription& operator[](Modes m);
	ModeDescription& operator[](int m);
	static GlobalAppSettings* instanse();
	static void init();
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
