#-------------------------------------------------
#
# Project created by QtCreator 2016-02-09T00:25:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = aaa_test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    renderarea.cpp \
    shape.cpp \
    renderthread.cpp

HEADERS  += mainwindow.h \
    renderarea.h \
    shape.h \
    renderthread.h

FORMS    += mainwindow.ui
