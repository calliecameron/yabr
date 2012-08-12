#include <ctime>
#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QTextStream>
#include <QTime>

#include "FolderListImpl.h"
#include "Log.h"
#include "lowlevel.h"
#include "RandBackImpl.h"


const QString RandBackImpl::KEY_CURR_BACK = "CurrentBackground";
const QString RandBackImpl::KEY_RESIZE_MODE = "ResizeMode";
const QString RandBackImpl::KEY_CHANGE_EVERY = "ChangeEvery";
const QString RandBackImpl::KEY_CHANGE_IN = "ChangeIn";
const QString RandBackImpl::KEY_RECENT_STORE_NUM = "RecentStoreNum";
const QString RandBackImpl::KEY_REPEAT_LIMIT = "RepeatLimit";
const QString RandBackImpl::KEY_THUMB_WIDTH = "ThumbnailWidth";
const QString RandBackImpl::KEY_THUMB_HEIGHT = "ThumbnailHeight";
const QString RandBackImpl::KEY_DIAGNOSTICS = "Diagnostics";



RandBackImpl::RandBackImpl(QString settings, QString folders, QString excluded, QString blacklist, QString recentImages, QString resizeFile, QString thumbnailPath, QString logFile, WarnFunc warnFunc, ChoiceFunc choiceFunc)
	: RandomBackgroundLib(), m_FoldersFile(QDir::fromNativeSeparators(folders)), m_ExcludedFile(QDir::fromNativeSeparators(excluded)),
	m_BlacklistFile(QDir::fromNativeSeparators(blacklist)), m_RecentImagesFile(QDir::fromNativeSeparators(recentImages)),
	m_ResizeFile(QDir::fromNativeSeparators(resizeFile)), m_ThumbnailPath(QDir::fromNativeSeparators(thumbnailPath))
{
	f_Warning = warnFunc;
	f_UserChoice = choiceFunc;

	Log::setFilename(QDir::fromNativeSeparators(logFile));

	loadSettingsFile(QDir::fromNativeSeparators(settings));

	m_Folders = FolderListImpl::load(m_FoldersFile, f_Warning);
	loadPathList(m_ExcludedFile, m_Excluded);
	loadPathList(m_BlacklistFile, m_Blacklist);

	if (!loadRecentList(m_RecentImagesFile, m_RecentImages, m_RecentThumbnails, getRecentStoreNum()) && f_Warning)
		(*f_Warning)("Unable to load the recent images list.");

	if (!m_ThumbnailPath.endsWith('/'))
		m_ThumbnailPath.append('/');
}

RandBackImpl::~RandBackImpl()
{
	delete m_Settings;
	delete m_Folders;
}


RandomBackgroundLib::ResizeMode RandBackImpl::getResizeMode() const
{
	return (ResizeMode)m_Settings->value(KEY_RESIZE_MODE).toInt();
}

void RandBackImpl::setResizeMode(ResizeMode mode)
{
	if (mode < 0 || mode >= RandomBackgroundLib::DUMMY_INVALID)
		mode = RandomBackgroundLib::SMART_RESIZE;

	m_Settings->setValue(KEY_RESIZE_MODE, mode);
}

int RandBackImpl::getChangeEvery() const
{
	return m_Settings->value(KEY_CHANGE_EVERY).toInt();
}

void RandBackImpl::setChangeEvery(int value)
{
	if (value < 1)
		value = 1;

	m_Settings->setValue(KEY_CHANGE_EVERY, value);
}

int RandBackImpl::getChangeIn() const
{
	return m_Settings->value(KEY_CHANGE_IN).toInt();
}

void RandBackImpl::setChangeIn(int value)
{
	m_Settings->setValue(KEY_CHANGE_IN, value);
}

int RandBackImpl::getRecentStoreNum() const
{
	return m_Settings->value(KEY_RECENT_STORE_NUM).toInt();
}

