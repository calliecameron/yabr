TARGET = yabr-config
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

win32:RC_FILE = yabr_config_resource.rc

HEADERS += \
    DlgConfig.h \
    sysfuncs.h \
    DlgList.h \
    DlgRecent.h \
    DlgProfile.h

SOURCES += \
    yabr_config_main.cpp \
    DlgConfig.cpp \
    DlgList.cpp \
    DlgRecent.cpp \
    DlgProfile.cpp

win32 {
    SOURCES += sysfuncs_win32.cpp
}
unix {
    SOURCES += sysfuncs_linux.cpp
}

RESOURCES += \
    yabr-config.qrc
