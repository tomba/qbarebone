#-------------------------------------------------
#
# Project created by QtCreator 2016-04-05T09:27:35
#
#-------------------------------------------------

QT       += core gui platformsupport-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = test
TEMPLATE = app

INCLUDEPATH += ../barebone

INCLUDEPATH += /home/tomba/work/kmsxx/libkms++
LIBPATH     += /home/tomba/work/kmsxx/build/libkms++
LIBS        += -lkms++ -ldrm

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