void RandBackImpl::setRecentStoreNum(int value)
{
	if (value < 1)
		value = 20;

	m_Settings->setValue(KEY_RECENT_STORE_NUM, value);

	if (value < m_RecentImages.count())
	{
		m_RecentImages = m_RecentImages.mid(0, value);
		m_RecentThumbnails = m_RecentThumbnails.mid(0, value);

		if (!saveRecentList(m_RecentImagesFile, m_RecentImages, m_RecentThumbnails) && f_Warning)
			(*f_Warning)("Failed to save the recent images list.");
	}
}

int RandBackImpl::getRepeatLimit() const
{
	return m_Settings->value(KEY_REPEAT_LIMIT).toInt();
}

void RandBackImpl::setRepeatLimit(int value)
{
	if (value < 1 || value > m_Settings->value(KEY_RECENT_STORE_NUM).toInt())
		value = m_Settings->value(KEY_RECENT_STORE_NUM).toInt();

	m_Settings->setValue(KEY_REPEAT_LIMIT, value);
}

int RandBackImpl::getThumbnailWidth() const
{
	return m_Settings->value(KEY_THUMB_WIDTH).toInt();
}

void RandBackImpl::setThumbnailWidth(int value)
{
	if (value < 1)
		value = 100;

	m_Settings->setValue(KEY_THUMB_WIDTH, value);
}

int RandBackImpl::getThumbnailHeight() const
{
	return m_Settings->value(KEY_THUMB_HEIGHT).toInt();
}

void RandBackImpl::setThumbnailHeight(int value)
{
	if (value < 1)
		value = 75;

	m_Settings->setValue(KEY_THUMB_HEIGHT, value);
}

bool RandBackImpl::getDiagnosticsOn() const
{
	return m_Settings->value(KEY_DIAGNOSTICS).toBool();
}

void RandBackImpl::setDiagnosticsOn(bool value)
{
	m_Settings->setValue(KEY_DIAGNOSTICS, value);
}

QString RandBackImpl::getSettingsFile() const
{
	return m_Settings->fileName();
}

QString RandBackImpl::getSearchFoldersFile() const
{
	return m_FoldersFile;
}

QString RandBackImpl::getExcludedFoldersFile() const
{
	return m_ExcludedFile;
}

QString RandBackImpl::getBlacklistFile() const
{
	return m_BlacklistFile;
}

QString RandBackImpl::getRecentImagesFile() const
{
	return m_RecentImagesFile;
}

QString RandBackImpl::getResizeFile() const
{
	return m_ResizeFile;
}

FolderList* RandBackImpl::getSearchFolders() const
{
	return m_Folders->duplicate();
}

bool RandBackImpl::setSearchFolders(FolderList* folders)
{
	delete m_Folders;
	m_Folders = (FolderListImpl*)(folders->duplicate());
	return m_Folders->save(m_FoldersFile);
}

QStringList RandBackImpl::getExcludedFolders() const
{
	return m_Excluded;
}

bool RandBackImpl::setExcludedFolders(QStringList list)
{
	return setPathList(list, m_ExcludedFile, m_Excluded);
}

bool RandBackImpl::addExcludedFolder(QString folder)
{
	return addToPathList(folder, m_ExcludedFile, m_Excluded);
}

bool RandBackImpl::removeExcludedFolder(QString folder)
{
	return removeFromPathList(folder, m_ExcludedFile, m_Excluded);
}

bool RandBackImpl::clearExcludedFolders()
{
	return clearPathList(m_ExcludedFile, m_Excluded);
}

QStringList RandBackImpl::getBlacklist() const
{
	return m_Blacklist;
}

bool RandBackImpl::setBlacklist(QStringList list)
{
	return setPathList(list, m_BlacklistFile, m_Blacklist);
}

bool RandBackImpl::addToBlacklist(QString file)
{
	return addToPathList(file, m_BlacklistFile, m_Blacklist);
}

bool RandBackImpl::removeFromBlacklist(QString file)
{
	return removeFromPathList(file, m_BlacklistFile, m_Blacklist);
}

bool RandBackImpl::clearBlacklist()
{
	return clearPathList(m_BlacklistFile, m_Blacklist);
}

