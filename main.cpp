#include "FreakyfierQt.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    FreakyfierQt w;
    w.show();
    return a.exec();
}