#pragma once
#include <QObject>
#include <QProcess>

class vtkImageData;
class VisualnnUNetCMDPrivate;
class  VisualnnUNetCMD : public QObject
{
	Q_OBJECT
public:
	VisualnnUNetCMD(QObject* parent = 0);
	~VisualnnUNetCMD();

	void SetInputFile(const QString inputFile);
	void SetInputFloder(const QString inputFloder);
	void SetOutputFile(const QString outputFile);
	void SetOutputFloder(const QString outputFloder);
	void SetWeightsFile(const QString wf);
	void SetCmdName(const QString cmdName);

	void SetFileMinSize(qint64 size);

	//void Setlabelmap(vtkImageData* imgData);
	//vtkImageData* Getlabelmap() const;

public slots:
	void Run();

signals:
	void getmaskImageFile(const QStringList&);
	void logInfo(const QString& info);
	void finished();
protected  slots:
	virtual void nnUNetFinished(int exitCode, QProcess::ExitStatus exitStatus);

protected:
	virtual bool nnUNetCommand();
	void nnUNet_predicet_Task002_VerSe();

protected:
	QScopedPointer<VisualnnUNetCMDPrivate> d_ptr;

private:
	Q_DECLARE_PRIVATE(VisualnnUNetCMD);
	Q_DISABLE_COPY(VisualnnUNetCMD);
};
