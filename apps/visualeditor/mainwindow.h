#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class InteractiveGraphicsScene;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    InteractiveGraphicsScene* scene;

private slots:
    void splitLines();
    void exportLineSegments();
};

#endif  // MAINWINDOW_H
