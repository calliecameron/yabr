#include <QApplication>
#include <QMessageBox>
#include <RandomBackgroundLib.h>



void warnCallback(QString str);
bool choiceCallback(QString str);

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

#ifdef Q_WS_WIN
	RandomBackgroundLib* lib = RandomBackgroundLib::init(
			qApp->applicationDirPath() + "/Background.ini",
			qApp->applicationDirPath() + "/Folders.txt",
			qApp->applicationDirPath() + "/Excluded.txt",
			qApp->applicationDirPath() + "/Blacklist.txt",
			qApp->applicationDirPath() + "/RecentImages.txt",
			qApp->applicationDirPath() + "/BackgroundResized.bmp",
			qApp->applicationDirPath(),
			qApp->applicationDirPath() + "/RandomBackground.log",
			&warnCallback,
			&choiceCallback);
#endif

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
		QMessageBox::critical(0, "Desktop Background Randomiser", "Unable to change the desktop background. " + result.message + ".");
	else if (result.code == RandomBackgroundLib::PARTIAL_SUCCESS)
		QMessageBox::warning(0, "Desktop Background Randomiser", result.message);

	delete lib;

	return 0;
}

void warnCallback(QString str)
{
	QMessageBox::warning(0, "Desktop Background Randomiser", str);
}

bool choiceCallback(QString str)
{
	return QMessageBox::warning(0, "Desktop Background Randomiser", str, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes;
}
