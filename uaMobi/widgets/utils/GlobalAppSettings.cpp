#include "GlobalAppSettings.h"
#include <QtCore/qsettings.h>
#include <QApplication>
#include <QVector>
#include <QDir>
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include "client_specific_defaults.h"

const float VERSION = 0.838f;
#ifdef LINELLA
const char* SUFFIX = "linella_release";
#else 
#ifdef BEMOL
const char* SUFFIX = "bemol_beta";
#else
#ifdef ETALONUS
const char* SUFFIX = "etalonus_release";
#else
const char* SUFFIX = "nigthly";
#endif
#endif
#endif
QString rootFilePath =
#ifdef Q_OS_ANDROID
    "/storage/emulated/0/";
#else
#ifdef Q_OS_WINCE
    "/Documents/";
#else
    QDir::currentPath();
#endif
#endif

static ModeDescription defaultMode;

GlobalAppSettings::GlobalAppSettings()
	: modes(), netEncoding(), localfile(), httpIn(),
	localDatabase(), additionalControlElements(), navigationElements(),
	autoSearch(), simpleSending(), sendingFormat(), scanPrefix(), scanSuffix(),
	scanButtonCode(), language(),qt_translator(Q_NULLPTR), fontMinHeight(), fontMaxHeight(),
	fontPercent(), separatorCode(), deserializationOrder(), deserializationPoints(),
	placeAsItem(), placeAsCode(), extrasearchPrefix(), clearScanBuffer(), sendLogin(), userLogin(),
	userPass(), autoFillQuantity(), taxInvoiceTemporary()
{
#ifdef DEBUG
	detrace_METHCALL("initializeGlobalAppSettings");
#endif
	QSettings settings("uaMobi.ini", QSettings::IniFormat);
	initialize_default_file(settings);
#ifdef DEBUG
	detrace_METHEXPL("if errors: " << settings.status());
#endif
#ifdef Q_OS_WIN
	localfile = settings.value("localAddress", "C:/").toString();
#endif // Q_OS_WIN
#ifdef Q_OS_ANDROID
	localfile = settings.value("localAddress", "/storage/emulated/0/").toString();
#endif
	httpIn = settings.value("httpAddressIn").toUrl();
	additionalControlElements = settings.value("additionalElements").toBool();
	autoSearch = settings.value("autoSearch", QVariant(true)).toBool();
	language = settings.value("language", "English").toString();
	simpleSending = settings.value("simpleSending", false).toBool();
	sendingDirection = settings.value("sendingDirection", 0).toInt();
	sendingFormat = settings.value("sendingFormat", 0).toInt();
	scanPrefix = settings.value("scanPrefix", QVariant(int('$'))).toInt();
	scanSuffix = settings.value("scanSuffix", QVariant(int('\n'))).toInt();
	navigationElements = settings.value("navigation", QVariant(true)).toBool();
	scanButtonCode = settings.value("scanButtonCode", QVariant(int('`'))).toInt();
	localDatabase = settings.value("localDatabase", QVariant()).toString();
	fontMaxHeight = settings.value("fontMaxHeight", QVariant(30)).toInt();
	fontMinHeight = settings.value("fontMinHeight", QVariant(10)).toInt();
	fontPercent = settings.value("fontPercent", QVariant(0.03)).toDouble();
	QStringList temp = settings.value("serializationOrder", QStringList()).toStringList();
	QList<int> orderForOneMode;
	separatorCode = settings.value("separatorCode", QVariant(QChar(','))).toChar();
	QStringList dlist = settings.value("deserializationOrder", QStringList()).toStringList();
	for (int i = 0; i < dlist.count(); ++i)
	{
		deserializationOrder.push_back(dlist.at(i).toInt());
	}
	dlist.clear();
	dlist = settings.value("deserializationPoints", QStringList()).toStringList();
	for (int i = 0; i < dlist.count(); ++i)
	{
		deserializationPoints.push_back(dlist.at(i).toInt());
	}
	dlist.clear();
	placeAsItem = settings.value("placeAsItem", QVariant()).toString();
	placeAsCode = settings.value("placeAsCode", QVariant()).toString();
	dlist.clear();
	extrasearchPrefix = settings.value("extrasearchPrefix", QString()).toString();
	clearScanBuffer = settings.value("clearScanBuffer", QVariant(false)).toBool();
	sendLogin = settings.value("sendLogin", QVariant(false)).toBool();
	userLogin = settings.value("userLogin", QVariant()).toString();
	QStringList modesSerialized = settings.value("modesSettings", QStringList()).toStringList();
	for (int i = 0; i < MODES_TOTAL; ++i)
	{
		if (i >= modesSerialized.count())
		{
			modes << ModeDescription(modeFromInt(i));
		}
		else
		{
			modes << ModeDescription();
			if (!modes.last().deserialize(modesSerialized.at(i)))
				modes.last() = ModeDescription(modeFromInt(i));
		}
	}
	SetTranslator();
	autoFillQuantity = settings.value("autoFillQuantity", QVariant(false)).toBool();
	netEncoding = settings.value("netEncoding", QVariant("CP1251")).toByteArray();
    settings.beginGroup("printer");
    printerIp = settings.value("printerIp", QVariant()).toString();
    printerPort = settings.value("printerPort", QVariant(4040)).toInt();
    printerUsername = settings.value("printerUsername", QVariant()).toString();
    printerPassword = settings.value("printerPassword", QVariant()).toString();
    operatorNumber = settings.value("operatorNumber", QVariant(12)).toInt();
    operatorPassword = settings.value("operatorPassword", QVariant()).toString();
    printOnlyToFile = settings.value("printOnlyToFile", QVariant(false)).toBool();
    toFilePrintFilepath = settings.value("toFilePrintFilepath", QVariant(QString(rootFilePath))).toString();
	printerServerPath = settings.value("printerServerPath", QVariant("http://localhost:4444/")).toString();
	useComInsteadOfIp = settings.value("useComInsteadOfIp", QVariant(false)).toBool();
    settings.endGroup();
    settings.beginGroup("email");
    sendToEmailByIntent = settings.value("sendToEmailByIntent", QVariant(false)).toBool();
    emailDestinations = settings.value("emailDestinations", QVariant()).toStringList();
    sendAsMessageByIntent = settings.value("sendAsMessageByIntent", QVariant(false)).toBool();
    settings.endGroup();
	settings.beginGroup("label_printer");
	labelPrinterTemplateText = settings.value("labelPrinterTemplateText", QVariant()).toString();
	labelPrinterTemplateGUID = settings.value("labelPrinterTemplateGUID", QVariant()).toLongLong();
	labelPrinterName = settings.value("labelPrinterName", QVariant("zebra")).toString();
	lastPrinterBTMAC = settings.value("lastPrinterBTMAC", QVariant()).toString();
	lastPrinterBTUUID = settings.value("lastPrinterBTUUID", QVariant()).toString();
	settings.endGroup();
    notificationsVolume = settings.value("notificationsVolume", QVariant(100U)).toUInt();

}

