#pragma once

#include "VisualnnUNetCMD.h"

#include <QWidget>
#include "ui_nnUNetTool.h"


QT_BEGIN_NAMESPACE
namespace Ui { class nnUNetTool; };
QT_END_NAMESPACE

class nnUNetTool : public QWidget
{
    Q_OBJECT

public:
    nnUNetTool(QWidget *parent = nullptr);
    ~nnUNetTool();
protected:
    void InitUI();

protected slots:
    
    void on_InDirTBtn_clicked();
    void on_InFileTBtn_clicked();
    void on_outDirTBtn_clicked();
    void on_RunTBtn_clicked();
    void updateCmd(const QString& fileName);
    void updataModel(const QString& fileName);

private:
    Ui::nnUNetTool*ui;
    VisualnnUNetCMD* m_nnUNetCmd = nullptr;
    QString m_inputDir;
    QString m_outputDir;
    QString m_inputFile;
    QString m_modelFile;
    QString m_cmdName;
  
};
