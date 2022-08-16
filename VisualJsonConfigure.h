#pragma once
#include <QObject>
#include <QMutex>

#define JsonManager VisualJsonConfigure::Instance()

class VisualJsonConfigurePrivate;
class  VisualJsonConfigure : public QObject
{
	Q_OBJECT
public:
	static VisualJsonConfigure* Instance();
	static void DeleteInstance();

	void SetJsonConfigureFile(const QString& file);
	QString GetJsonConfigureFile() const;

	void SetStringValue(const QString& group, const QString& key, const QString& value);
	QString GetValueAsString(const QString& group, const QString& key);

	QStringList GetValueAsStringList(const QString& key);

	QStringList GetAllGroups() const;
	QStringList GetGroupKeys(const QString& group) const;
	QString GetGroupNode(const QString& group) const;

	void Save();
	void Read();

protected:
	QScopedPointer<VisualJsonConfigurePrivate> d_ptr;
	VisualJsonConfigure(QObject* parent = 0);
	~VisualJsonConfigure();

private:
	Q_DECLARE_PRIVATE(VisualJsonConfigure);
	Q_DISABLE_COPY(VisualJsonConfigure);

	static QAtomicPointer<VisualJsonConfigure> pInstance;
	static QMutex pMutex;
};