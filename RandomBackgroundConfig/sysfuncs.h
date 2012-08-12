#ifndef SYSFUNCS_H
#define SYSFUNCS_H

class QString;

namespace sys
{
	bool shellExecute(QString path, QString parameters=QString(), QString workingpath=QString(), QString verb=QString());
	QString getErrMessage();
    bool getRunAtStartup();
    void setRunAtStartup(bool b);
}

#endif // SYSFUNCS_H
