#include "Stylesheet.h"
#include <QApplication>
#include <QColor>
#include <QStyleFactory>
#include <QPalette>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFontDatabase>
#include <QDesktopWidget>

Stylesheet::Stylesheet(QObject* parent) : QObject(parent)
{

	loadStyle();
}

void Stylesheet::loadStyle()
{

	QFile f(":/qss/styledark.css");
	if (!f.exists())
	{
		return;
	}

	if (f.exists() && f.open(QFile::ReadOnly | QFile::Text))
	{
		QTextStream ts(&f);
		//从qss文件中读取主题色
		while (!ts.atEnd())
		{
			if (ts.readLine().indexOf("QPalette") >= 0)
			{
				QString line = ts.readLine().remove(';');
				QStringList pcList = line.split(":");
				if (1 < pcList.size())
				{
					QString paColor = pcList[1];
					QApplication::setStyle(QStyleFactory::create("fusion"));
					QApplication::setPalette(QColor(paColor.trimmed()));
					break;
				}
			}
		}

		ts.seek(0);

		QString sheet = ts.readAll();
		QRect screenGeometry = QApplication::desktop()->screenGeometry();
		double width = screenGeometry.width();
		double scale = width / 2742.0 + 0.3;
		if (scale < 1)
		{
			scale = 1.0;
		}

		if (scale != 1)
		{
			QRegExp rx("\\d+px", Qt::CaseInsensitive);
			rx.setMinimal(true);
			int index = -1;
			while ((index = rx.indexIn(sheet, index + 1)) >= 0)
			{
				int capLen = rx.cap(0).length() - 2;
				QString snum = sheet.mid(index, capLen);
				snum = QString::number(int(snum.toInt() * scale));
				sheet.replace(index, capLen, snum);
				index += snum.length();
				if (index > sheet.size() - 2)
				{
					break;
				}
			}
		}
		qApp->setStyleSheet(sheet);
	}
	f.close();
	int fontID = QFontDatabase::addApplicationFont(":/fonts/Alibaba-PuHuiTi-Regular.ttf");
	QString PuHuiTi = QFontDatabase::applicationFontFamilies(fontID).at(0);
	QFont font(PuHuiTi, 12);
	QApplication::setFont(font);
}

Stylesheet::~Stylesheet()
{

}
