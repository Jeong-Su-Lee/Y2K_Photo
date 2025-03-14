#-------------------------------------------------
#
# Project created by QtCreator 2025-03-12T10:39:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UI
TEMPLATE = app


SOURCES += main.cpp\
    secondwindow.cpp \
    firstwindow.cpp \
    guideselectwindow.cpp \
    endingwindow.cpp \
    photowindow.cpp

HEADERS  += \
    secondwindow.h \
    firstwindow.h \
    guideselectwindow.h \
    endingwindow.h \
    photowindow.h

FORMS    += \
    secondwindow.ui \
    firstwindow.ui \
    guideselectwindow.ui \
    endingwindow.ui \
    photowindow.ui

RESOURCES += \
    ../../../../../nfsroot/images/images.qrc
