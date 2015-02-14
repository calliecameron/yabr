TARGET = yabr
TEMPLATE = app
QT += widgets
DESTDIR = ../bin

INCLUDEPATH += $$quote(../yabr-lib)


CONFIG(debug, debug|release) {
    win32 {
        LIBS += $$quote(../yabr-lib/debug/libyabr-libD.a)
    }
    unix {
        LIBS += $$quote(../yabr-lib/libyabr-libD.a)
    }
}
CONFIG(release, debug|release) {
    win32 {
        LIBS += $$quote(../yabr-lib/release/libyabr-lib.a)
    }
    unix {
        LIBS += $$quote(../yabr-lib/libyabr-lib.a)
    }
}

win32:RC_FILE = yabr_resource.rc

SOURCES += \
    yabr_main.cpp
