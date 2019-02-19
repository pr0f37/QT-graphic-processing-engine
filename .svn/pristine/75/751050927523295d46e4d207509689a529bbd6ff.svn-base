#include <QtGui>
#include "mainwindow.h"
#include "connection.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (!createConnection())
        return 1;
    MainWindow w;
    w.show();

    return a.exec();
}
