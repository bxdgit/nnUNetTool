#include "VisualnnUNetCMD.h"


//QT includes
#include <QDebug>
#include <QSharedPointer>
#include <QString>
#include <QProcess>
#include <QMessageBox>
#include <QDir>
#include <QStandardPaths.h>
#include <QFile>
#include <QTextStream>
#include <QApplication>

//STD includes
#include <deque>
#include <vector>
#include <set>

#include <Windows.h>
//#include <QtWidgets/qapplication.h>



class VisualSegment;
class VisualnnUNetCMDPrivate
{
	Q_DECLARE_PUBLIC(VisualnnUNetCMD);
protected:
	VisualnnUNetCMD* const q_ptr;

public:
	VisualnnUNetCMDPrivate(VisualnnUNetCMD& object);
	~VisualnnUNetCMDPrivate();
	void GetValidFile(const QString& dirName, QStringList& fileNames, QString filter = "*.nii.gz", qint64 minSize = 0);
	//void DeleteLabmapImage();

	QProcess* m_nnUNetProcess;
	QString m_inputFolder;
	QString m_inputFile;
	QString m_outputFolder;
	QString m_outputFile;
	QString nnUNet_raw_data_base;
	QString nnUNet_preprocessed;
	QString RESULTS_FOLDER;
	QString m_weightsFile;
	QString m_cmdName;
	//vtkSmartPointer<vtkImageData> m_labmapImage;
	QWidget* m_BusyWidget = nullptr;
	//QMap<QString, vtkImageData*> m_labmapImageMaps;
	qint64 m_fileMinSize = 0;
};

//---------------------------------------------------------------------------
VisualnnUNetCMDPrivate::VisualnnUNetCMDPrivate(VisualnnUNetCMD& object)
	:q_ptr(&object)
	, m_nnUNetProcess(Q_NULLPTR)
	, nnUNet_raw_data_base("")
	, nnUNet_preprocessed("")
	, m_weightsFile("")
	, m_inputFolder("")
	, m_cmdName("")
{
	//m_inputFolder = "c:/project/data/predict_input/Task002_VerSe_crop/11_0000.nii.gz";
	m_inputFile = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/VisualSpine/" + "121_0000.nii.gz";
	m_outputFolder = "C:/project/data/predict_output/Task002_lowres_crop";
	m_weightsFile = "C:/project/data/nnUNet_trained_models/nnUNet/3d_lowres/Task002_VerSe/nnUNetTrainerV2__nnUNetPlansv2.1/all/model_best.model";
	m_cmdName = "nnUNet_predict_single.exe";

}

//---------------------------------------------------------------------------
VisualnnUNetCMDPrivate::~VisualnnUNetCMDPrivate()
{
	Q_Q(VisualnnUNetCMD);
	//DeleteLabmapImage();
}

//---------------------------------------------------------------------------
void VisualnnUNetCMDPrivate::GetValidFile(const QString& dirName, QStringList& fileNames, QString filter, qint64 minSize)
{
	// 只获取当前目录的文件
	QDir dir(dirName);
	QStringList nameFilters;
	nameFilters << filter;
	//fileNames = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
	QFileInfoList infoList = dir.entryInfoList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
	foreach(QFileInfo info, infoList)
	{
		if (info.size() < minSize)
			continue;
		fileNames.append(info.filePath());
	}

}

