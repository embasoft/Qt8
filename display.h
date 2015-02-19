#ifndef DISPLAY_H
#define DISPLAY_H

#include "chip8.h"
#include <QGraphicsView>
#include <QKeyEvent>

class Display : public QGraphicsView
{
public:
    Display(QWidget *parent = 0);
    ~Display();
    bool chip8Set() const;
    void setChip8Set(bool chip8Set);

    Chip8 *getChip8() const;
    void setChip8(Chip8 *value);

private:
    Chip8 *chip8;
    bool _chip8Set = false;
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // DISPLAY_H
