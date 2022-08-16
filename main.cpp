#include "nnUNetTool.h"
#include "Stylesheet.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Stylesheet::loadStyle();
    nnUNetTool w;
    w.show();
    return a.exec();
}