QStringList RandBackImpl::getRecentImages() const
{
	return m_RecentImages;
}

QStringList RandBackImpl::getRecentImageThumbnails() const
{
	QStringList list;
	int num;

	QString str = m_ThumbnailPath + "Thumbnail_";

	for (int i = 0; i < m_RecentThumbnails.count(); ++i)
	{
		num = m_RecentThumbnails[i];

		if (num >= 0)
			list << str + QString::number(num) + ".jpg";
		else
			list << QString();
	}

	return list;
}


QString RandBackImpl::getBackground() const
{
    QString str = lowlevel::getBackground();

	if (str.compare(m_ResizeFile, Qt::CaseInsensitive) == 0)
		return m_Settings->value(KEY_CURR_BACK).toString();
	else
		return str;
}

RandomBackgroundLib::Result RandBackImpl::setBackground(QString filename)
{
	filename = QDir::fromNativeSeparators(filename);
	QFileInfo file(filename);

	if (!file.exists())
		return Result(RandomBackgroundLib::FAILURE, "The specified file does not exist, or is not a valid file path");

	if (file.isRelative())
		return Result(RandomBackgroundLib::FAILURE, "The specified file must be a valid absolute path");

	QString suffix = file.suffix().toLower();

	if (suffix != "jpg" && suffix != "jpeg" && suffix != "bmp")
		return Result(RandomBackgroundLib::FAILURE, "The specified file is not a jpeg or bitmap image");

	return processImage(filename);
}


RandomBackgroundLib::Result RandBackImpl::changeBackground(bool override)
{
	if (!override)
	{
		if (m_Settings->value(KEY_CHANGE_IN).toInt() == 0)
			m_Settings->setValue(KEY_CHANGE_IN, m_Settings->value(KEY_CHANGE_EVERY).toInt() - 1);
		else
		{
			m_Settings->setValue(KEY_CHANGE_IN, m_Settings->value(KEY_CHANGE_IN).toInt() - 1);
			return Result(RandomBackgroundLib::SUCCESS);
		}
	}

	Result result = chooseFile();

	if (result.code != RandomBackgroundLib::SUCCESS)
		return result;
	else
		return processImage(result.message);
}



// ********Private functions********

