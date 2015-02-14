#include <QApplication>
#include <QDir>
#include "RandomBackgroundLib.h"
#include "RandBackImpl.h"


RandomBackgroundLib* RandomBackgroundLib::s_Instance = NULL;


RandomBackgroundLib* RandomBackgroundLib::init(WarnFunc warnFunc, ChoiceFunc choiceFunc)
{
    if (!s_Instance)
    {
#ifdef Q_OS_WIN
        QString settingsDirPrefix = qgetenv("APPDATA") + "/YABR/";
#endif
#ifdef Q_OS_LINUX
        QString settingsDirPrefix = QDir::homePath() + "/.yabr/";
#endif

        if (!QDir().mkpath(settingsDirPrefix))
        {
            warnFunc("Unable to create the application's settings directory: '" + settingsDirPrefix + "'.");
            return NULL;
        }

        s_Instance = new RandBackImpl(
                    settingsDirPrefix + "Background.ini",
                    settingsDirPrefix + "Folders.txt",
                    settingsDirPrefix + "Excluded.txt",
                    settingsDirPrefix + "Blacklist.txt",
                    settingsDirPrefix + "RecentImages.txt",
                    settingsDirPrefix + "BackgroundResized.bmp",
                    settingsDirPrefix,
                    settingsDirPrefix + "RandomBackground.log",
                    warnFunc,
                    choiceFunc);
    }

    return s_Instance;
}

RandomBackgroundLib* RandomBackgroundLib::instance()
{
    return s_Instance;
}
