#include "RandomBackgroundLib.h"
#include "RandBackImpl.h"


RandomBackgroundLib* RandomBackgroundLib::s_Instance = NULL;


RandomBackgroundLib* RandomBackgroundLib::init(QString settings, QString folders, QString excluded, QString blacklist, QString recentImages, QString resizeFile, QString thumbnailPath, QString logFile, WarnFunc warnFunc, ChoiceFunc choiceFunc)
{
    if (!s_Instance)
        s_Instance = new RandBackImpl(settings, folders, excluded, blacklist, recentImages, resizeFile, thumbnailPath, logFile, warnFunc, choiceFunc);

    return s_Instance;
}

RandomBackgroundLib* RandomBackgroundLib::instance()
{
    return s_Instance;
}
