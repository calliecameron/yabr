#ifndef SYSFUNCS_H
#define SYSFUNCS_H

class QString;

namespace sys
{
	bool shellExecute(QString path, QString parameters=QString(), QString workingpath=QString(), QString verb=QString());

	QString getErrMessage();
}

#endif // SYSFUNCS_H
