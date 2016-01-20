#-------------------------------------------------
#
# Project created by QtCreator 2016-01-19T20:46:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ocean
TEMPLATE = app


SOURCES += main.cpp\
        ocean.cpp \
    drawing_scene.cpp \
    fish.cpp \
    predators.cpp \
    victims.cpp

HEADERS  += ocean.h \
    drawing_scene.h \
    fish.h \
    predators.h \
    victims.h

FORMS    += ocean.ui
