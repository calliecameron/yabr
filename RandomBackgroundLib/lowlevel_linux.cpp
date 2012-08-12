#include <QFileInfo>
#include <QProcess>

#include "lowlevel.h"

QString lowlevel::getBackground()
{
    QProcess gsettings;
    gsettings.start("gsettings", QStringList() << "get" << "org.gnome.desktop.background" << "picture-uri");

    if (!gsettings.waitForStarted())
        return QString();

    if (!gsettings.waitForFinished())
        return QString();

    QString out = gsettings.readAll();
    out = out.trimmed();

    if (out.startsWith(QChar('\'')))
        out.remove(0, 1);

    if (out.endsWith(QChar('\'')))
        out.chop(1);

    if (out.startsWith("file://"))
        out.remove(0, 7);

    return out;
}

bool lowlevel::setBackground(QString filename)
{
    QProcess gsettings;
    gsettings.start("gsettings", QStringList() << "set" << "org.gnome.desktop.background" << "picture-uri"
                    << ("file://" + QFileInfo(filename).absoluteFilePath()));

    if (!gsettings.waitForStarted())
        return false;

    if (!gsettings.waitForFinished())
        return false;

    return true;
}

void lowlevel::setResizeMode(RandomBackgroundLib::ResizeMode mode)
{

}

bool lowlevel::convertJpegsToBmp()
{
    return false;
}

QString lowlevel::getErrorMsg()
{
    return QString();
}


