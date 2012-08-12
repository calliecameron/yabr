
TARGET = RandomBackgroundConfig
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

win32:RC_FILE = RandomBackgroundConfig_Resource.rc

HEADERS += \
    DlgConfig.h \
    sysfuncs.h \
    DlgList.h \
    DlgRecent.h \
    DlgProfile.h

SOURCES += \
    randbackconfig_main.cpp \
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
    RandomBackgroundConfig.qrc
