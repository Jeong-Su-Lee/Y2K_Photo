#include "firstwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FirstWindow f;
    f.show();

    return a.exec();
}
