#pragma once
#include <QObject>
#include <QKeyEvent>

class  Stylesheet : public QObject {
	Q_OBJECT

public:
	Stylesheet(QObject * parent = Q_NULLPTR);
	~Stylesheet();

	static void  loadStyle();

private:
	
};
