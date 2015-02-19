#include "chip8.h"
#include <QDebug>

Chip8::Chip8(QObject *parent) : QThread(parent)
{
    delayTimer = new QTimer(this);
    connect(delayTimer, &QTimer::timeout, this, &Chip8::updateDelayTimer);
    drawTimer = new QTimer(this);
    //connect(drawTimer, &QTimer::timeout, this, &Chip8::drawGraphics);
}

Chip8::~Chip8()
{

}

void Chip8::run()
{
    while (running())
    {
        if (QThread::currentThread()->isInterruptionRequested())
        {
            return;
        }

        emulateCycle();

        if (drawFlag())
            emit drawScreen();

        setKeys();
    }
}

void Chip8::initialize()
{
    pc = 0x0200;
    opcode = 0;
    I = 0;
    sp = 0;

    unsigned char temp_fontset[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    for (int i = 0; i < 80; i++)
        chip8_fontset[i] = temp_fontset[i];

    setDrawFlag(false);
    for (int i = 0; i < (64 * 32); i++)
        gfx[i] = 0x0;
    for (int i = 0; i < 4096; i++)
        memory[i] = 0x0;
    for (int i = 0; i < 16; i++)
        stack[i] = 0x0;
    for (int i = 0; i < 16; i++)
        V[i] = 0x0;
    // Clear display
    // Clear stack
    // Clear registers V0-VF
    // Clear memory

    for (int i = 0; i < 80; i++)
        memory[i] = chip8_fontset[i];

    if (!loadGame("PONG.ch8"))
        exit(1);
    // Reset timers
}

bool Chip8::loadGame(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    QByteArray program(file.readAll());
    for (int i = 0; i < program.size(); i++)
        memory[i + 512] = program.at(i);
    file.close();

    return true;
}

void Chip8::startEmulation()
{
    setRunning(true);
    delayTimer->start(1000);
    start();
    drawTimer->start(16);
}

void Chip8::stopEmulation()
{
    setRunning(false);
    delayTimer->stop();
    drawTimer->stop();
}

void Chip8::emulateCycle()
{
    // fetch opcode
    opcode = memory[pc] << 8 | memory[pc + 1];
    // decode opcode
    decodeOpCode();

    // update timers
    if (delay_timer)
        delay_timer--;

    if (sound_timer)
    {
//        if (sound_timer == 1)
//            printf("BEEP!\n");
        sound_timer--;
    }
    usleep(1667);
    _opcodes++;
}

void Chip8::decodeOpCode()
{
    switch (opcode & 0xF000)
    {
    case 0x0000:
        switch (opcode)
        {
        case 0x00E0: // 00E0 | Clears the screen
            for (int i = 0; i < (64 * 32); i++)
                gfx[i] = 0x0;
            break;
        case 0x00EE: // 00EE | Return from a subroutine
            pc = stack[--sp];
            break;
        }
        break;
    case 0x1000: // 1NNN | Jumps to address NNN
        pc = opcodeNNN();
        break;
    case 0x2000: // 2NNN | Calls subroutine at NNN
        stack[sp++] = pc + 2;
        pc = opcodeNNN();
        break;
    case 0x3000: // 3XNN | Skips the next instruction if VX equals NN
        if (V[opcodeX()] == opcodeNN())
            skipNextInstruction();
        else
            nextInstruction();
        break;
    case 0x4000: // 4XNN | Skips the next instruction if VX doesn't equal NN
        if (V[opcodeX()] != opcodeNN())
            skipNextInstruction();
        else
            nextInstruction();
        break;
    case 0x5000: // 5XY0 | Skips the next instruction if VX equals VY
        if (V[opcodeX()] == V[opcodeY()])
            skipNextInstruction();
        else
            nextInstruction();
        break;
    case 0x6000: // 6XNN | Sets VX to NN
        V[opcodeX()] = opcodeNN();
        nextInstruction();
        break;
    case 0x7000: // 7XNN | Adds NN to VX
        V[opcodeX()] += opcodeNN();
        nextInstruction();
        break;
    case 0x8000:
        switch (opcodeN())
        {
        case 0x0000: // 8XY0 | Sets VX to the value of VY
            V[opcodeX()] = V[opcodeY()];
            nextInstruction();
            break;
        case 0x0001: // 8XY1 | Sets VX to VX OR VY
            V[opcodeX()] = V[opcodeX()] | V[opcodeY()];
            nextInstruction();
            break;
        case 0x0002: // 8XY2 | Sets VX to VX AND VY
            V[opcodeX()] = V[opcodeX()] & V[opcodeY()];
            nextInstruction();
            break;
        case 0x0003: // 8XY3 | Sets VX to VX XOR VY
            V[opcodeX()] = V[opcodeX()] ^ V[opcodeY()];
            nextInstruction();
            break;
        case 0x0004: // 8XY4 | Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't
            if (V[opcodeY()] > (0xFF - V[opcodeX()]))
                V[0xF] = 1;
            else
                V[0xF] = 0;
            V[opcodeX()] += V[opcodeY()];
            nextInstruction();
            break;
        case 0x0005: // 8XY5 | VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
            if (V[opcodeX()] > V[opcodeY()])
                V[0xF] = 1;
            else
                V[0xF] = 0;
            V[opcodeX()] = V[opcodeX()] - V[opcodeY()];
            nextInstruction();
            break;
        case 0x0006: // 8XY6 | Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift
            V[0xF] = V[opcodeX()] & 0x0001;
            V[opcodeX()] /= 2;
            nextInstruction();
            break;
        case 0x0007: // 8XY7 | Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
            if (V[opcodeY()] > V[opcodeX()])
                V[0xF] = 1;
            else
                V[0xF] = 0;
            V[opcodeX()] = V[opcodeY()] - V[opcodeX()];
            nextInstruction();
            break;
        case 0x000E: // 8XYE | Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift
            V[0xF] = V[opcodeX()] & 0x0001;
            V[opcodeX()] *= 2;
            nextInstruction();
        }
        break;
    case 0x9000: // 9XY0 | Skips the next instruction if VX doesn't equal VY
        if (V[opcodeX()] != V[opcodeY()])
            skipNextInstruction();
        else
            nextInstruction();
        break;
    case 0xA000: // ANNN | Sets I to the address NNN
        I = opcodeNNN();
        nextInstruction();
        break;
    case 0xB000: // BNNN | Jumps to the Address NNN plus V0
        pc = (opcodeNNN()) + V[0];
        break;
    case 0xC000: // CXNN | Sets VX to a random number, masked by NN
        V[opcodeX()] = (qrand() % 256) & (opcodeNN());
        nextInstruction();
        break;
    case 0xD000: // DXYN | Sprites stored in memory at location in index register I, maximum 8bits wide. Wraps around the screen. If when drawn, clears a pixel, register VF is set to 1 otherwise it is zero. All drawing is XOR drawing (i.e. it toggles the screen pixels)
        unsigned short x, y;
        x = V[opcodeX()];
        y = V[opcodeY()];
        unsigned short height;
        height = opcodeN();
        unsigned short pixel;

        V[0xF] = 0;
        for (int yline = 0; yline < height; yline++)
        {
            pixel = memory[I + yline];
            for (int xline = 0; xline < 8; xline++)
            {
                if ((pixel & (0x80 >> xline)) != 0)
                {
                    if(gfx[(x + xline + ((y + yline) * 64))] == 1)
                        V[0xF] = 1;
                    gfx[x + xline + ((y + yline) * 64)] ^= 1;
                }
            }
        }

        setDrawFlag(true);
        nextInstruction();
        break;
    case 0xE000:
        switch (opcodeNN())
        {
        case 0x009E: // EX9E | Skips the next instruction if the key stores in VX is pressed
            if (key[V[opcodeX()]] == 1)
                skipNextInstruction();
            else
                nextInstruction();
            break;
        case 0x00A1: // EXA1 | Skips the next instruction if the key stores in VX isn't pressed
            if (key[V[opcodeX()]] == 0)
                skipNextInstruction();
            else
                nextInstruction();
            break;
        }
        break;
    case 0xF000:
        switch (opcodeNN())
        {
        case 0x0007: // FX07 | Sets VX to the value of the delay timer
            V[opcodeX()] = delay_timer;
            nextInstruction();
            break;
        case 0x000A: // FX0A | A key press is awaited, and then stored in VX
            bool pressed;
            pressed = false;
            for (int i = 0; i < 16; i++)
            {
                if (key[i] != 0)
                {
                    V[opcodeX()] = i;
                    pressed = true;
                }
            }
            if (!pressed)
                return;
            nextInstruction();
            break;
        case 0x0015: // FX15 | Sets the delay timer to VX
            delay_timer = V[opcodeX()];
            nextInstruction();
            break;
        case 0x0018: // FX18 | Sets the sound timer to VX
            sound_timer = V[opcodeX()];
            nextInstruction();
            break;
        case 0x001E: // FX1E | Adds VX to I
            I += V[opcodeX()];
            nextInstruction();
            break;
        case 0x0029: // FX29 | Sets I to to the location of the sprite for the chracter in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
            I = 5 * V[opcodeX()];
            nextInstruction();
            break;
        case 0x0033: // FX33 | Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2
            memory[I]     = V[opcodeX()] / 100;
            memory[I + 1] = (V[opcodeX()] / 10) % 10;
            memory[I + 2] = (V[opcodeX()] % 100) % 10;
            nextInstruction();
            break;
        case 0x0055: // FX55 | Stores V0 to VX in memory starting at address I
            for (int i = 0; i <= (opcodeX()); i++)
            {
                memory[I + i] = V[i];
            }
            nextInstruction();
            break;
        case 0x0065: // FX65 | Fills V0 to VX with values from memory starting at address I
            for (int i = 0; i <= (opcodeX()); i++)
            {
                V[i] = memory[I + i];
            }
            nextInstruction();
            break;
        }
        break;
    }
}

void Chip8::setKeys()
{

}

void Chip8::setKeyPressed(int index)
{
   key[index] = 1;
}

void Chip8::setKeyReleased(int index)
{
    key[index] = 0;
}
bool Chip8::drawFlag() const
{
    return _drawFlag;
}

void Chip8::setDrawFlag(bool drawFlag)
{
    _drawFlag = drawFlag;
}
bool Chip8::running() const
{
    return _running;
}

void Chip8::setRunning(bool running)
{
    _running = running;
}
QTimer *Chip8::getDelayTimer() const
{
    return delayTimer;
}

void Chip8::setDelayTimer(QTimer *value)
{
    delayTimer = value;
}


inline short Chip8::opcodeX()
{
    return (opcode & 0x0F00) >> 8;
}

inline short Chip8::opcodeY()
{
    return (opcode & 0x00F0) >> 4;
}

inline short Chip8::opcodeN()
{
    return opcode & 0x000F;
}

inline short Chip8::opcodeNN()
{
    return opcode & 0x00FF;
}

inline short Chip8::opcodeNNN()
{
    return opcode & 0x0FFF;
}

inline void Chip8::nextInstruction()
{
    pc += 2;
}

inline void Chip8::skipNextInstruction()
{
    pc += 4;
}

void Chip8::updateDelayTimer()
{
    if (delay_timer > 0)
    {
        delay_timer--;
    }
}

void Chip8::drawGraphics()
{
    if (drawFlag())
    {
        requestInterruption();
        if (wait())
        {
            emit drawScreen();
        }
    }
    qDebug() << QString::number(_opcodes);
    _opcodes = 0;
    start();
}


