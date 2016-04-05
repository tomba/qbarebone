QT       += core-private gui-private platformsupport-private

TARGET = barebone
TEMPLATE = lib
CONFIG += plugin

SOURCES += main.cpp qminimalintegration.cpp qminimalbackingstore.cpp

HEADERS += qminimalintegration.h qminimalbackingstore.h

OTHER_FILES += barebone.json
