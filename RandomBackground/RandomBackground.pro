
TARGET = RandomBackground
TEMPLATE = app

INCLUDEPATH += $$quote(../RandomBackgroundLib)


CONFIG(debug, debug|release) {
	LIBS += $$quote(../RandomBackgroundLib/debug/libRandomBackgroundLibD.a)
}
CONFIG(release, debug|release) {
	LIBS += $$quote(../RandomBackgroundLib/release/libRandomBackgroundLib.a)
}

win32:RC_FILE = RandomBackground_Resource.rc

SOURCES += \
    randback_main.cpp
