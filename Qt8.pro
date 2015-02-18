#-------------------------------------------------
#
# Project created by QtCreator 2015-02-14T01:07:54
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt8
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    chip8.cpp \
    qlwidget.cpp \
    display.cpp

HEADERS  += mainwindow.h \
    chip8.h \
    qlwidget.h \
    display.h

FORMS    += mainwindow.ui