void RandBackImpl::loadSettingsFile(QString filename)
{
	m_Settings = new QSettings(filename, QSettings::IniFormat);

    // CurrentBackground - string, default to lowlevel::getBackground()
	if (!m_Settings->contains(KEY_CURR_BACK) || m_Settings->value(KEY_CURR_BACK).toString().trimmed().isEmpty())
        m_Settings->setValue(KEY_CURR_BACK, lowlevel::getBackground());
	else
		m_Settings->setValue(KEY_CURR_BACK, QDir::fromNativeSeparators(m_Settings->value(KEY_CURR_BACK).toString()));


	// ResizeMode - integer 0 <= int < DUMMY_INVALID, default to 0
	// 0 = smart resize, 1 = centre, 2 = stretch, 3 = tile
	if (!m_Settings->contains(KEY_RESIZE_MODE) || !m_Settings->value(KEY_RESIZE_MODE).canConvert(QVariant::Int))
		m_Settings->setValue(KEY_RESIZE_MODE, 0);
	else
	{
		int tempint = m_Settings->value(KEY_RESIZE_MODE).toInt();
		if (tempint < 0 || tempint >= RandomBackgroundLib::DUMMY_INVALID)
			m_Settings->setValue(KEY_RESIZE_MODE, 0);
	}


	// ChangeEvery - must be integer >= 1, default to 1
	if (!m_Settings->contains(KEY_CHANGE_EVERY) || !m_Settings->value(KEY_CHANGE_EVERY).canConvert(QVariant::Int)
		|| m_Settings->value(KEY_CHANGE_EVERY).toInt() < 1)
		m_Settings->setValue(KEY_CHANGE_EVERY, 1);


	// ChangeIn - any integer, default to ChangeEvery - 1
	if (!m_Settings->contains(KEY_CHANGE_IN) || !m_Settings->value(KEY_CHANGE_IN).canConvert(QVariant::Int))
		m_Settings->setValue(KEY_CHANGE_IN, m_Settings->value(KEY_CHANGE_EVERY).toInt() - 1);


	// RecentStoreNum - must be integer > 0, default to 20
	if (!m_Settings->contains(KEY_RECENT_STORE_NUM) || !m_Settings->value(KEY_RECENT_STORE_NUM).canConvert(QVariant::Int)
		|| m_Settings->value(KEY_RECENT_STORE_NUM).toInt() < 1)
		m_Settings->setValue(KEY_RECENT_STORE_NUM, 20);


	// RepeatLimit - integer 1 <= int <= RecentStoreNum, default to RecentStoreNum
	if (!m_Settings->contains(KEY_REPEAT_LIMIT) || !m_Settings->value(KEY_REPEAT_LIMIT).canConvert(QVariant::Int))
		m_Settings->setValue(KEY_REPEAT_LIMIT, m_Settings->value(KEY_RECENT_STORE_NUM).toInt());
	else
	{
		int tempint = m_Settings->value(KEY_REPEAT_LIMIT).toInt();
		if (tempint < 1 || tempint > m_Settings->value(KEY_RECENT_STORE_NUM).toInt())
			m_Settings->setValue(KEY_REPEAT_LIMIT, m_Settings->value(KEY_RECENT_STORE_NUM).toInt());
	}


	// ThumbnailWidth - must be integer >= 1, default to 100
	if (!m_Settings->contains(KEY_THUMB_WIDTH) || !m_Settings->value(KEY_THUMB_WIDTH).canConvert(QVariant::Int)
		|| m_Settings->value(KEY_THUMB_WIDTH).toInt() < 1)
		m_Settings->setValue(KEY_THUMB_WIDTH, 100);


	// ThumbnailHeight - must be integer >= 1, default to 75
	if (!m_Settings->contains(KEY_THUMB_HEIGHT) || !m_Settings->value(KEY_THUMB_HEIGHT).canConvert(QVariant::Int)
		|| m_Settings->value(KEY_THUMB_HEIGHT).toInt() < 1)
		m_Settings->setValue(KEY_THUMB_HEIGHT, 75);


	// Diagnostics - bool
	if (!m_Settings->contains(KEY_DIAGNOSTICS))
		m_Settings->setValue(KEY_DIAGNOSTICS, false);
	else
		m_Settings->setValue(KEY_DIAGNOSTICS, m_Settings->value(KEY_DIAGNOSTICS).toBool());
}


void RandBackImpl::loadPathList(QString filename, QStringList& list)
{
	QString str;
	QFile file(filename);

	list.clear();

	// Check that the file can be opened
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	// Load the contents of the file
	while (!file.atEnd())
	{
		str = QDir::fromNativeSeparators(file.readLine().trimmed());
		if (!str.isEmpty() && !list.contains(str, Qt::CaseInsensitive))
			list << str;
	}

	file.close();
}

bool RandBackImpl::savePathList(QString filename, QStringList list)
{
	QFile file(filename);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;

	QTextStream out(&file);
	for (int i = 0; i < list.count(); ++i)
		out << list[i] << "\n";

	file.close();
	return true;
}

bool RandBackImpl::setPathList(QStringList newlist, QString filename, QStringList& list)
{
	QString str;

	list.clear();

	for (int i = 0; i < newlist.count(); ++i)
	{
		str = QDir::fromNativeSeparators(newlist[i]);

		if (!list.contains(str, Qt::CaseInsensitive))
			list << str;
	}

	return savePathList(filename, list);
}

bool RandBackImpl::addToPathList(QString item, QString filename, QStringList& list)
{
	item = QDir::fromNativeSeparators(item);

	if (list.contains(item, Qt::CaseInsensitive))
		return true;

	list << item;
	return savePathList(filename, list);
}