//---------------------------------------------------------------------------
VisualnnUNetCMD::VisualnnUNetCMD(QObject* parent)
	:QObject(parent), d_ptr(new VisualnnUNetCMDPrivate(*this))
{
	Q_D(VisualnnUNetCMD);

	//nnUNet 
	d->m_nnUNetProcess = new QProcess();
	QObject::connect(d->m_nnUNetProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
		[this](int exitCode, QProcess::ExitStatus exitStatus) { nnUNetFinished(exitCode, exitStatus); });

	QObject::connect(d->m_nnUNetProcess, &QProcess::readyReadStandardOutput, [d,this]() {
		QString cmdOutputString;
		//std::cout << d->m_nnUNetProcess->readAllStandardOutput().data() << std::endl;
		cmdOutputString.append(d->m_nnUNetProcess->readAllStandardOutput().data());
		emit logInfo(cmdOutputString);
		//std::cout << cmdOutputString.toStdString() << std::endl;
		});

	QObject::connect(d->m_nnUNetProcess, &QProcess::readyReadStandardError, this, [d]() {
		QMessageBox::information(0, "Error", d->m_nnUNetProcess->readAllStandardError().data());
		});

	QObject::connect(d->m_nnUNetProcess, &QProcess::errorOccurred, [d]() {
		qWarning() << d->m_nnUNetProcess->errorString();
	/*	d->m_BusyWidget->hide();
		d->m_BusyWidget->deleteLater();*/
		});



	//QObject::connect(this, &VisualnnUNetCMD::getmaskImageFile, [d,this](const QStringList& files) {

	//	if (files.isEmpty())
	//		return;

	//	//d->DeleteLabmapImage();

	//	foreach(auto f, files)
	//	{
	//		QFileInfo info(f);
	//		vtkImageData* imgData = vtkImageData::New();
	//		vtkImageDataIO::readImageDataFromNiiGz(f.toStdString().c_str(), imgData);
	//		d->m_labmapImageMaps[info.baseName()] = imgData;
	//	}

	//	emit finished();

	//	});
}

//---------------------------------------------------------------------------
VisualnnUNetCMD::~VisualnnUNetCMD()
{
	Q_D(VisualnnUNetCMD);
	if (d->m_nnUNetProcess)
	{
		d->m_nnUNetProcess->closeWriteChannel();
		d->m_nnUNetProcess->close();
		d->m_nnUNetProcess->waitForFinished();
	}
}

//---------------------------------------------------------------------------
void VisualnnUNetCMD::SetInputFile(const QString inputFile)
{
	Q_D(VisualnnUNetCMD);
	d->m_inputFile = inputFile;
}

//---------------------------------------------------------------------------
void VisualnnUNetCMD::SetInputFloder(const QString inputFloder)
{
	Q_D(VisualnnUNetCMD);
	d->m_inputFolder = inputFloder;
}

//---------------------------------------------------------------------------
void VisualnnUNetCMD::SetOutputFile(const QString outputFile)
{
	Q_D(VisualnnUNetCMD);
	d->m_outputFile = outputFile;
}

//---------------------------------------------------------------------------
void VisualnnUNetCMD::SetOutputFloder(const QString outputFloder)
{
	Q_D(VisualnnUNetCMD);
	d->m_outputFolder = outputFloder;
}

//---------------------------------------------------------------------------
void VisualnnUNetCMD::SetWeightsFile(const QString wf)
{
	Q_D(VisualnnUNetCMD);
	d->m_weightsFile = wf;
}

//---------------------------------------------------------------------------
void VisualnnUNetCMD::SetCmdName(const QString cmdName)
{
	Q_D(VisualnnUNetCMD);
	d->m_cmdName = cmdName;
}

//---------------------------------------------------------------------------
void VisualnnUNetCMD::SetFileMinSize(qint64 size)
{
	Q_D(VisualnnUNetCMD);
	d->m_fileMinSize = size;
}

//---------------------------------------------------------------------------
void VisualnnUNetCMD::Run()
{
	Q_D(VisualnnUNetCMD);
	//d->m_BusyWidget = QW(tr("正在计算......"), NULL);
	if (!nnUNetCommand())
	{
		/*d->m_BusyWidget->hide();
		d->m_BusyWidget->deleteLater();*/
	}
}