void GlobalAppSettings::SetTranslator()
{
	if (language == "Russian")
	{
		qt_translator.load(":/translations/unaretail_ru.qm", ".");
	}
	else if (language == "Romanian")
	{
		qt_translator.load(":/translations/unaretail_ro.qm", ".");
	}
	else
	{
		qt_translator.load(":/translations/unaretail_en.qm", ".");
	}
	qApp->installTranslator(&qt_translator);
}

void GlobalAppSettings::Save()
{
#ifdef DEBUG
	detrace_METHCALL("dumpGlobalAppSettings");
#endif
	QSettings settings("uaMobi.ini", QSettings::IniFormat);
#ifdef DEBUG
	detrace_METHEXPL("if errors: " << settings.status());
#endif
	settings.setValue("localAddress", localfile);
	settings.setValue("httpAddressIn", httpIn);
	settings.setValue("additionalElements", additionalControlElements);
	settings.setValue("language", language);
	settings.setValue("autoSearch", autoSearch);
	settings.setValue("simpleSending", simpleSending);
	settings.setValue("sendingDirection", sendingDirection);
	settings.setValue("sendingFormat", sendingFormat);
	settings.setValue("scanPrefix", scanPrefix);
	settings.setValue("scanSuffix", scanSuffix);
	settings.setValue("scanButtonCode", scanButtonCode);
	settings.setValue("navigation", navigationElements);
	settings.setValue("localDatabase", localDatabase);
	QStringList a;
	settings.setValue("fontMaxHeight", fontMaxHeight);
	settings.setValue("fontMinHeight", fontMinHeight);
	settings.setValue("fontPercent", fontPercent);
	settings.setValue("separatorCode", separatorCode);
	a.clear();
	for (int i = 0; i < deserializationOrder.count(); ++i)
	{
		a << QString::number(deserializationOrder.at(i));
	}
	settings.setValue("deserializationOrder", a);
	a.clear();
	for (int i = 0; i < deserializationPoints.count(); ++i)
	{
		a << QString::number(deserializationPoints.at(i));
	}
	settings.setValue("deserializationPoints", a);
	settings.setValue("placeAsItem", placeAsItem);
	settings.setValue("placeAsCode", placeAsCode);
	a.clear();
	settings.setValue("sysfeed", a);
	a.clear();
	settings.setValue("floatControl", a);
	settings.setValue("extrasearchPrefix", extrasearchPrefix);
	settings.setValue("clearScanBuffer", clearScanBuffer);
	settings.setValue("sendLogin", sendLogin);
	settings.setValue("userLogin", userLogin);
	QStringList serializedModes;
	for (int i = 0; i < modes.count(); ++i)
	{
		serializedModes << modes.at(i).serialize();
	}
	settings.setValue("modesSettings", serializedModes);
	settings.setValue("autoFillQuantity", autoFillQuantity);
	settings.setValue("netEncoding", netEncoding);

    settings.beginGroup("printer");
    settings.setValue("printerIp", printerIp);
    settings.setValue("printerPort", printerPort);
    settings.setValue("printerUsername", printerUsername);
    settings.setValue("printerPassword", printerPassword);
    settings.setValue("operatorNumber", operatorNumber);
    settings.setValue("operatorPassword", operatorPassword);
    settings.setValue("printOnlyToFile", printOnlyToFile);
    settings.setValue("toFilePrintFilepath", toFilePrintFilepath);
	settings.setValue("printerServerPath", printerServerPath);
	settings.setValue("useComInsteadOfIp", useComInsteadOfIp);
    settings.endGroup();
    settings.beginGroup("email");
    settings.setValue("sendToEmailByIntent", sendToEmailByIntent);
    settings.setValue("emailDestinations", emailDestinations);
    settings.setValue("sendAsMessageByIntent", sendAsMessageByIntent);
    settings.endGroup();
	settings.beginGroup("label_printer");
	settings.setValue("labelPrinterTemplateText", labelPrinterTemplateText);
	settings.setValue("labelPrinterTemplateGUID", labelPrinterTemplateGUID);
	settings.setValue("labelPrinterName", labelPrinterName);
	settings.setValue("lastPrinterBTMAC", lastPrinterBTMAC);
	settings.setValue("lastPrinterBTUUID", lastPrinterBTUUID);
	settings.endGroup();
    settings.setValue("notificationsVolume", notificationsVolume);
}

