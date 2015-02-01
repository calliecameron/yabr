#include <QDateTime>
#include <QFile>

#include "Log.h"


QString Log::s_Filename = "errorlog.txt";


Log& Log::instance()
{
    static Log inst(s_Filename);
    return inst;
}

void Log::setFilename(QString filename)
{
    s_Filename = filename;
}

Log::Log(QString filename)
{
    m_File = new QFile(filename);
    m_File->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    m_Stream.setDevice(m_File);
}

Log::~Log()
{
    m_File->close();
    delete m_File;
}

Log& Log::operator<<(const QString& s)
{
    m_Stream << s;
    return *this;
}

Log& Log::operator<<(int i)
{
    m_Stream << i;
    return *this;
}

Log& Log::timestamp()
{
    m_Stream << "[" << QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") << "]: ";
    return *this;
}

Log& Log::flush()
{
    m_Stream.flush();
    m_File->flush();
    return *this;
}
