#ifndef CHIP8_H
#define CHIP8_H


class chip8 : public QObject
{
    Q_OBJECT
public:
    explicit chip8(QObject *parent = 0);
    ~chip8();

signals:

public slots:
};

#endif // CHIP8_H
