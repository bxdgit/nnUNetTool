#include "nnUNetTool.h"
#include "VisualJsonConfigure.h"
#include "nlohmann/json.hpp"

#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>

nnUNetTool::nnUNetTool(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::nnUNetTool())
{
    ui->setupUi(this);
    nlohmann::json j_patch = R"([
  { "op": "replace", "path": "/baz", "value": "boo" },
  { "op": "add", "path": "/hello", "value": ["world"] },
  { "op": "remove", "path": "/foo"}
])"_json;

	InitUI();
	m_nnUNetCmd = new VisualnnUNetCMD(this);

	//    "Use currentIndexChanged(int) instead, and get the text using itemText(index)")
	//   void currentIndexChanged(const QString &);
	QObject::connect(ui->comboBox_cmd, SIGNAL(currentTextChanged(const QString&)), this, SLOT(updateCmd(const QString&)));
	QObject::connect(m_nnUNetCmd, &VisualnnUNetCMD::logInfo, [this](const QString& info) {

		ui->textBrowser->insertPlainText(info + "\n");

		});

	QObject::connect(m_nnUNetCmd, &VisualnnUNetCMD::finished, [this]() {

		ui->textBrowser->insertPlainText(QString("程序执行结束") + "\n");
		ui->RunTBtn->setEnabled(true);

		});
   // qDebug() << QString::fromStdString(j_patch.dump(4));
}

nnUNetTool::~nnUNetTool()
{
    delete ui;
}

void nnUNetTool::InitUI()
{
	ui->InFileTBtn->setVisible(false);
	const auto model_name_list = JsonManager->GetValueAsStringList("models");
	const auto nnUnet_exe_list = JsonManager->GetValueAsStringList("cmds");

	if (model_name_list.isEmpty()|| nnUnet_exe_list.isEmpty())
	{
		//QMessbxo
	}

	ui->comboBox_cmd->addItems(nnUnet_exe_list);
	ui->comboBox_model->addItems(model_name_list);

	m_cmdName = ui->comboBox_cmd->currentText();
	m_modelFile = ui->comboBox_model->currentText();
}

void nnUNetTool::on_InDirTBtn_clicked()
{
	m_inputDir = QFileDialog::getExistingDirectory(NULL, tr("Volume Dir"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
	if (!m_inputDir.isEmpty())
	{
		ui->textBrowser->insertPlainText(m_inputDir +"\n");
	}
}
void nnUNetTool::on_InFileTBtn_clicked()
{
	m_inputFile = QFileDialog::getOpenFileName(NULL, tr("Volume File"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "Volume Files (*.nii.gz)");

	if (!m_inputFile.isEmpty()) {
		ui->textBrowser->insertPlainText(m_inputFile + "\n");
	}

}

void nnUNetTool::on_outDirTBtn_clicked()
{
	m_outputDir = QFileDialog::getExistingDirectory(NULL, tr("输出目录"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
	if (!m_outputDir.isEmpty())
	{
		ui->textBrowser->insertPlainText(m_outputDir + "\n");
	}
}

void nnUNetTool::updateCmd(const QString& fileName)
{
	m_cmdName = fileName;
	ui->textBrowser->insertPlainText(m_cmdName + "\n");
	if (m_cmdName == "nnUNet_predict_single.exe")
	{
		ui->InDirTBtn->setVisible(false);
		ui->InFileTBtn->setVisible(true);
	}
	else
	{
		ui->InDirTBtn->setVisible(true);
		ui->InFileTBtn->setVisible(false);
	}
}

void nnUNetTool::updataModel(const QString& fileName)
{
	m_modelFile = fileName;
	ui->textBrowser->insertPlainText(m_modelFile + "\n");
}

void nnUNetTool::on_RunTBtn_clicked()
{
	m_nnUNetCmd->SetCmdName(m_cmdName);
	m_nnUNetCmd->SetWeightsFile(m_modelFile);
	m_nnUNetCmd->SetInputFloder(m_inputDir);
	m_nnUNetCmd->SetInputFile(m_inputFile);
	m_nnUNetCmd->SetOutputFloder(m_outputDir);
	m_nnUNetCmd->Run();
	ui->RunTBtn->setEnabled(false);
	ui->textBrowser->insertPlainText(m_cmdName + " "+ m_modelFile + " " + m_inputDir + " " + m_outputDir);
}