bool RandBackImpl::removeFromPathList(QString item, QString filename, QStringList& list)
{
	item = QDir::fromNativeSeparators(item);

	if (!list.contains(item, Qt::CaseInsensitive))
		return true;

	list.removeAll(item);
	return savePathList(filename, list);
}

bool RandBackImpl::clearPathList(QString filename, QStringList& list)
{
	if (list.isEmpty())
		return true;

	list.clear();
	return savePathList(filename, list);
}


// Pick a file at random and return its filename
RandomBackgroundLib::Result RandBackImpl::chooseFile()
{
	QStringList chooselist;
	QStringList excludelist;
	QStringList blacklist;
	QStringList filelist;
	QStringList recentlist = m_RecentImages.mid(0, m_Settings->value(KEY_REPEAT_LIMIT).toInt());
	QTime timer;
	int enumTime = 0;
	int numfiles = 0;
	int chooseTime = 0;
	bool diagnostics = getDiagnosticsOn();

	// Check all the folders in the folder list actually exist
	if (m_Folders->getActiveFolders(chooselist, f_UserChoice))
	{
		if (!m_Folders->save(m_FoldersFile) && f_Warning)
			(*f_Warning)("Failed to save the updated folder list.");
	}

	if (chooselist.isEmpty())
		return Result(RandomBackgroundLib::FAILURE, "No folders are specified to search in");


	// Check all the folders in the excluded list actually exist
	excludelist = m_Excluded;

	if (removeNonexistentItems(excludelist, m_Excluded, "excluded folder", "excluded folder list", true))
	{
		if (!savePathList(m_ExcludedFile, m_Excluded) && f_Warning)
			(*f_Warning)("Failed to save the updated excluded folder list.");
	}


	// Check all the files in the blacklist actually exist
	blacklist = m_Blacklist;

	if (removeNonexistentItems(blacklist, m_Blacklist, "blacklisted file", "blacklist", false))
	{
		if (!savePathList(m_BlacklistFile, m_Blacklist) && f_Warning)
			(*f_Warning)("Failed to save the updated blacklist.");
	}

	if (diagnostics)
		timer.start();

	// Enumerate all files in list
	for (int i = 0; i < chooselist.count(); ++i)
		enumerateFiles(chooselist[i], filelist, excludelist, blacklist);

	if (diagnostics)
	{
		enumTime = timer.restart();
		numfiles = filelist.count();
	}

	// Choose a suitable file, making sure it is not the current background
	QString str;
	int tempint;
	randomise();

    QString curfile = lowlevel::getBackground();
	if (curfile == m_ResizeFile)
		curfile = m_Settings->value(KEY_CURR_BACK).toString();


	while (!filelist.isEmpty())
	{
		tempint = randint(0, filelist.count() - 1);

		if (!recentlist.contains(filelist[tempint], Qt::CaseInsensitive)
			&& curfile.compare(filelist[tempint], Qt::CaseInsensitive) != 0)
		{
			str = filelist[tempint];
			break;
		}
		else
			filelist.removeAt(tempint);
	}

	if (str.isEmpty())
	{
		// Try the files in the recent list
		for (tempint = recentlist.count() - 1; tempint > 0; --tempint)
		{
			if (QFile::exists(recentlist[tempint]))
			{
				str = recentlist[tempint];
				break;
			}
		}
	}

	if (diagnostics)
	{
		chooseTime = timer.elapsed();
		LOG.timestamp() << "Enumerated " << numfiles << " files in " << enumTime << " ms. Chose file from list in " << chooseTime << " ms.\n";
	}

	if (str.isEmpty())
		return Result(RandomBackgroundLib::FAILURE, "None of the specified folders contained suitable files");
	else
		return Result(RandomBackgroundLib::SUCCESS, str);
}

