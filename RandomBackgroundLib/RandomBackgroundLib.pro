#-------------------------------------------------
#
# Project created by QtCreator 2011-06-04T13:08:37
#
#-------------------------------------------------

CONFIG(debug, debug|release) {
    TARGET = RandomBackgroundLibD
}
CONFIG(release, debug|release) {
    TARGET = RandomBackgroundLib
}

TEMPLATE = lib
CONFIG += staticlib
QT += widgets

SOURCES += RandomBackgroundLib.cpp \
    RandBackImpl.cpp \
    Log.cpp \
    FolderListImpl.cpp

win32 {
    SOURCES += lowlevel_win32.cpp
}
unix {
    SOURCES += lowlevel_linux.cpp
}

HEADERS += RandomBackgroundLib.h\
    RandBackImpl.h \
    Log.h \
    FolderList.h \
    FolderListImpl.h \
    lowlevel.h
