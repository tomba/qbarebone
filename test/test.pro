QT += core gui platformsupport-private widgets

CONFIG += c++11

TARGET = test
TEMPLATE = app

QMAKE_CXXFLAGS += -Wno-unused-parameter

DESTDIR = $$top_builddir/bin

BUILDDIR = $$top_builddir/test
OBJECTS_DIR = $$BUILDDIR/obj
MOC_DIR = $$BUILDDIR/moc
RCC_DIR = $$BUILDDIR/qrc
UI_DIR = $$BUILDDIR/ui

INCLUDEPATH += $$top_srcdir/barebone

KMSXX = $$top_srcdir/../kmsxx
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
    moveblocks.cpp \
    camerastreamer.cpp \
    kmsmanager.cpp

HEADERS  += mainwindow.h \
    qbareclient.h \
    openglwindow.h \
    trianglewindow.h \
    rasterwindow.h \
    camerastreamer.h \
    kmsmanager.h

FORMS    += mainwindow.ui
