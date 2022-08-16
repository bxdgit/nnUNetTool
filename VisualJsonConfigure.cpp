#include "VisualJsonConfigure.h"
#include "nlohmann/json.hpp"

#include <QFile>
#include <QGuiApplication>


QAtomicPointer<VisualJsonConfigure> VisualJsonConfigure::pInstance = 0;
QMutex VisualJsonConfigure::pMutex;

class VisualJsonConfigurePrivate : QObject
{
	Q_DECLARE_PUBLIC(VisualJsonConfigure);
protected:
	VisualJsonConfigure* const q_ptr;
public:
	VisualJsonConfigurePrivate(VisualJsonConfigure* parent) : q_ptr(parent) {};
	~VisualJsonConfigurePrivate() {};
	bool FindEffectiveKey(const nlohmann::json& node, const QString& key);

	QString m_configureFile;
	nlohmann::json m_configureJson;
};

bool VisualJsonConfigurePrivate::FindEffectiveKey(const nlohmann::json& node, const QString& key)
{
	if (node.is_null() || !node.is_object()) return false;
	if (node.find(key.toUtf8().data()) == node.end()) return false;

	return true;
}

VisualJsonConfigure* VisualJsonConfigure::Instance()
{
#ifndef Q_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE
	if (!QAtomicPointer::isTestAndSetNative())
	{
#ifdef CONSOLEDEBUG
		qDebug() << "Error: TestAndSetNative not supported!";
#endif
	}

#endif
	if (pInstance.testAndSetOrdered(0, 0))
	{
		QMutexLocker locker(&pMutex);
		pInstance.testAndSetOrdered(0, new VisualJsonConfigure);
	}

	return pInstance;
}

void VisualJsonConfigure::DeleteInstance()
{
	if (nullptr != pInstance)
		delete pInstance;
	pInstance = nullptr;
}

void VisualJsonConfigure::SetJsonConfigureFile(const QString& file)
{
	d_ptr->m_configureFile = file;
}

QString VisualJsonConfigure::GetJsonConfigureFile() const
{
	return d_ptr->m_configureFile;
}

void VisualJsonConfigure::SetStringValue(const QString& group, const QString& key, const QString& value)
{
	d_ptr->m_configureJson[group.toUtf8().data()][key.toUtf8().data()] = value.toUtf8().data();
}

QString VisualJsonConfigure::GetValueAsString(const QString& group, const QString& key)
{
	if (d_ptr->FindEffectiveKey(d_ptr->m_configureJson, group))
	{
		if (d_ptr->FindEffectiveKey(d_ptr->m_configureJson[group.toUtf8().data()], key))
		{
			std::string valString = d_ptr->m_configureJson[group.toUtf8().data()][key.toUtf8().data()];
			return QString::fromStdString(valString);
		}
	}

	return QString();
}


QStringList VisualJsonConfigure::GetValueAsStringList(const QString& key)
{
	QStringList values;
	auto JsonObject = d_ptr->m_configureJson[key.toUtf8().data()];
	if (JsonObject.is_null())
	{
		return values;
	}
	else if (JsonObject.is_array())
	{
		for (nlohmann::json::iterator it = JsonObject.begin(); it != JsonObject.end(); ++it) {
			if (it.value().is_string())
			{
				values << QString::fromStdString(it.value());
			}
		}
	}
	else if (JsonObject.is_string())
	{

	}

	return values;
}

QStringList VisualJsonConfigure::GetAllGroups() const
{
	QStringList result;
	for(auto it = d_ptr->m_configureJson.begin(); it != d_ptr->m_configureJson.end(); it++)
		result << QString::fromStdString(it.key());

	return result;
}

QStringList VisualJsonConfigure::GetGroupKeys(const QString& group) const
{
	QStringList result;
	if (d_ptr->FindEffectiveKey(d_ptr->m_configureJson, group))
	{
		auto groupJson = d_ptr->m_configureJson[group.toUtf8().data()];
		for (auto it = groupJson.begin(); it != groupJson.end(); it++)
			result << QString::fromStdString(it.key());
	}

	return result;
}

QString VisualJsonConfigure::GetGroupNode(const QString& group) const
{
	QString result;
	if (d_ptr->FindEffectiveKey(d_ptr->m_configureJson, group))
		result = QString::fromStdString(d_ptr->m_configureJson[group.toUtf8().data()].dump(4));

	return result;
}

void VisualJsonConfigure::Save()
{
	QFile saveFile(d_ptr->m_configureFile);
	if(saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		saveFile.write(d_ptr->m_configureJson.dump(4).c_str());
		saveFile.close();
	}
}

void VisualJsonConfigure::Read()
{
	QFile readFile(d_ptr->m_configureFile);
	if(readFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString jsonString = readFile.readAll();
		d_ptr->m_configureJson = nlohmann::json::parse(jsonString.toUtf8().data());

		readFile.close();
	}
}

VisualJsonConfigure::VisualJsonConfigure(QObject* parent)
	:QObject(parent), d_ptr(new VisualJsonConfigurePrivate(this))
{
	d_ptr->m_configureFile = qApp->applicationDirPath() + "/configure.json";
	Read();
}

VisualJsonConfigure::~VisualJsonConfigure()
{
}
