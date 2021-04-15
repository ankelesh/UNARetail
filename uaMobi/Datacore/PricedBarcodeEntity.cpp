#include "PricedBarcodeEntity.h"
#include <QtSql/qsqlquery.h>
#include <QVariant>
#include "widgets/utils/ElementsStyles.h"
#include <cmath>
#include <QStringBuilder>
#include "dataproviders/TableHandlers.h"


namespace PricedBarcodeEntityPrivate
{
	static QString tableDefinition(QStringLiteral("("
		"id number PRIMARY KEY NOT NULL,"
		"barcode TEXT,"
		"scanedtime datetime,"
		"generalPrice number,"
		"discountPrice number,"
		"secondaryPrice number,"
		"unnecessaryPrice number,"
		"uploaded number,"
		"expDateTime datetime,"
		"expComment TEXT"
		")"));
	QStringList _initTPBF()
	{
		QStringList t;
		t <<
			QStringLiteral("id") <<
			QStringLiteral("barcode") <<
			QStringLiteral("scanedtime") <<
			QStringLiteral("generalPrice") <<
			QStringLiteral("discountPrice") <<
			QStringLiteral("secondaryPrice") <<
			QStringLiteral("unnecessaryPrice") <<
			QStringLiteral("uploaded") <<
			QStringLiteral("expDateTime") <<
			QStringLiteral("expComment");
		return t;
	}

	static QStringList tableFields(_initTPBF());


	static TemplatedTableHandler* barcodeTableHandler(
		new TemplatedTableHandler(
			"pricedbcs",
			tableDefinition,
			tableFields,
			0
		)
	);
}
QString normalizePrice(double num)
{
	return QString::number(num, 'g',4);
}

QString PricedBarcodeEntity::_toSql() const
{
	return "( " % serializeId() % ",'" % barcode % "' , '" % addDate.toString(DATETIME_ENCODING_FORMAT) % 
		"' , " % QString::number(generalPrice) % " , "
		% QString::number(discountPrice) % " , " %
		QString::number(secondaryPrice) % " , " %
		QString::number(unnecessaryPrice) % " , 1 , '" 
		% expDate.toString(DATETIME_ENCODING_FORMAT) % 
		"' , '" % QString(comment).replace("'", "''").replace("\"", "\"\"") % "')";
}

const TemplatedTableHandler* PricedBarcodeEntity::_assocTable() const
{
	return PricedBarcodeEntityPrivate::barcodeTableHandler;
}

QString PricedBarcodeEntity::_formatedView(QString sep, QString /*dform*/) const
{
	return comment % sep % barcode % sep % normalizePrice(generalPrice) % sep
		% normalizePrice(discountPrice) % sep % normalizePrice(secondaryPrice)
		% sep % normalizePrice(unnecessaryPrice);
}

QString PricedBarcodeEntity::_maximumInfoView(QString sep, QString dform) const
{
	QString formated;
	formated.reserve(150);
	formated = barcode % "\n" % sep % normalizePrice(generalPrice) % sep
		% normalizePrice(discountPrice) % sep % normalizePrice(secondaryPrice)
		% sep % normalizePrice(unnecessaryPrice) % sep;
	if (dform.isEmpty())
	{
		formated += expDate.toString(TIME_ENCODING_FORMAT);
	}
	else
	{
		formated += expDate.toString(dform);
	}
	formated += "\n" % comment;
	return formated;
}

QString PricedBarcodeEntity::_normalizedCsvView() const
{
	return barcodeUtil::CSV_BARCODE_STR_TEMPLATE.arg(barcode).arg(addDate.toString(DATETIME_ENCODING_FORMAT)).arg(
		generalPrice).arg(expDate.toString(DATETIME_ENCODING_FORMAT)).arg(
			comment).arg(discountPrice).arg(secondaryPrice).arg(
				unnecessaryPrice).arg("").arg("");
}

bool PricedBarcodeEntity::_isValid() const
{
	return !barcode.isEmpty() &&(generalPrice > 0 || discountPrice > 0);
}

bool PricedBarcodeEntity::_fromSql(QSqlQuery* q)
{
	if (!q->next())
		return false;
	GUID = q->value(0).toLongLong();
	barcode = q->value(1).toString();
	addDate = QDateTime::fromString(q->value(2).toString(), DATETIME_ENCODING_FORMAT);
	generalPrice = q->value(3).toDouble();
	discountPrice = q->value(4).toDouble();
	secondaryPrice = q->value(5).toDouble();
	unnecessaryPrice = q->value(6).toDouble();
	expDate = QDateTime::fromString(q->value(8).toString(), DATETIME_ENCODING_FORMAT);
	comment = q->value(9).toString();
	return true;
}

AbsEntity* PricedBarcodeEntity::_clone() const
{
	return new PricedBarcodeEntity(*this);
}

PricedBarcodeEntity::PricedBarcodeEntity(QString Barcode, QDateTime adddt, 
	QDateTime expdt, QString comm, double gP, double dP, double sP, double unP)
	:AbsEntity(int(barcodeUtil::barcodetypes::pricedBc)),  barcode(Barcode),
	addDate(adddt), expDate(expdt), comment(comm), 
	generalPrice(gP), discountPrice(dP), secondaryPrice(sP), unnecessaryPrice(unP)
{
}

