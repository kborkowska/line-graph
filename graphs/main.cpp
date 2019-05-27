#include "mainwindow.h"
#include "drawwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DrawWindow *drawWindow = new DrawWindow();
    Iligra *iligra = new Iligra();
    drawWindow->setIligra(iligra);
    drawWindow->show();
    drawWindow->update();

    return a.exec();
}
