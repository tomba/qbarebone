QT += core-private gui-private platformsupport-private

CONFIG += c++11

TARGET = barebone
TEMPLATE = lib
CONFIG += plugin

QMAKE_CXXFLAGS += -Wno-unused-parameter

DEFINES += Q_FONTCONFIGDATABASE
LIBS += -lfontconfig

DESTDIR = $$top_builddir/plugins/platforms/

BUILDDIR = $$top_builddir/barebone
OBJECTS_DIR = $$BUILDDIR/obj
MOC_DIR = $$BUILDDIR/moc
RCC_DIR = $$BUILDDIR/qrc
UI_DIR = $$BUILDDIR/ui

SOURCES += main.cpp qbareintegration.cpp qbarebackingstore.cpp qbarescreen.cpp \
    qbarewindow.cpp \
    qbarenativeinterface.cpp \
    qbarecursor.cpp

HEADERS += qbareintegration.h qbarebackingstore.h qbarescreen.h \
    qbarewindow.h \
    qbarenativeinterface.h \
    qbareinterface.h \
    qbarecursor.h

OTHER_FILES += barebone.json
