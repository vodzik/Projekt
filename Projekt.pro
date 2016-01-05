#-------------------------------------------------
#
# Project created by QtCreator 2015-12-16T01:28:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Projekt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    refreasher.cpp \
    sterownik.cpp \
    shelf.cpp \
    coordinates.cpp \
    zadanie.cpp

HEADERS  += mainwindow.h \
    refreasher.h \
    sterownik.h \
    shelf.h \
    coordinates.h \
    zadanie.h

FORMS    += mainwindow.ui

RESOURCES += \
    zasoby.qrc


