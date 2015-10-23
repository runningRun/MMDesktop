#-------------------------------------------------
#
# Project created by QtCreator 2015-10-13T16:51:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MMDesktop
TEMPLATE = app


SOURCES += main.cpp\
    globalxagentocx.cpp \
    mmdbasewidget.cpp \
    mainwidget.cpp \
    logindialog.cpp \
    chatlistwidget.cpp \
    mmdesktopapp.cpp \
    agentinfo.cpp \
    sessionhandle.cpp

HEADERS  += \
    globalxagentocx.h \
    mmdbasewidget.h \
    globalvar.h \
    mainwidget.h \
    logindialog.h \
    chatlistwidget.h \
    mmdesktopapp.h \
    agentinfo.h \
    sessionhandle.h

FORMS    += \
    logindialog.ui \
    mainwidget.ui

QT += axcontainer quickwidgets


RESOURCES += \
    resource.qrc
