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
    secondwindow.cpp \
    imagedecoration.cpp \
    imagefilter.cpp \
    udp_sender_thread.cpp \
    imagefilter2.cpp \
    imagedecoration2.cpp \
    imageprocessworker.cpp \
    bgmsoundthread2.cpp \
    bgmsoundplayer2.cpp
    imageprocessworker.cpp

HEADERS  += \
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
    secondwindow.h \
    imagedecoration.h \
    imagefilter.h \
    udp_sender_thread.h \
    imagefilter2.h \
    imagedecoration2.h \
    imageprocessworker.h \
    bgmsoundthread2.h \
    bgmsoundplayer2.h
    imageprocessworker.h

FORMS    += \
    endingwindow.ui \
    firstwindow.ui \
    guideselectwindow.ui \
    photowindow.ui \
    secondwindow.ui \
    imagefilter2.ui \
    imagedecoration2.ui

RESOURCES += \
    ../../../../../../nfsroot/images/images.qrc
