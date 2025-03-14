#-------------------------------------------------
#
# Project created by QtCreator 2025-02-21T17:42:11
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = camera
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    camerathread.cpp \
    udp_listener_thread.cpp \
    camerasoundplayer.cpp \
    camerasoundthread.cpp \
    bgmsoundplayer.cpp \
    bgmsoundthread.cpp

HEADERS  += mainwindow.h \
    camerathread.h \
    udp_listener_thread.h \
    camerasoundplayer.h \
    camerasoundthread.h \
    bgmsoundplayer.h \
    bgmsoundthread.h

FORMS    += mainwindow.ui
