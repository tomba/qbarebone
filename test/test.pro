QT += core gui platformsupport-private widgets

CONFIG += c++11

TARGET = test
TEMPLATE = app

QMAKE_CXXFLAGS += -Wno-unused-parameter

release: DESTDIR = build/release
debug:   DESTDIR = build/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

INCLUDEPATH += ../barebone

KMSXX = /home/tomba/work/kmsxx
INCLUDEPATH += $$KMSXX/kms++/inc
INCLUDEPATH += $$KMSXX/kms++util/inc
LIBPATH     += $$KMSXX/build/lib
LIBS        += -lkms++ -lkms++util -ldrm

SOURCES += main.cpp\
        mainwindow.cpp \
    qbareclient.cpp \
    openglwindow.cpp \
    trianglewindow.cpp \
    rasterwindow.cpp \
    moveblocks.cpp

HEADERS  += mainwindow.h \
    qbareclient.h \
    openglwindow.h \
    trianglewindow.h \
    rasterwindow.h

FORMS    += mainwindow.ui
