#pragma once
#include <QString>
#include <QList>

/*
	This header contains everything required for work with modes - it's values, 
	hashing possibilities and structure which is encapsulating mode options. Also it 
	defines some usefull enumerations used as destination switcher in various functions.
*/


enum
#ifdef QT_VERSION5X
	class
#endif
	Modes
{
	// This enum represents modes. Warning! All these values must be linked with entities and 
	// ready for QString convertation. Warning! Some settings arrays are hard linked to these numbers.
	Search, Inventory, Supplies, Simple, Prices, Invoices, SalesAccounting
};

// This value binds number of modes to it's actual representation in GlobalAppSettings. It can be reoptimized
// by using length of the list instead of constant. !TODO
extern const int MODES_TOTAL;

typedef unsigned int uint;
inline uint qHash(Modes m)
// used in QHash container
{
	return uint(m);
}
inline Modes modeFromInt(const int m)
// used for carefull conversion, returns search if conversion is impossible.
{
	if (m > 0 && m < MODES_TOTAL)
		return static_cast<Modes>(m);
	return Modes::Search;
}
// used to determine which mode can modify it's sysfeed. Renundant, but still required. Can be done better.
bool hasModifiableSysfeed(Modes m);
// these prefixes required for correct formatting of table names. Renundant, can be refactored.
extern const QString ModePrefixes[];

// this enumeration is used in upload function to determine which data must be sent
enum sendingMode { sendSent, sendUnsent, sendAll };

class ModeDescription
	//  this class represents single mode and it's options. !can be refactored to store all mode info in itself
{
private:
	Modes mode;
	int sysfeed;     // sysfeed is a document template number in system
	QList<int> serializationOrder;	//	order used during saving data
	bool floatControl;				//	used to determine which mode of control must be used
	long long int previousDocument;	//	used to determine number of previously used document
	bool attachNewDataToPrevious;	//	marks whenether data must be attached to previous document or be stored separately
	bool clearBeforeAttachingNewData;	//	marks whenether previous document must be cleared before inserting data
	bool allowInsertingTaxInvoiceNumber;	//	marks whenether tax number field is required
	bool newBCMustHaveTaxInvoiceNumber;		//	marks if it is possible to insert barcode without tax invoice number
	bool historyRequired;
protected:
	// extracts mode from string
	virtual bool _deserialize(const QString&);
	// stores mode to string
	virtual QString _serialize() const;
public:
	explicit ModeDescription();
	explicit ModeDescription(Modes md);
	explicit ModeDescription(QString& serialized);
	ModeDescription(Modes m, int sf, QList<int>& sO, bool fc,long long int pvd, bool andtp, bool cband,
	bool aITIN,
	bool NBMHTIN,
		bool hiR
	);
	bool deserialize(const QString&);
	QString serialize() const;
	Modes getMode() const;
	int getSysfeed() const;
	// getters
	const QList<int>& getSerializationOrder() const;
	bool requiresFloatControl() const;
	bool requiresAttachingToPreviousDoc() const;
	bool mustClearBeforeAttaching() const;
	long long int getPreviousDocNumber() const;
	bool isInsertingTaxInvoiceNumAllowed() const;
	bool isForbiddenInsertingWithoutTaxInvoice() const;
	bool isHistoryRequired() const;

	//setters
	void setSysfeed(int);
	void setSerializationOrder(QList<int>&);
	void setFloatControl(bool);
	void setAttachingToPrevDoc(bool);
	void setCleanBeforeAttaching(bool);
	void setPreviousDocument(long long int);
	void setInsertingTaxNumber(bool);
	void setForbiddingInsertingWithoutTaxInvoice(bool);
	void setHistoryRequire(bool);
};

// enum used to represent table names. Renundant.
enum
#ifdef QT_VERSION5X
	class
#endif
	TableNames
{
	Scanned,		//	Here are stored barcodes which was not uploaded
		Uploaded,		//	Here are stored barcodes marked as sent
		TempScan
};
inline uint qHash(TableNames t)
// used in QHash collection
{
	return uint(t);
}
// stores suffixes attached to table name, which separate tables
extern const QString TableSuffixes[];

// stores destination of uploading
enum
#ifdef QT_VERSION5X
	class
#endif
	Destinations {
	NetworkDestination, FileDestination
};
