#include "GlobalAppSettings.h"
#include <QtCore/qsettings.h>
#include <QApplication>
#include <QVector>
#include <QDir>
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif
#include "client_specific_defaults.h"

const float VERSION = 0.850f;
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

static ModeDescription defaultMode;

GlobalAppSettings::GlobalAppSettings()
	: CommonAppSettings(
		QStringLiteral(":/translations/unaretail_ru.qm"),
		QStringLiteral(":/translations/unaretail_ro.qm"),
		QStringLiteral(":/translations/unaretail_en.qm")
	),
	modes(), localfile(), httpIn(),
	localDatabase(), additionalControlElements(), navigationElements(),
	autoSearch(), simpleSending(), sendingDirection(), sendingFormat(), 
	scanButtonCode(), separatorCode(), deserializationOrder(), deserializationPoints(),
	placeAsItem(), placeAsCode(), extrasearchPrefix(), clearScanBuffer(), sendLogin(), userLogin(),
	userPass(), autoFillQuantity(), taxInvoiceTemporary(), printerIp(), printerPort(),
	printerPassword(), printerUsername(), operatorNumber(), operatorPassword(),
	printOnlyToFile(), toFilePrintFilepath(), printerServerPath(),
	useComInsteadOfIp(), sendToEmailByIntent(), emailDestinations(), sendAsMessageByIntent(),
	labelPrinterTemplateText(), labelPrinterTemplateGUID()
{
#ifdef DEBUG
	detrace_METHCALL("initializeGlobalAppSettings");
#endif
	
}

p_QSettings GlobalAppSettings::_getSettingsFile()
{
	p_QSettings settings(new QSettings("uaMobi.ini", QSettings::IniFormat));
	initialize_default_file(*settings);
	return settings;
}

void GlobalAppSettings::_save(p_QSettings settings)
{
	if (settings.isNull())
	{
#ifdef DEBUG
		detrace_METHPERROR("GlobalAppSettings::_save", "NO SETTINGS OBJECT PROVIDED TO APP SETTINGS");
#endif
		return;
	}
	CommonAppSettings::_save(settings);
#ifdef DEBUG
	detrace_METHEXPL("if errors: " << settings->status());
#endif

	settings->beginGroup("interface");
	settings->setValue("additionalElements", additionalControlElements);
	settings->setValue("navigation", navigationElements);
	settings->endGroup();

	settings->beginGroup("behaviour");
	settings->setValue("autoSearch", autoSearch);
	settings->setValue("simpleSending", simpleSending);
	settings->setValue("sendingDirection", sendingDirection);
	settings->setValue("sendingFormat", sendingFormat);
	settings->setValue("localDatabase", localDatabase);
	settings->setValue("clearScanBuffer", clearScanBuffer);
	settings->setValue("autoFillQuantity", autoFillQuantity);
	settings->endGroup();

	settings->beginGroup("scaning");
	extrasearchPrefix = settings->value("extrasearchPrefix", QString()).toString();
	settings->setValue("scanButtonCode", scanButtonCode);
	settings->setValue("extrasearchPrefix", extrasearchPrefix);
	settings->endGroup();

	settings->beginGroup("network");
	settings->setValue("localAddress", localfile);
	settings->setValue("httpAddressIn", httpIn);
	settings->setValue("sendLogin", sendLogin);
	settings->setValue("userLogin", userLogin);
	settings->setValue("placeAsItem", placeAsItem);
	settings->setValue("placeAsCode", placeAsCode);
	settings->endGroup();

	settings->beginGroup("printer");
	settings->setValue("printerIp", printerIp);
	settings->setValue("printerPort", printerPort);
	settings->setValue("printerUsername", printerUsername);
	settings->setValue("printerPassword", printerPassword);
	settings->setValue("operatorNumber", operatorNumber);
	settings->setValue("operatorPassword", operatorPassword);
	settings->setValue("printOnlyToFile", printOnlyToFile);
	settings->setValue("toFilePrintFilepath", toFilePrintFilepath);
	settings->setValue("printerServerPath", printerServerPath);
	settings->setValue("useComInsteadOfIp", useComInsteadOfIp);
	settings->endGroup();

	settings->beginGroup("email");
	settings->setValue("sendToEmailByIntent", sendToEmailByIntent);
	settings->setValue("emailDestinations", emailDestinations);
	settings->setValue("sendAsMessageByIntent", sendAsMessageByIntent);
	settings->endGroup();

	settings->beginGroup("label_printer");
	settings->setValue("labelPrinterTemplateText", labelPrinterTemplateText);
	settings->setValue("labelPrinterTemplateGUID", labelPrinterTemplateGUID);
	settings->endGroup();

	settings->beginGroup("modes");
	QStringList serializedModes;
	for (int i = 0; i < modes.count(); ++i)
	{
		serializedModes << modes.at(i).serialize();
	}
	settings->setValue("modesSettings", serializedModes);
	settings->endGroup();

	settings->beginGroup("serialization");
	QStringList a;
	settings->setValue("separatorCode", separatorCode);
	a.clear();
	for (int i = 0; i < deserializationOrder.count(); ++i)
	{
		a << QString::number(deserializationOrder.at(i));
	}
	settings->setValue("deserializationOrder", a);
	a.clear();
	for (int i = 0; i < deserializationPoints.count(); ++i)
	{
		a << QString::number(deserializationPoints.at(i));
	}
	settings->setValue("deserializationPoints", a);
	a.clear();
}


