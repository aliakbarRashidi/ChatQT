#-------------------------------------------------
#
# Project created by QtCreator 2016-06-24T12:33:49
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatQT
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp \
    runnable.cpp

HEADERS  += mainwindow.h \
    server.h \
    runnable.h

FORMS    += mainwindow.ui
