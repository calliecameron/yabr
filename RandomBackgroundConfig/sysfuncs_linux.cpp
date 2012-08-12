#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QUrl>

#include "sysfuncs.h"

static const QString STARTUP_FILE = "RandomBackground.desktop";
static const QString STARTUP_DIR = ".config/autostart";

bool sys::shellExecute(QString path, QString& err)
{
    path = QFileInfo(QDir::toNativeSeparators(path)).absoluteFilePath();
    if (!QFile::exists(path))
    {
        err = "The specified file does not exist.";
        return false;
    }

    return QDesktopServices::openUrl(QUrl("file://" + path));
}

bool sys::getRunAtStartup()
{
    return QFile::exists(QDir::homePath() + "/" + STARTUP_DIR + "/" + STARTUP_FILE);
}

void sys::setRunAtStartup(bool b)
{
    QString path = QDir::homePath() + "/" + STARTUP_DIR + "/" + STARTUP_FILE;

    if (b)
    {
        if (!QFile::exists(path))
            QFile::copy(":/" + STARTUP_FILE, path);
    }
    else
    {
        if (QFile::exists(path))
            QFile::remove(path);
    }
}
