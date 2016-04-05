QT       += core-private gui-private platformsupport-private

TARGET = barebone
TEMPLATE = lib
CONFIG += plugin

SOURCES += main.cpp qbareintegration.cpp qbarebackingstore.cpp qbarescreen.cpp \
    qbarewindow.cpp

HEADERS += qbareintegration.h qbarebackingstore.h qbarescreen.h \
    qbarewindow.h

OTHER_FILES += barebone.json
