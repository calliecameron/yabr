#ifndef SYSFUNCS_H
#define SYSFUNCS_H

class QString;

namespace sys
{
    bool shellExecute(QString path, QString& err);
    bool getRunAtStartup();
    void setRunAtStartup(bool b);
}

#endif // SYSFUNCS_H
