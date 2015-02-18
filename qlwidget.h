#ifndef QLWIDGET_H
#define QLWIDGET_H

#include "chip8.h"
#include <QGLWidget>
#include <QKeyEvent>

class qlWidget : public QGLWidget
{
public:
    qlWidget(QWidget *parent = 0);
    ~qlWidget();

    Chip8 *getChip8() const;
    void setChip8(Chip8 *value);

    bool chip8Set() const;
    void setChip8Set(bool chip8Set);

private:
    Chip8 *chip8;
    bool _chip8Set = false;

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    // Input
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // QLWIDGET_H
