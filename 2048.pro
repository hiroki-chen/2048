#-------------------------------------------------
#
# Project created by QtCreator 2020-07-20T09:27:27
#
#-------------------------------------------------

QT       += core gui opengl \
            multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2048
TEMPLATE = app


SOURCES += main.cpp\
        game.cpp \
        widget.cpp

HEADERS  += widget.h \
    game.h

CONFIG += mobility
MOBILITY = 

ICON = 2048.icns

RESOURCES += \
    res.qrc
