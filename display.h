#ifndef DISPLAY_H
#define DISPLAY_H

#include <QGraphicsView>

class Display : public QGraphicsView
{
public:
    Display(QWidget *parent = 0);
    ~Display();
};

#endif // DISPLAY_H
