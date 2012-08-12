
#include <QDir>
#include <QSettings>
#include <QString>
#include <windows.h>

#include "lowlevel.h"

static const char* KEY_TILE_WALLPAPER = "TileWallpaper";
static const char* KEY_WALLPAPER_STYLE = "WallpaperStyle";

static QString getErrorMsg()
{
    QString str;
    str.fill(' ', 1000);

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, (WCHAR*)str.constData(), 1000, 0);

    return str.left(str.indexOf(QChar(0))).trimmed();
}

QString lowlevel::getBackground()
{
    QString str;
    str.fill(' ', MAX_PATH);

    if (!SystemParametersInfo(SPI_GETDESKWALLPAPER, MAX_PATH, (WCHAR*)str.constData(), 0))
        return QString();
    else
        return QDir::fromNativeSeparators(str.left(str.indexOf(QChar(0))));
}

bool lowlevel::setBackground(QString filename, QString& err)
{
    bool success = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (WCHAR*)QDir::toNativeSeparators(filename).constData(), SPIF_UPDATEINIFILE);

    if (!success)
        err = getErrorMsg();

    return success;
}

void lowlevel::setResizeMode(RandomBackgroundLib::ResizeMode mode)
{
    QSettings reg("HKEY_CURRENT_USER\\Control Panel\\Desktop", QSettings::NativeFormat);

    if (mode == RandomBackgroundLib::SMART_RESIZE || mode == RandomBackgroundLib::CENTRE)
    {
        reg.setValue(KEY_TILE_WALLPAPER, "0");
        reg.setValue(KEY_WALLPAPER_STYLE, "0");
    }
    else if (mode == RandomBackgroundLib::STRETCH)
    {
        reg.setValue(KEY_TILE_WALLPAPER, "0");
        reg.setValue(KEY_WALLPAPER_STYLE, "2");
    }
    else if (mode == RandomBackgroundLib::TILE)
    {
        reg.setValue(KEY_TILE_WALLPAPER, "1");
        reg.setValue(KEY_WALLPAPER_STYLE, "0");
    }
}

bool lowlevel::convertJpegsToBmp()
{
    return QSysInfo::windowsVersion() < QSysInfo::WV_VISTA;
}
