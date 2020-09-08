#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.setWindowTitle(QString("2048小游戏"));
    w.show();

    return a.exec();
}
