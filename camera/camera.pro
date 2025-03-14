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
    bgmsoundthread.cpp \
    endingwindow.cpp \
    firstwindow.cpp \
    guideselectwindow.cpp \
    photowindow.cpp \
    secondwindow.cpp

HEADERS  += mainwindow.h \
    camerathread.h \
    udp_listener_thread.h \
    camerasoundplayer.h \
    camerasoundthread.h \
    bgmsoundplayer.h \
    bgmsoundthread.h \
    endingwindow.h \
    firstwindow.h \
    guideselectwindow.h \
    photowindow.h \
    secondwindow.h

FORMS    += mainwindow.ui \
    endingwindow.ui \
    firstwindow.ui \
    guideselectwindow.ui \
    photowindow.ui \
    secondwindow.ui

RESOURCES += \
    ../../../../../../nfsroot/images/images.qrc
