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
    robot.cpp\
    zadanie.cpp\
    smith.cpp


HEADERS  += mainwindow.h \
    refreasher.h \
    sterownik.h \
    shelf.h \
    coordinates.h \
    robot.h\
    zadanie.h\
    smith.h


FORMS    += mainwindow.ui

RESOURCES += \
    zasoby.qrc


