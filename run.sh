#!/bin/sh

#export QT_LOGGING_RULES="qt.qpa.input=true"

#export QT_DEBUG_PLUGINS=1

# XXX on Ubuntu with Gtk installed, failure to connect to X aborts the app.
# So add the DISPLAY here, even if we never use X
export DISPLAY=:0

export QT_PLUGIN_PATH=./build/plugins

# For qemu
#export QT_QPA_EVDEV_MOUSE_PARAMETERS="abs"

./build/bin/test -platform barebone
#gdb --args ./build/bin/test -platform barebone

