
TARGET = RandomBackground
TEMPLATE = app

INCLUDEPATH += $$quote(../RandomBackgroundLib)


CONFIG(debug, debug|release) {
    win32 {
        LIBS += $$quote(../RandomBackgroundLib/debug/libRandomBackgroundLibD.a)
    }
    unix {
        LIBS += $$quote(../RandomBackgroundLib/libRandomBackgroundLibD.a)
    }
}
CONFIG(release, debug|release) {
    win32 {
        LIBS += $$quote(../RandomBackgroundLib/release/libRandomBackgroundLib.a)
    }
    unix {
        LIBS += $$quote(../RandomBackgroundLib/libRandomBackgroundLib.a)
    }
}

win32:RC_FILE = RandomBackground_Resource.rc

SOURCES += \
    randback_main.cpp
