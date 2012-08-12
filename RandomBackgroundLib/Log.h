#ifndef LOG_H
#define LOG_H

#include <QTextStream>

class QFile;

#define LOG Log::instance()


class Log
{
public:

	static Log& instance();
	static void setFilename(QString filenmame);

	Log& operator<<(const QString& s);
	Log& operator<<(int i);

	Log& timestamp();
	Log& flush();

	~Log();

private:

	Log(QString filename);

	static QString s_Filename;

	QFile* m_File;
	QTextStream m_Stream;

};

#endif // LOG_H