// Returns true if persistlist has been modified
bool RandBackImpl::removeNonexistentItems(QStringList& uselist, QStringList& persistlist, QString itemtype, QString listname, bool isDirList)
{
	bool modified = false;

	for (int i = persistlist.count() - 1; i >= 0; --i)
	{
		if (isDirList ? QDir(persistlist[i]).exists() : QFile::exists(persistlist[i]))
			continue;

		if (f_UserChoice && (*f_UserChoice)("The " + itemtype + " '" + QDir::toNativeSeparators(persistlist[i]) + "' does not exist. Would you like to remove it from the " + listname + "?"))
		{
			modified = true;
			persistlist.removeAt(i);
		}

		uselist.removeAt(i);
	}

	return modified;
}

// Add the names of all JPG and BMP files in foldername to filelist, and call recursively for subfolders
void RandBackImpl::enumerateFiles(QString foldername, QStringList& filelist, const QStringList& excludelist, const QStringList& blacklist)
{
	// Make sure this folder is not in the excludelist
	if (excludelist.contains(foldername, Qt::CaseInsensitive))
		return;

	QDir folder(foldername);
	QFileInfoList infolist = folder.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files | QDir::NoSymLinks);

	for (int i = 0; i < infolist.count(); ++i)
	{
		if (infolist[i].isDir())
			enumerateFiles(infolist[i].absoluteFilePath(), filelist, excludelist, blacklist);
		else
		{
			QString str = infolist[i].suffix().toLower();

			if (str == "jpg" || str == "jpeg" || str == "bmp")
			{
				str = infolist[i].absoluteFilePath();

				if (!blacklist.contains(str, Qt::CaseInsensitive) && !filelist.contains(str, Qt::CaseInsensitive))
					filelist << str;
			}
		}
	}
}


RandomBackgroundLib::Result RandBackImpl::processImage(QString filename)
{
	QString target = filename;
	QTime timer;
	int processTime = 0;
	int setTime = 0;
	bool diagnostics = getDiagnosticsOn();

	RandomBackgroundLib::ResizeMode mode = getResizeMode();

	if (diagnostics)
		timer.start();

	if (mode == RandomBackgroundLib::SMART_RESIZE)
	{
		// Smart Resize mode
		QImage image(filename);

		// Get the screen size
		QRect screen = QApplication::desktop()->screenGeometry();
		int xres = screen.width();
		int yres = screen.height();

		// Resize image if necessary
		if (image.width() > xres || image.height() > yres)
		{
			image = smartResize(image, xres, yres);

			// Save image
			if (!image.save(m_ResizeFile, "BMP", 100))
			{
				if (f_Warning)
					(*f_Warning)("Unable to save a resized version of the image. The full sized version will be used.");
			}
			else
				target = m_ResizeFile;

		}
    }

    lowlevel::setResizeMode(mode);


    if (lowlevel::convertJpegsToBmp())
	{
		QString tempinfo = QFileInfo(target).suffix().toLower();

		if (tempinfo == "jpg" || tempinfo == "jpeg")
		{
			QImage image(target);

			if (!image.save(m_ResizeFile, "BMP", 100))
				return Result(RandomBackgroundLib::FAILURE, "Unable to convert jpeg image to bmp");
			else
				target = m_ResizeFile;
		}
	}

	if (diagnostics)
		processTime = timer.restart();

	// Set background
    QString err;
    if (!lowlevel::setBackground(target, err))
        return Result(RandomBackgroundLib::FAILURE, "Failed to set the desktop background (system message: " + err + ")");

	if (diagnostics)
	{
		setTime = timer.elapsed();
		LOG.timestamp() << "Set desktop background. Processing time: " << processTime << " ms, setting time: " << setTime << " ms, total: " << processTime + setTime << " ms\n";
	}

	m_Settings->setValue(KEY_CURR_BACK, filename);

	if (addToRecentList(filename))
		return Result(RandomBackgroundLib::SUCCESS);
	else
		return Result(RandomBackgroundLib::PARTIAL_SUCCESS, "Failed to add the image to the recent images list");
}

