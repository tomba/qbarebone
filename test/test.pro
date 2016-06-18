QT       += core gui platformsupport-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = test
TEMPLATE = app

release: DESTDIR = build/release
debug:   DESTDIR = build/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

INCLUDEPATH += ../barebone

INCLUDEPATH += /home/tomba/work-lappy/kmsxx/kms++/inc
LIBPATH     += /home/tomba/work-lappy/kmsxx/build/lib
LIBS        += -lkms++ -ldrm

SOURCES += main.cpp\
        mainwindow.cpp \
    qbareclient.cpp \
    openglwindow.cpp \
    trianglewindow.cpp \
    rasterwindow.cpp

HEADERS  += mainwindow.h \
    qbareclient.h \
    openglwindow.h \
    trianglewindow.h \
    rasterwindow.h

FORMS    += mainwindow.ui