ModeDescription& GlobalAppSettings::getModeDescription(Modes m)
{
	if (int(m) >= 0 && int(m) < modes.count())
	{
		return modes[int(m)];
	}
#ifdef DEBUG
	detrace_FAIL;
	detrace_METHPERROR("GlobalAppSettings::getModeDescription", "Attempt to get inexistent mode on index " << int(m));
#endif
	return defaultMode;
}

ModeDescription& GlobalAppSettings::getModeDescription(int m)
{
	return (*this)[m];
}


const QByteArray& GlobalAppSettings::getNetworkEncoding()
{
	return netEncoding;
}

void GlobalAppSettings::setNetworkEncoding(const QString& enc)
{
	netEncoding = enc.toLocal8Bit();
}

ModeDescription& GlobalAppSettings::operator[](Modes m)
{
	return getModeDescription(m);
}

ModeDescription& GlobalAppSettings::operator[](int m)
{
	if (m >= 0 && m < modes.count())
	{
		return modes[m];
	}
#ifdef DEBUG
	detrace_FAIL;
	detrace_METHPERROR("GlobalAppSettings::getModeDescription", "Attempt to get inexistent mode on index " << m);
#endif
	return defaultMode;
}

GlobalAppSettings::~GlobalAppSettings()
{
	Save();
}

GlobalAppSettings* GlobalAppSettings::_instanse = Q_NULLPTR;
GlobalAppSettings* GlobalAppSettings::instanse()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new GlobalAppSettings();
	}
	return _instanse;
}
