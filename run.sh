#!/bin/sh

#export QT_DEBUG_PLUGINS=1

export QT_PLUGIN_PATH=./plugins

# hack to create platforms plugin dir
mkdir -p plugins/platforms
ln -fs ../../barebone/build/debug/libbarebone.so plugins/platforms/

./test/build/debug/test -platform barebone

