#-------------------------------------------------
#
# Project created by QtCreator 2014-10-02T11:07:02
#
#-------------------------------------------------

QT       += core gui
QT       += websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    csnetmanager.cpp \
    csaccount.cpp \
    cschannel.cpp \
    csmessage.cpp \
    csclientmanager.cpp

HEADERS  += mainwindow.h \
    csnetmanager.h \
    csaccount.h \
    cschannel.h \
    csmessage.h \
    csclientmanager.h

FORMS    += mainwindow.ui
