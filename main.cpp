#include "mainwindow.h"

#include <QApplication>
#include <QScreen>
#include <QtCore>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile styleSheetFile(":/images/Darkeum.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    a.setStyleSheet(styleSheet);

    MainWindow w;

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    float height = screenGeometry.height();
    float width = screenGeometry.width();
    w.resize(width / 1.1, height / 1.1);

    w.show();
    return a.exec();
}
