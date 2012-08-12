#include <QDesktopServices>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QUrl>

#include "sysfuncs.h"


bool sys::shellExecute(QString path, QString& err)
{
    path = QFileInfo(path).absoluteFilePath();
    if (!QFile::exists(path))
    {
        err = "The specified file does not exist.";
        return false;
    }

    return QDesktopServices::openUrl(QUrl("file://" + path));
}

bool sys::getRunAtStartup()
{
    return false;
}

void sys::setRunAtStartup(bool b)
{

}

