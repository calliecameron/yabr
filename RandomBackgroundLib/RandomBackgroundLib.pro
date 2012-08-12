#-------------------------------------------------
#
# Project created by QtCreator 2011-06-04T13:08:37
#
#-------------------------------------------------

CONFIG(debug, debug|release) {
    TARGET = RandomBackgroundLibD
#    win32 {
#        QMAKE_POST_LINK = copy /Y debug\\*.dll ..\\RandomBackground\\debug &
#        QMAKE_POST_LINK += copy /Y debug\\*.dll ..\\RandomBackgroundConfig\\debug
#    }
}
CONFIG(release, debug|release) {
    TARGET = RandomBackgroundLib
#    win32 {
#        QMAKE_POST_LINK = copy /Y release\\*.dll ..\\RandomBackground\\release &
#        QMAKE_POST_LINK += copy /Y release\\*.dll ..\\RandomBackgroundConfig\\release
#    }
}

TEMPLATE = lib
CONFIG += staticlib
#win32:RC_FILE = RandomBackgroundLib_Resource.rc

DEFINES += RANDOMBACKGROUNDLIB_LIBRARY

SOURCES += RandomBackgroundLib.cpp \
    RandBackImpl.cpp \
    Log.cpp \
    FolderListImpl.cpp

HEADERS += RandomBackgroundLib.h\
        RandomBackgroundLib_global.h \
    RandBackImpl.h \
    Log.h \
    FolderList.h \
    FolderListImpl.h