void PricedBarcodeEntity::_setWriteable(int role, QString text)
{
	switch (role)
	{
	case 0:
	case Roles::common::barcode:
		barcode = text;
		break;
	case Roles::common::comment:
		comment = text;
		break;
	case Roles::common::date:
		addDate = QDateTime::fromString(text, DATETIME_ENCODING_FORMAT);
		break;
	case Roles::PricedBarcode::Writeables::expDate:
		expDate = QDateTime::fromString(text, DATETIME_ENCODING_FORMAT);
		break;
	default:
		break;
	}
}

QString PricedBarcodeEntity::_getWriteable(int role) const
{
	switch (role)
	{
	case 0:
	case Roles::common::barcode:
		return barcode;
		break;
	case Roles::common::comment:
		return comment;
		break;
	case Roles::common::date:
		return addDate.toString(DATETIME_ENCODING_FORMAT);
		break;
	case Roles::PricedBarcode::Writeables::expDate:
		return expDate.toString(DATETIME_ENCODING_FORMAT);
		break;
	default:
		return QString();
		break;
	}
}
void PricedBarcodeEntity::_erase()
{
	GUID = AbsEntity::makeGUID();
	barcode.clear();
	addDate = QDateTime::currentDateTime();
	expDate = addDate;
	comment.clear();
	generalPrice = 0;
	discountPrice = 0;
	secondaryPrice = 0;
	unnecessaryPrice = 0;
}
/*
"id number PRIMARY KEY NOT NULL,"0
"barcode TEXT,"1
"scanedtime datetime,"2
"generalPrice number,"3
"discountPrice number,"4
"secondaryPrice number,"5
"unnecessaryPrice number,"6
"uploaded number,"7
"expDateTime datetime,"8
"expComment TEXT"9
")"*/
int PricedBarcodeEntity::_getFieldNumberForRole(int role) const
{
	switch (role)
	{
	case Roles::common::barcode:
		return 1;
	case Roles::common::comment:
		return 9;
	case Roles::common::date:
		return 2;
	case Roles::PricedBarcode::discount:
		return 4;
	case Roles::PricedBarcode::expDate:
		return 8;
	case Roles::PricedBarcode::general :
		return 3;
	case Roles::PricedBarcode::secondary:
		return 5;
	case Roles::PricedBarcode::unnecessary:
		return 6;
	default:
		return 0;
	}
}

QString PricedBarcodeEntity::_getName() const
{
	return barcode;
}

double PricedBarcodeEntity::_getEnumerable(int role) const
{
	switch (role)
	{
	case 0:
	case Roles::PricedBarcode::Enumerables::general:
		return generalPrice;
	case Roles::PricedBarcode::Enumerables::discount:
		return discountPrice;
	case Roles::PricedBarcode::Enumerables::secondary:
		return secondaryPrice;
	case Roles::PricedBarcode::Enumerables::unnecessary:
		return unnecessaryPrice;
	default:
		return 0;
	}
}

bool PricedBarcodeEntity::deepCompare(AbsEntity* other) const
{
	return AbsEntity::deepCompare(other) && other->getName() == barcode;
}

void PricedBarcodeEntity::_invalidate()
{
	generalPrice = 0;
	discountPrice = 0;
	secondaryPrice = 0;
	unnecessaryPrice = 0;
}

int PricedBarcodeEntity::_getHeight() const
{
	return (std::ceil(double(barcode.count() + comment.count()
		+ DATETIME_ENCODING_FORMAT.count()
		) / double(AppFonts->howMuchCharacterFitsIntoScreen())) + comment.count("\n") + 2.0);
}

const QStringList& PricedBarcodeEntity::_getFields() const
{
	return PricedBarcodeEntityPrivate::tableFields;
}

QString PricedBarcodeEntity::_fullComparationQuery() const
{
	return " barcode = '" % barcode % "' and generalPrice = " % QString::number(generalPrice)%
		" and discountPrice = " % QString::number(discountPrice);
}

void PricedBarcodeEntity::_setEnumerable(int role, double value)
{
	switch (role)
	{
	case 0:
	case Roles::PricedBarcode::Enumerables::general:
		generalPrice = value;
		break;
	case Roles::PricedBarcode::Enumerables::discount:
		discountPrice = value;
		break;
	case Roles::PricedBarcode::Enumerables::secondary:
		secondaryPrice = value;
		break;
	case Roles::PricedBarcode::Enumerables::unnecessary:
		unnecessaryPrice = value;
		break;
	}

}

void PricedBarcodeEntity::fillPrepQuery(QSqlQuery* q) const
{
	q->bindValue(0, GUID);
	q->bindValue(1, barcode);
	q->bindValue(2, addDate.toString(DATETIME_ENCODING_FORMAT));
	q->bindValue(3, generalPrice);
	q->bindValue(4, discountPrice);
	q->bindValue(5, secondaryPrice);
	q->bindValue(6, unnecessaryPrice);
	q->bindValue(7, 1);
	q->bindValue(8, expDate.toString(DATETIME_ENCODING_FORMAT));
	q->bindValue(9, comment);
}
