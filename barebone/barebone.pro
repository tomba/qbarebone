QT += core-private gui-private platformsupport-private

TARGET = barebone
TEMPLATE = lib
CONFIG += plugin

QMAKE_CXXFLAGS += -Wno-unused-parameter

DEFINES += Q_FONTCONFIGDATABASE
LIBS += -lfontconfig

release: DESTDIR = build/release
debug:   DESTDIR = build/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

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
