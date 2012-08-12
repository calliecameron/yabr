#include <QDir>
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

    return QDir::fromNativeSeparators(out);
}

bool lowlevel::setBackground(QString filename, QString& err)
{
    filename = QFileInfo(QDir::toNativeSeparators(filename)).absoluteFilePath();

    if (!QFile::exists(filename))
    {
        err = "The specified file does not exist";
        return false;
    }

    QProcess gsettings;
    gsettings.start("gsettings", QStringList() << "set" << "org.gnome.desktop.background" << "picture-uri"
                    << ("file://" + filename));

    if (!gsettings.waitForStarted())
        return false;

    if (!gsettings.waitForFinished())
        return false;

    return true;
}

void lowlevel::setResizeMode(RandomBackgroundLib::ResizeMode mode)
{
    QString s;

    if (mode == RandomBackgroundLib::SMART_RESIZE)
        s = "zoom";
    else if (mode == RandomBackgroundLib::STRETCH)
        s = "stretched";
    else if (mode == RandomBackgroundLib::CENTRE)
        s = "centered";
    else if (mode == RandomBackgroundLib::TILE)
        s = "wallpaper";
    else
        return;

    QProcess gsettings;
    gsettings.start("gsettings", QStringList() << "set" << "org.gnome.desktop.background" << "picture-options" << s);

    if (!gsettings.waitForStarted())
        return;

    gsettings.waitForFinished();
}

bool lowlevel::convertJpegsToBmp()
{
    return false;
}
