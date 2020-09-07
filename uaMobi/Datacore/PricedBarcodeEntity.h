#pragma once
#include "AbsEntity.h"
#include <QtCore/QDateTime>

namespace Roles
{
	namespace PricedBarcode {
		enum Enumerables
		{
			general = 64,
			discount,
			secondary,
			unnecessary
		};
		enum Writeables
		{
			expDate = 128
		};
	}
}

class PricedBarcodeEntity : public AbsEntity
{
protected:
	// Inherited via AbsEntity
	virtual QString _toSql() const override;
	virtual const TemplatedTableHandler* _assocTable() const override;
	virtual QString _formatedView(QString sep, QString dform) const override;
	virtual QString _maximumInfoView(QString sep, QString dform) const override;
	virtual QString _normalizedCsvView() const override;
	virtual bool _isValid() const override;
	virtual bool _fromSql(QSqlQuery*) override;
	virtual AbsEntity* _clone() const override;
	virtual QString _getName() const override;
	virtual double _getEnumerable(int role) const override;
	virtual bool deepCompare(AbsEntity*) const override;
	virtual void _invalidate() override;
	virtual int _getHeight() const override;
	virtual const QStringList& _getFields() const override;
	virtual QString _fullComparationQuery() const override;
	virtual void _setEnumerable(int role, double value) override;
	virtual void fillPrepQuery(QSqlQuery*) const override;
	virtual void _setWriteable(int role, QString text) override;
	virtual QString _getWriteable(int role) const override;
	virtual void _erase() override;
	virtual int _getFieldNumberForRole(int role) const override;
public:
	QString barcode;			//	String representation of barcode. Is used to distinct one barcode from another.
	QDateTime addDate;		//	datetime when barcode was scanned. If it was scanned multiple times - only first stays
	QDateTime expDate;		//	datetime of expiration. Is needed only for sending - no modifications after user input
	QString comment;		//	comment to barcode
	double generalPrice;
	double discountPrice;
	double secondaryPrice;
	double unnecessaryPrice;

	explicit PricedBarcodeEntity(QString barcode = QString(),
		QDateTime adddt = QDateTime::currentDateTime(),
		QDateTime expdt = QDateTime::currentDateTime(),
		QString comm = "", double gP = 0, double dP = 0, double sP = 0, double unP = 0);

	// Inherited via AbsEntity
};

typedef QSharedPointer<PricedBarcodeEntity> PricedBarcode;
typedef QVector<PricedBarcode> PricedBcList;