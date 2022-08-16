#include "nnUNetTool.h"
#include "nlohmann/json.hpp"
#include <QDebug>

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

    qDebug() << QString::fromStdString(j_patch.dump(4));
}

nnUNetTool::~nnUNetTool()
{
    delete ui;
}