//---------------------------------------------------------------------------
bool VisualnnUNetCMD::nnUNetCommand()
{
	Q_D(VisualnnUNetCMD);

#if 0
	//获取环境变量
	d->nnUNet_raw_data_base = QProcessEnvironment::systemEnvironment().value(QString("nnUNet_raw_data_base"));
	d->nnUNet_preprocessed = QProcessEnvironment::systemEnvironment().value(QString("nnUNet_preprocessed"));
	d->RESULTS_FOLDER = QProcessEnvironment::systemEnvironment().value(QString("RESULTS_FOLDER"));
	qDebug() << "nnUNet_raw_data_base:" << d->nnUNet_raw_data_base;
	qDebug() << "nnUNet_preprocessed:" << d->nnUNet_preprocessed;
	qDebug() << "RESULTS_FOLDER:" << d->RESULTS_FOLDER;
#endif

	if (d->m_nnUNetProcess == Q_NULLPTR)
		return false;
	if (d->m_cmdName.isEmpty())
	{
		qCritical() << "cmd is null";
		return false;
	}
	QStringList nnUNetCmd;
	//nnUNet_predict_single - i c:/project/data/predict_input/Task002_VerSe/ 1.3.6.1.4.1.9328.50.4.0090_0000.nii.gz -o
	//c:/project/data/predict_output/ -m c:/project/data/nnUNet_trained_models/nnUNet/3d_lowres/Task002_VerSe/nnUNetTrainerV2__nnUNetPlansv2.1/all/model_best.model
	if (d->m_cmdName == "nnUNet_predict_single.exe")
	{
		if (d->m_inputFile.isEmpty() || d->m_weightsFile.isEmpty() || d->m_outputFolder.isEmpty())
		{
			qCritical() << "file not exist!";
			return false;
		}

		nnUNetCmd.append("-i");
		nnUNetCmd.append(d->m_inputFile);
		nnUNetCmd.append("-o");
		nnUNetCmd.append(d->m_outputFolder);
		nnUNetCmd.append("-m");
		nnUNetCmd.append(d->m_weightsFile);

		//qDebug() << "start nnUNet.....";
		//qDebug() << nnUNetCmd;
	}
	else if (d->m_cmdName == "nnUNet_predict_multiple.exe")
	{
		if (d->m_inputFolder.isEmpty() || d->m_weightsFile.isEmpty() || d->m_outputFolder.isEmpty())
		{
			qCritical() << "file not exist!";
			return false;
		}

		nnUNetCmd.append("-i");
		nnUNetCmd.append(d->m_inputFolder);
		nnUNetCmd.append("-o");
		nnUNetCmd.append(d->m_outputFolder);
		nnUNetCmd.append("-m");
		nnUNetCmd.append(d->m_weightsFile);

		//qDebug() << "start nnUNet.....";
		//qDebug() << nnUNetCmd;
	}


	//d->m_nnUNetProcess->start(d->m_cmdName, nnUNetCmd);
	WriteHistoryCmd(d->m_cmdName, nnUNetCmd);
	return true;
}

//---------------------------------------------------------------------------
void VisualnnUNetCMD::WriteHistoryCmd(const QString& cmd, const QStringList& Parameter)
{
	QString fileName = qApp->applicationDirPath() + "/history.txt";
	QFile historyFile(fileName);
	if (historyFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text))
	{
		QTextStream stream(&historyFile);
		//stream.seek(file.size());
		stream << cmd << " ";

		foreach(const QString & str, Parameter)
		{
			stream << str << " ";
		}
		stream << "\n";
		historyFile.flush();
	}

	historyFile.close();
}

//---------------------------------------------------------------------------
void VisualnnUNetCMD::nnUNetFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_D(VisualnnUNetCMD);
	if (QProcess::NormalExit == exitCode)
	{
		QStringList fileList;
		QStringList nameList;
		d->GetValidFile(d->m_outputFolder, fileList, "*.nii.gz", d->m_fileMinSize);
		emit getmaskImageFile(fileList);
	}
	else
	{
		QMessageBox::critical(0, "Failed", "nnUNet process finished!");
	}

	emit finished();

	/*d->m_BusyWidget->hide();
	d->m_BusyWidget->deleteLater();*/
}

