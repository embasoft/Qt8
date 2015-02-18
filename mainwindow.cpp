#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->glWidget->setChip8(&chip8);
    scene = new QGraphicsScene(this);
    pixmap = new QPixmap(64, 32);
    painter = new QPainter();
    connect(&chip8, &Chip8::drawScreen, this, &MainWindow::drawGraphic);
    ui->graphicsView->setScene(scene);
    connect(scene, SIGNAL(changed(QList<QRectF>)), ui->graphicsView, SLOT(updateScene(QList<QRectF>)));

    chip8.initialize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startEmulation()
{
    chip8.setRunning(true);
    chip8.run();
}

void MainWindow::stopEmulation()
{
    chip8.setRunning(false);
}

void MainWindow::drawGraphic()
{
    qDebug() << painter->begin(pixmap);
    for (int i = 0; i < (64 * 32); i++)
    {
        if (chip8.gfx[i])
            painter->setPen(Qt::white);
        else
            painter->setPen(Qt::black);
        painter->drawPoint(i % 64, i / 64);
    }
    painter->end();
    chip8.setDrawFlag(false);
}

void MainWindow::on_actionStart_Emulation_triggered()
{
    startEmulation();
}

void MainWindow::on_actionStop_Emulation_triggered()
{
    stopEmulation();
}
