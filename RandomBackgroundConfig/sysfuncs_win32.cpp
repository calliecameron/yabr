#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include <QString>
#include <windows.h>

#include "sysfuncs.h"

static const char* KEY_RUN = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
static const char* KEY_RANDBACK = "RandomBackground";

// Return the last Windows error message
static QString getErrMessage()
{
    QString tempstr = QString().fill(' ', 1000);

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, (WCHAR*)tempstr.constData(), 1000, 0);

    return tempstr.left(tempstr.indexOf(QChar(0))).trimmed();
}

// Launch the specified file with the specified parameters and working directory
bool sys::shellExecute(QString path, QString& err)
{
    bool success = (int)ShellExecute(0, (WCHAR*)QString().constData(), (WCHAR*)QDir::toNativeSeparators(path).constData(), (WCHAR*)QString().constData(), (WCHAR*)QDir::toNativeSeparators(QString()).constData(), SW_SHOWNORMAL) > 32;

    if (!success)
        err = getErrMessage();

    return success;
}


// Return the state (checked or unchecked) of the checkbox based on whether or not the program is set to run at startup in the registry
bool sys::getRunAtStartup()
{
    QSettings reg(KEY_RUN, QSettings::NativeFormat);

    if (reg.contains(KEY_RANDBACK))
    {
        QString temp = reg.value(KEY_RANDBACK).toString();

        if (QDir::fromNativeSeparators(temp).compare(qApp->applicationDirPath() + "/RandomBackground.exe", Qt::CaseInsensitive) != 0)
        {
            if (QMessageBox::warning(0, qApp->applicationName(), "A copy of Desktop Background Randomiser is enabled on this computer, but at a different location from this copy. Do you want to make this version run at startup?\n(If unsure, click No, but bear in mind that any changes made in this copy will not affect the enabled copy.)\nPath: " + temp, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            {
                setRunAtStartup(true);
                return true;
            }
            else
                return false;
        }
        else
            return true;
    }
    else
        return false;
}


// Write or delete the run key from the registry
void sys::setRunAtStartup(bool b)
{
    QSettings reg(KEY_RUN, QSettings::NativeFormat);

    if (b)
        reg.setValue(KEY_RANDBACK, QDir::toNativeSeparators(qApp->applicationDirPath() + "/RandomBackground.exe"));
    else
    {
        if (reg.contains(KEY_RANDBACK))
            reg.remove(KEY_RANDBACK);
    }
}
