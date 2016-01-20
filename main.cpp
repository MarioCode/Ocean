#include "ocean.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Ocean w;
    w.show();

    return a.exec();
}
