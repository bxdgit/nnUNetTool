#include "nnUNetTool.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    nnUNetTool w;
    w.show();
    return a.exec();
}