QImage RandBackImpl::smartResize(QImage image, int xres, int yres)
{
	int width = image.width();
	int height = image.height();

	if ((float)xres / (float)yres < (float)width / (float)height || width <= height)
	{
		// Scale to height
		image = image.scaledToHeight(yres, Qt::SmoothTransformation);

		if (image.width() > xres)
			image = image.copy((image.width() - xres) / 2, 0, xres, yres);
	}
	else
    {
		// Scale to width
		image = image.scaledToWidth(xres, Qt::SmoothTransformation);

		if (image.height() > yres)
			image = image.copy(0, (image.height() - yres) / 2, xres, yres);
	}

	return image;
}

bool RandBackImpl::loadRecentList(QString filename, QStringList& images, QList<int>& thumbnails, int maxlen)
{
	QString line;
	int i = 0;
	int thumb;
	int temp;
	bool ok = false;
	QFile file(filename);

	images.clear();
	thumbnails.clear();

	// If file doesn't exist, keep the empty list
	if (!file.exists())
		return true;

	// Check that the file can be opened
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	// Load the contents of the file
	while (!file.atEnd() && i < maxlen)
	{
		line = QDir::fromNativeSeparators(file.readLine().trimmed());

		if (!line.isEmpty())
		{
			// Parse
			if ((temp = line.indexOf(' ')) == -1 || temp == line.length() - 1)
			{
				images.clear();
				thumbnails.clear();
				return false;
			}

			thumb = line.left(temp).toInt(&ok);

			if (!ok)
			{
				images.clear();
				thumbnails.clear();
				return false;
			}

			images << line.mid(temp + 1);
			thumbnails << thumb;

			++i;
		}
	}

	file.close();
	return true;
}

bool RandBackImpl::saveRecentList(QString filename, QStringList images, QList<int> thumbnails)
{
	QFile file(filename);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;

	QTextStream out(&file);
	for (int i = 0; i < images.count(); ++i)
		out << thumbnails[i] << " " << images[i] << "\n";

	file.close();
	return true;
}


bool RandBackImpl::addToRecentList(QString filename)
{
	QStringList images = m_RecentImages;
	QList<int> thumbs = m_RecentThumbnails;
	int index = images.indexOf(QRegExp(filename, Qt::CaseInsensitive, QRegExp::FixedString));
	int thumb = -1;
	bool generateThumb = false;
	QDir dir(m_ThumbnailPath);

	if (index != -1)
	{
		// Item is already in list
		thumb = thumbs[index];

		if (thumb < 0 || !QFile::exists(m_ThumbnailPath + "Thumbnail_" + QString::number(thumb) + ".jpg"))
			generateThumb = true;

		images.removeAt(index);
		thumbs.removeAt(index);
		images.prepend(filename);
		thumbs.prepend(thumb);
	}
	else
	{
		// Item is not in list
		generateThumb = true;

		if (images.count() >= getRecentStoreNum())
		{
			images.removeLast();
			thumbs.removeLast();
		}

		images.prepend(filename);
		thumbs.prepend(thumb);
	}

	if (thumb < 0)
	{
		thumb = 0;

		while (thumbs.contains(thumb))
			++thumb;

		thumbs[0] = thumb;
	}

	if (!saveRecentList(m_RecentImagesFile, images, thumbs))
		return false;

	m_RecentImages = images;
	m_RecentThumbnails = thumbs;

	// Create new thumbnail
	if (generateThumb)
	{
		QImage image(filename);

		int xres = getThumbnailWidth();
		int yres = getThumbnailHeight();

		// Resize image if necessary
		if (image.width() > xres || image.height() > yres)
			image = smartResize(image, xres, yres);

		// Save image
		if (!dir.exists() && !dir.mkpath(""))
			return false;

		return image.save(m_ThumbnailPath + "Thumbnail_" + QString::number(thumb) + ".jpg", "JPG", 85);
	}
	else
		return true;
}



// Initialise the random number generator with the current time
void RandBackImpl::randomise()
{
	qsrand(time(0));
	qrand();
}

// Generate a random integer between start and end inclusive
int RandBackImpl::randint(int start, int end)
{
	return start + int(qRound(double(qrand())/double(RAND_MAX) * end));
}
