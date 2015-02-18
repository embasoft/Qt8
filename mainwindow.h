#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chip8.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Chip8 chip8;
    void startEmulation();
    void stopEmulation();

public slots:
    void drawGraphic();

private slots:
    void on_actionStart_Emulation_triggered();

    void on_actionStop_Emulation_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QPixmap *pixmap;
    QPainter *painter;
};

#endif // MAINWINDOW_H
