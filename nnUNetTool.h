#pragma once

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

private:
    Ui::nnUNetTool*ui;
};
