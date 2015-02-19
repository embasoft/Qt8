#include "display.h"

Display::Display(QWidget *parent) : QGraphicsView(parent)
{

}

Display::~Display()
{

}
bool Display::chip8Set() const
{
    return _chip8Set;
}

void Display::setChip8Set(bool chip8Set)
{
    _chip8Set = chip8Set;
}
Chip8 *Display::getChip8() const
{
    return chip8;
}

void Display::setChip8(Chip8 *value)
{
    chip8 = value;
    setChip8Set(true);
}

void Display::keyPressEvent(QKeyEvent *event)
{
    if (chip8Set())
    {
        switch (event->key())
        {
        case Qt::Key_1:
            chip8->setKeyPressed(0x1);
            break;
        case Qt::Key_2:
            chip8->setKeyPressed(0x2);
            break;
        case Qt::Key_3:
            chip8->setKeyPressed(0x3);
            break;
        case Qt::Key_4:
            chip8->setKeyPressed(0xC);
            break;
        case Qt::Key_Q:
            chip8->setKeyPressed(0x4);
            break;
        case Qt::Key_W:
            chip8->setKeyPressed(0x5);
            break;
        case Qt::Key_E:
            chip8->setKeyPressed(0x6);
            break;
        case Qt::Key_R:
            chip8->setKeyPressed(0xD);
            break;
        case Qt::Key_A:
            chip8->setKeyPressed(0x7);
            break;
        case Qt::Key_S:
            chip8->setKeyPressed(0x8);
            break;
        case Qt::Key_D:
            chip8->setKeyPressed(0x9);
            break;
        case Qt::Key_F:
            chip8->setKeyPressed(0xE);
            break;
        case Qt::Key_Y:
            chip8->setKeyPressed(0xA);
            break;
        case Qt::Key_X:
            chip8->setKeyPressed(0x0);
            break;
        case Qt::Key_C:
            chip8->setKeyPressed(0xB);
            break;
        case Qt::Key_V:
            chip8->setKeyPressed(0xF);
            break;
        }
    }
}

void Display::keyReleaseEvent(QKeyEvent *event)
{
    if (chip8Set())
    {
        switch (event->key())
        {
        case Qt::Key_1:
            chip8->setKeyReleased(0x1);
            break;
        case Qt::Key_2:
            chip8->setKeyReleased(0x2);
            break;
        case Qt::Key_3:
            chip8->setKeyReleased(0x3);
            break;
        case Qt::Key_4:
            chip8->setKeyReleased(0xC);
            break;
        case Qt::Key_Q:
            chip8->setKeyReleased(0x4);
            break;
        case Qt::Key_W:
            chip8->setKeyReleased(0x5);
            break;
        case Qt::Key_E:
            chip8->setKeyReleased(0x6);
            break;
        case Qt::Key_R:
            chip8->setKeyReleased(0xD);
            break;
        case Qt::Key_A:
            chip8->setKeyReleased(0x7);
            break;
        case Qt::Key_S:
            chip8->setKeyReleased(0x8);
            break;
        case Qt::Key_D:
            chip8->setKeyReleased(0x9);
            break;
        case Qt::Key_F:
            chip8->setKeyReleased(0xE);
            break;
        case Qt::Key_Y:
            chip8->setKeyReleased(0xA);
            break;
        case Qt::Key_X:
            chip8->setKeyReleased(0x0);
            break;
        case Qt::Key_C:
            chip8->setKeyReleased(0xB);
            break;
        case Qt::Key_V:
            chip8->setKeyReleased(0xF);
            break;
        }
    }
}



