#ifndef CHIP8_H
#define CHIP8_H

#include <QObject>
#include <QThread>
#include <QByteArray>
#include <QFile>
#include <QTimer>

class Chip8 : public QThread
{
    Q_OBJECT
public:
    explicit Chip8(QObject *parent = 0);
    ~Chip8();

    void run() Q_DECL_OVERRIDE;

    unsigned short opcode;
    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short I;
    unsigned short pc;
    unsigned char gfx[64 * 32];
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short stack[16];
    unsigned short sp;
    unsigned char key[16];
    unsigned char chip8_fontset[80];

    void initialize();
    bool loadGame(QString path);
    void startEmulation();
    void stopEmulation();
    void emulateCycle();
    void decodeOpCode();
    void setKeys();
    void setKeyPressed(int index);
    void setKeyReleased(int index);

    bool drawFlag() const;
    void setDrawFlag(bool drawFlag);

    bool running() const;
    void setRunning(bool running);

    QTimer *getDelayTimer() const;
    void setDelayTimer(QTimer *value);

private:
    int _opcodes;
    bool _drawFlag;
    bool _running = false;
    QTimer *delayTimer;
    QTimer *soundTimer;
    QTimer *drawTimer;
    short opcodeX();
    short opcodeY();
    short opcodeN();
    short opcodeNN();
    short opcodeNNN();
    void nextInstruction();
    void skipNextInstruction();

signals:
    void resultReady(const QString &s);
    void drawScreen();

public slots:
    void updateDelayTimer();
    void drawGraphics();
};

#endif // CHIP8_H
