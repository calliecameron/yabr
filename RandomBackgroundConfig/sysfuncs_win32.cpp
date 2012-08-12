#include <QDir>
#include <QString>
#include <windows.h>

#include "sysfuncs.h"


// Launch the specified file with the specified parameters and working directory
bool sys::shellExecute(QString path, QString parameters, QString workingpath, QString verb)
{
	return (int)ShellExecute(0, (WCHAR*)verb.constData(), (WCHAR*)QDir::toNativeSeparators(path).constData(), (WCHAR*)parameters.constData(), (WCHAR*)QDir::toNativeSeparators(workingpath).constData(), SW_SHOWNORMAL) > 32;
}

// Return the last Windows error message
QString sys::getErrMessage()
{
	QString tempstr = QString().fill(' ', 1000);

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, (WCHAR*)tempstr.constData(), 1000, 0);

	return tempstr.left(tempstr.indexOf(QChar(0))).trimmed();
}
