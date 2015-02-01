#include <QApplication>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QString>
#include <QUrl>

#include "sysfuncs.h"

static const QString STARTUP_FILE = "RandomBackground.desktop";
static const QString STARTUP_DIR = ".config/autostart";
static const char* STARTUP_SCRIPT_BEGIN = "\n[Desktop Entry]\nType=Application\nExec=";
static const char* STARTUP_SCRIPT_END = "\nHidden=false\nNoDisplay=false\nX-GNOME-Autostart-enabled=true\n"
        "Name[en_GB]=RandomBackground\nName=RandomBackground\nComment[en_GB]=\nComment=";


bool sys::shellExecute(QString path, QString& err)
{
    path = QFileInfo(QDir::fromNativeSeparators(path)).absoluteFilePath();
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
        {
            QFile f(path);
            if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::warning(0, "Desktop Background Randomiser", "Unable to open startup file for writing.");
                return;
            }

            f.write(STARTUP_SCRIPT_BEGIN);
            f.write(qPrintable(QApplication::applicationDirPath() + "/RandomBackground"));
            f.write(STARTUP_SCRIPT_END);

            f.close();
        }
    }
    else
    {
        if (QFile::exists(path))
            QFile::remove(path);
    }
}
