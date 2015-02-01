#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <RandomBackgroundLib.h>



void warnCallback(QString str);
bool choiceCallback(QString str);

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
    QApplication::setApplicationName("Desktop Background Randomiser");

#ifdef Q_OS_WIN
    QString settingsDirPrefix = QApplication::applicationDirPath() + "/";
#endif
#ifdef Q_OS_LINUX
    QString settingsDirPrefix = QDir::homePath() + "/.randomBackground/";
    if (!QDir().mkpath(settingsDirPrefix))
    {
        QMessageBox::critical(0, QApplication::applicationName(), "Unable to create the application's settings directory: '" + settingsDirPrefix + "'.");
        return 1;
    }
#endif

    RandomBackgroundLib* lib = RandomBackgroundLib::init(
            settingsDirPrefix + "Background.ini",
            settingsDirPrefix + "Folders.txt",
            settingsDirPrefix + "Excluded.txt",
            settingsDirPrefix + "Blacklist.txt",
            settingsDirPrefix + "RecentImages.txt",
            settingsDirPrefix + "BackgroundResized.bmp",
            settingsDirPrefix,
            settingsDirPrefix + "RandomBackground.log",
            &warnCallback,
            &choiceCallback);


	// Check command line - if the first parameter is -o (for 'override') change image randomly regardless
	// of ChangeIn, or if it is a valid filename, set background to that image
	RandomBackgroundLib::Result result(RandomBackgroundLib::SUCCESS);

	if (argc > 1)
	{
		QString cmd(argv[1]);

		if (cmd.toLower() == "-o")
			result = lib->changeBackground(true);
		else
			result = lib->setBackground(cmd);
	}
	else
		result = lib->changeBackground();

	if (result.code == RandomBackgroundLib::FAILURE)
        QMessageBox::critical(0, QApplication::applicationName(), "Unable to change the desktop background. " + result.message + ".");
	else if (result.code == RandomBackgroundLib::PARTIAL_SUCCESS)
        QMessageBox::warning(0, QApplication::applicationName(), result.message);

	delete lib;

	return 0;
}

void warnCallback(QString str)
{
    QMessageBox::warning(0, QApplication::applicationName(), str);
}

bool choiceCallback(QString str)
{
    return QMessageBox::warning(0, QApplication::applicationName(), str, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes;
}
