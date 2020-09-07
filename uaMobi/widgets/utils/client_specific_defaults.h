#pragma once
#ifdef LINELLA
#include "dataproviders/ModesDescriptions.h"
#include <QSettings>
#include <QString>
#include <QVariant>
#include <QUrl>
inline void initialize_default_file(QSettings& settings)
{
	if (!settings.allKeys().isEmpty())
		return;
	// settings default for all linella users
	settings.setValue("httpAddressIn", QUrl::fromUserInput("http://92.115.237.226:60080/um/prices_uniback.php"));
	settings.setValue("language", "Romanian");
	settings.setValue("autoSearch", QVariant(true));
	settings.setValue("additionalElements", true);
	settings.setValue("clearScanBuffer", QVariant(true));

	QList<ModeDescription> modes;
	for (int i = 0; i < MODES_TOTAL; ++i)
	{
		modes << ModeDescription(modeFromInt(i));
	}
	modes[int(Modes::Prices)].setAttachingToPrevDoc(true);
	QStringList serializedModes;
	for (int i = 0; i < modes.count(); ++i)
	{
		serializedModes << modes.at(i).serialize();
	}
	settings.setValue("modesSettings", serializedModes);
}
#else
#ifdef BEMOL
#include "dataproviders/ModesDescriptions.h"
#include <QSettings>
#include <QString>
#include <QVariant>
#include <QUrl>
inline void initialize_default_file(QSettings& settings)
{
	if (!settings.allKeys().isEmpty())
		return;
	// settings default for all linella users
	settings.setValue("httpAddressIn", QUrl::fromUserInput("http://178.168.80.129:1858/bemol.php"));
	settings.setValue("language", "Romanian");
	settings.setValue("autoSearch", QVariant(true));
	settings.setValue("additionalElements", false);
	settings.setValue("clearScanBuffer", QVariant(false));
}
#else
#ifdef ETALONUS
#include "dataproviders/ModesDescriptions.h"
#include <QSettings>
#include <QString>
#include <QVariant>
#include <QUrl>
inline void initialize_default_file(QSettings& settings)
{
	if (!settings.allKeys().isEmpty())
		return;
	// settings default for all linella users
	settings.setValue("httpAddressIn", QUrl::fromUserInput("http://uamobi.una.md/etalonusgrup.php"));
	settings.setValue("language", "Romanian");
	settings.setValue("autoSearch", QVariant(true));
	settings.setValue("additionalElements", true);
	settings.setValue("clearScanBuffer", QVariant(true));

	QList<ModeDescription> modes;
	for (int i = 0; i < MODES_TOTAL; ++i)
	{
		modes << ModeDescription(modeFromInt(i));
	}
	modes[int(Modes::Supplies)].setInsertingTaxNumber(true);
	modes[int(Modes::Simple)].setSysfeed(1208);

	QStringList serializedModes;
	for (int i = 0; i < modes.count(); ++i)
	{
		serializedModes << modes.at(i).serialize();
	}
	settings.setValue("modesSettings", serializedModes);
}
#else
inline void initialize_default_file(QSettings& settings)
{
	return;
}
#endif
#endif
#endif