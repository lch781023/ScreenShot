
#include"screenwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScreenWidget w;
    w.show();
    return a.exec();
}