void GlobalAppSettings::_load(p_QSettings settings)
{
	if (settings.isNull())
	{
#ifdef DEBUG
		detrace_METHPERROR("GlobalAppSettings::_load", "NO SETTINGS OBJECT PROVIDED TO COMMON SETTINGS");
#endif
		return;
	}
	CommonAppSettings::_load(settings);
#ifdef DEBUG
	detrace_METHEXPL("if errors: " << settings->status());
#endif
	settings->beginGroup("interface");
	additionalControlElements = settings->value("additionalElements").toBool();
	navigationElements = settings->value("navigation", QVariant(true)).toBool();
	settings->endGroup();
	settings->beginGroup("behaviour");
	autoSearch = settings->value("autoSearch", QVariant(true)).toBool();
	simpleSending = settings->value("simpleSending", false).toBool();
	sendingDirection = settings->value("sendingDirection", 0).toInt();
	sendingFormat = settings->value("sendingFormat", 0).toInt();
	localDatabase = settings->value("localDatabase", QVariant()).toString();
	clearScanBuffer = settings->value("clearScanBuffer", QVariant(false)).toBool();
	autoFillQuantity = settings->value("autoFillQuantity", QVariant(false)).toBool();
	settings->endGroup();
	settings->beginGroup("serialization");
	QList<int> orderForOneMode;
	separatorCode = settings->value("separatorCode", QVariant(QChar(','))).toChar();
	QStringList dlist = settings->value("deserializationOrder", QStringList()).toStringList();
	for (int i = 0; i < dlist.count(); ++i)
	{
		deserializationOrder.push_back(dlist.at(i).toInt());
	}
	dlist.clear();
	dlist = settings->value("deserializationPoints", QStringList()).toStringList();
	for (int i = 0; i < dlist.count(); ++i)
	{
		deserializationPoints.push_back(dlist.at(i).toInt());
	}
	dlist.clear();
	settings->endGroup();
	settings->beginGroup("scaning");
	extrasearchPrefix = settings->value("extrasearchPrefix", QString()).toString();
	scanButtonCode = settings->value("scanButtonCode", QVariant(int('`'))).toInt();
	settings->endGroup();
	settings->beginGroup("network");
#ifdef Q_OS_WIN
	localfile = settings->value("localAddress", "C:/").toString();
#endif // Q_OS_WIN
#ifdef Q_OS_ANDROID
	localfile = settings->value("localAddress", "/storage/emulated/0/").toString();
#endif
	httpIn = settings->value("httpAddressIn").toUrl();
	sendLogin = settings->value("sendLogin", QVariant(false)).toBool();
	userLogin = settings->value("userLogin", QVariant()).toString();
	placeAsItem = settings->value("placeAsItem", QVariant()).toString();
	placeAsCode = settings->value("placeAsCode", QVariant()).toString();
	settings->endGroup();
	settings->beginGroup("modes");
	QStringList modesSerialized = settings->value("modesSettings", QStringList()).toStringList();
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
	settings->endGroup();
	settings->beginGroup("printer");
	printerIp = settings->value("printerIp", QVariant()).toString();
	printerPort = settings->value("printerPort", QVariant(4040)).toInt();
	printerUsername = settings->value("printerUsername", QVariant()).toString();
	printerPassword = settings->value("printerPassword", QVariant()).toString();
	operatorNumber = settings->value("operatorNumber", QVariant(12)).toInt();
	operatorPassword = settings->value("operatorPassword", QVariant()).toString();
	printOnlyToFile = settings->value("printOnlyToFile", QVariant(false)).toBool();
	toFilePrintFilepath = settings->value("toFilePrintFilepath", QVariant(QString(rootFilePath))).toString();
	printerServerPath = settings->value("printerServerPath", QVariant("http://localhost:4444/")).toString();
	useComInsteadOfIp = settings->value("useComInsteadOfIp", QVariant(false)).toBool();
	settings->endGroup();
	settings->beginGroup("email");
	sendToEmailByIntent = settings->value("sendToEmailByIntent", QVariant(false)).toBool();
	emailDestinations = settings->value("emailDestinations", QVariant()).toStringList();
	sendAsMessageByIntent = settings->value("sendAsMessageByIntent", QVariant(false)).toBool();
	settings->endGroup();
	settings->beginGroup("label_printer");
	labelPrinterTemplateText = settings->value("labelPrinterTemplateText", QVariant()).toString();
	labelPrinterTemplateGUID = settings->value("labelPrinterTemplateGUID", QVariant()).toLongLong();
	settings->endGroup();
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

GlobalAppSettings* GlobalAppSettings::_instanse = Q_NULLPTR;
GlobalAppSettings* GlobalAppSettings::instanse()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new GlobalAppSettings();
		_instanse->load();
		_instanse->setAsCommon();
	}
	return _instanse;
}

void GlobalAppSettings::init()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new GlobalAppSettings();
		_instanse->load();
		_instanse->setAsCommon();
	}
}
