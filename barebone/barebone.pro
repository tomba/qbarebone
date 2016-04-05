QT       += core-private gui-private platformsupport-private

TARGET = barebone
TEMPLATE = lib
CONFIG += plugin

SOURCES += main.cpp qbareintegration.cpp qbarebackingstore.cpp qbarescreen.cpp \
    qbarewindow.cpp \
    qbarenativeinterface.cpp

HEADERS += qbareintegration.h qbarebackingstore.h qbarescreen.h \
    qbarewindow.h \
    qbarenativeinterface.h \
    qbareinterface.h \
    qbareclientinterface.h

OTHER_FILES += barebone.json
