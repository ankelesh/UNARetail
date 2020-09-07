#pragma once
#include "AbsEntity.h"

namespace Roles
{
	namespace ShortBarcode
	{
		enum Enumerables
		{
			code = 64,
			price,
			discount
		};
		enum Writeables
		{
			count = 128
		};
	}
}

class ShortBarcodeEntity : public AbsEntity
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
	virtual void _setEnumerable(int role, double value) override;
	virtual bool deepCompare(AbsEntity*) const override;
	virtual void _invalidate() override;
	virtual const QStringList& _getFields() const override;
	virtual QString _fullComparationQuery() const override;
	virtual void fillPrepQuery(QSqlQuery*) const override;
	virtual void _setWriteable(int role, QString text) override;
	virtual QString _getWriteable(int role) const override;
	virtual void _erase() override;
	virtual int _getFieldNumberForRole(int role) const override;
public:
	QString barcode;			//	String representation of barcode. Is used to distinct one barcode from another.
	int code;		
	QString info;
	QString count;
	double price;
	double discount;
	explicit ShortBarcodeEntity(QString barcode = QString(),
		int cod = 0,
		QString info = QString(), QString count = QString(), double Price = 0.0, double discount = 0.0);

	void clear();

	static const TemplatedTableHandler* getTableHandler();
	static const QString makeInsertionFromString(QString);
	static const QStringList & getFieldNames();
	static QSharedPointer<ShortBarcodeEntity> extractFromLine(QString line);



};

typedef QSharedPointer<ShortBarcodeEntity> ShortBarcode;
typedef QVector<ShortBarcode> ShortBcList;