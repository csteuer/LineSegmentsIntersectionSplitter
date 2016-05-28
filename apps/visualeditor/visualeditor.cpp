#include <QApplication>

#include "mainwindow.h"

int main(int argc, char** args) {
    QApplication app(argc, args);

    MainWindow mainWindow;
    mainWindow.showMaximized();

    return app.exec();
}
