#include "BackupingEngine.h"
#include "qexception.h"
#include <QtCore/QFile>
#include <QtCore/QDate>
#ifdef Q_OS_ANDROID
#define ROOT_PATH "/storage/emulated/0/"
#else
#define ROOT_PATH ""
#endif


const QString directoryTemplate("Logs");
const QStringList OperationNames{ "SCAN", "EDIT", "BACK", "REPL", "DELE", "UPLD", "DESC" , "SAVE", "WIPE"};
const QStringList ModeNames{ "INVENTORY", "SUPPLIES", "SIMPLE", "PRICES" };

class nodirException : public QException
{
public:
	void raise() const override { throw *this; }
	nodirException* clone() const override { return new nodirException(*this); };
	QString what() { return "no dir exists!"; };
};

BackupingEngine::BackupingEngine()
{
	mainDir = QDir(ROOT_PATH);
	if (!mainDir.cd("UOlogs"))
	{
		first_launch();
	}
	if (!verify_directory())
	{
		make_directory();
	}
	else
	{
		currentDir.setPath((mainDir.absolutePath() + "/" + make_dirname()));
	}
	next_file();
}

bool BackupingEngine::pushString(QString&str)
{
	using barcodeUtil::CSV_NONBARCODE_OP_TEMPLATE;
	if (!verify_file())
		next_file();
	++stringCounter;
	stream << CSV_NONBARCODE_OP_TEMPLATE.arg("DESC", str, "", "");
	stream.flush();
	return true;
}

bool BackupingEngine::pushOperation(OpType opt, ModType mode, Entity barcode)
{
	using barcodeUtil::CSV_BARCODE_HEADER;
	if (!verify_file())
		next_file();
	++stringCounter;
	stream << CSV_BARCODE_HEADER.arg(OperationNames.at(int(opt)), ModeNames.at(int(mode)),
		barcode->normalizedCsvView());
	stream.flush();
	return true;
}

bool BackupingEngine::pushOperation(OpType o, ModType m, QString sup, QString sec)
{
	using barcodeUtil::CSV_STRING_TEMPLATE;
	if (!verify_file())
		next_file();
	++stringCounter;
	stream << CSV_STRING_TEMPLATE.arg(OperationNames.at(int(o)), ModeNames.at(int(m)), sup, sec);
	stream.flush();
	return true;
}

bool BackupingEngine::wipeOld()
{
	QDir tempdir(mainDir.absolutePath());
	if (tempdir.count() > 0)
	{
		auto sublist = tempdir.entryList(QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot);
		auto start = sublist.begin();
		bool ok;
		int year;
		while (start != sublist.end())
		{
			year = start->toInt(&ok);
			if (ok)
			{
				if (QDate::currentDate().year() - year > 0)
				{
					tempdir.cd(*start);
					tempdir.removeRecursively();
					tempdir.setPath(mainDir.absolutePath());
				}
				else
				{
					tempdir.cd(*start);
					auto sublistlvl2 = tempdir.entryList(QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot);
					auto startlvl2 = sublistlvl2.begin();
					int month;
					while (startlvl2 != sublistlvl2.end())
					{
						month = startlvl2->toInt(&ok);
						if (ok)
						{
							if (QDate::currentDate().month() - month > 2)
							{
								tempdir.cd(*start);
								tempdir.removeRecursively();
								tempdir.setPath(mainDir.absolutePath());
							}
						}
					}
				}
			}
			++start;
		}
	}
	return true;
}

BackupingEngine* BackupingEngine::_instanse = Q_NULLPTR;
BackupingEngine* BackupingEngine::instanse()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new BackupingEngine();
	}
	return _instanse;
}

bool BackupingEngine::verify_file()
{
	if (!currentFile.isOpen() || stringCounter >= 65535)
		return false;
	return true;
}

bool BackupingEngine::verify_directory()
{
	return mainDir.exists(make_dirname());
}

bool BackupingEngine::next_file()
{
	QString fileName = make_filename();
	if (currentDir.count() > 0)
	{
		auto entrys = currentDir.entryList(QDir::Filter::Files);
		int mod = 0;
		while (entrys.contains(fileName))
		{
			++mod;
			fileName = make_filename(mod);
		}	
	}
	if (currentFile.isOpen())
		currentFile.close();
	currentFile.setFileName(currentDir.absoluteFilePath(fileName));
	if (!currentFile.open(QIODevice::OpenModeFlag::WriteOnly)) return false;
	stream.setDevice(&currentFile);
	stream << barcodeUtil::CSV_HEADER_STRING;
	stream.flush();
	stringCounter = 1;
	return true;
}

bool BackupingEngine::erase_dir()
{
	return false;
}

void BackupingEngine::make_directory()
{
	if (mainDir.count() > 0)
	{
		QStringList dirLevels;
		dirLevels << QString::number(QDate::currentDate().year())
		<< QString::number(QDate::currentDate().month())
		<< QString::number(QDate::currentDate().day());
		currentDir = QDir(mainDir.path());
		QStringList::iterator start = dirLevels.begin();
		while (start != dirLevels.end()) {
			if (!currentDir.exists(*start))
			{
				currentDir.mkdir(*start);
			}
			currentDir.cd(*start);
			++start;
		}
	}
	else
	{
		mainDir.mkdir(QString::number(QDate::currentDate().year()));
		currentDir = QDir(mainDir.path());
		currentDir.cd(QString::number(QDate::currentDate().year()));
		currentDir.mkdir(QString::number(QDate::currentDate().month()));
		currentDir.cd(QString::number(QDate::currentDate().month()));
		currentDir.mkdir(QString::number(QDate::currentDate().day()));
		currentDir.cd(QString::number(QDate::currentDate().day()));
	}
}

QString BackupingEngine::make_filename(int iteration )
{
	return "UOL_session_" + QString::number(iteration) + ".csv";
}

QString BackupingEngine::make_dirname()
{
	return QString::number(QDate::currentDate().year()) + '/'
		+ QString::number(QDate::currentDate().month()) + '/'
		+ QString::number(QDate::currentDate().day());
}

void BackupingEngine::first_launch()
{
	mainDir.mkdir("UOlogs");
	mainDir.cd("UOlogs");
	make_directory();
}

ModType databaseToMode(int db)
{
	switch (db)
	{
	case 1:
		return ModType::INV;
	case 2:
		return ModType::SUP;
	case 3:
		return ModType::SIM;
	case 4:
		return ModType::PRI;
	default:
		return ModType::INV;
	}
}
