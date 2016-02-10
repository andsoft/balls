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
    renderthread.cpp \
    circle.cpp

HEADERS  += mainwindow.h \
    renderarea.h \
    renderthread.h \
    circle.h

FORMS    += mainwindow.ui
