#ifndef RANDBACKIMPL_H
#define RANDBACKIMPL_H

#include "RandomBackgroundLib.h"

class QSettings;
class FolderListImpl;


class RandBackImpl : public RandomBackgroundLib
{
public:

	RandBackImpl(QString settings, QString folders, QString excluded, QString blacklist, QString recentImages, QString resizeFile, QString thumbnailPath, QString logFile, WarnFunc warnFunc, ChoiceFunc choiceFunc);
	virtual ~RandBackImpl();


	virtual ResizeMode getResizeMode() const;
	virtual void setResizeMode(ResizeMode mode);

	virtual int getChangeEvery() const;
	virtual void setChangeEvery(int value);

	virtual int getChangeIn() const;
	virtual void setChangeIn(int value);

	virtual int getRecentStoreNum() const;
	virtual void setRecentStoreNum(int value);

	virtual int getRepeatLimit() const;
	virtual void setRepeatLimit(int value);

	virtual int getThumbnailWidth() const;
	virtual void setThumbnailWidth(int value);

	virtual int getThumbnailHeight() const;
	virtual void setThumbnailHeight(int value);

	virtual bool getDiagnosticsOn() const;
	virtual void setDiagnosticsOn(bool value);

	virtual QString getSettingsFile() const;
	virtual QString getSearchFoldersFile() const;
	virtual QString getExcludedFoldersFile() const;
	virtual QString getBlacklistFile() const;
	virtual QString getRecentImagesFile() const;
	virtual QString getResizeFile() const;

	virtual FolderList* getSearchFolders() const;
	virtual bool setSearchFolders(FolderList* folders);

	virtual QStringList getExcludedFolders() const;
	virtual bool setExcludedFolders(QStringList list);
	virtual bool addExcludedFolder(QString folder);
	virtual bool removeExcludedFolder(QString folder);
	virtual bool clearExcludedFolders();

	virtual QStringList getBlacklist() const;
	virtual bool setBlacklist(QStringList list);
	virtual bool addToBlacklist(QString file);
	virtual bool removeFromBlacklist(QString file);
	virtual bool clearBlacklist();

	virtual QStringList getRecentImages() const;
	virtual QStringList getRecentImageThumbnails() const;


	virtual QString getBackground() const;
	virtual Result setBackground(QString filename);

	virtual Result changeBackground(bool override=false);


private:


	void loadSettingsFile(QString filename);

	static void loadPathList(QString filename, QStringList& list);
	static bool savePathList(QString filename, QStringList list);
	static bool setPathList(QStringList newlist, QString filename, QStringList& list);
	static bool addToPathList(QString item, QString filename, QStringList& list);
	static bool removeFromPathList(QString item, QString filename, QStringList& list);
	static bool clearPathList(QString filename, QStringList& list);

	Result chooseFile();
	bool removeNonexistentItems(QStringList& uselist, QStringList& persistlist, QString itemtype, QString listname, bool isDirList);
	void enumerateFiles(QString foldername, QStringList& filelist, const QStringList& excludelist, const QStringList& blacklist);
	Result processImage(QString filename);
	//QImage oldSmartResize(QImage image, int xres, int yres);
	QImage smartResize(QImage image, int xres, int yres);

	static bool loadRecentList(QString filename, QStringList& images, QList<int>& thumbnails, int maxlen);
	static bool saveRecentList(QString filename, QStringList images, QList<int> thumbnails);
	bool addToRecentList(QString filename);

	static QString systemGetBackground();
	static bool systemSetBackground(QString filename);
	static QString systemGetErrorMsg();
	static void randomise();
	static int randint(int start, int end);

	WarnFunc f_Warning;
	ChoiceFunc f_UserChoice;

	QString m_FoldersFile;
	QString m_ExcludedFile;
	QString m_BlacklistFile;
	QString m_RecentImagesFile;
	QString m_ResizeFile;
	QString m_ThumbnailPath;

	QSettings* m_Settings;

	FolderListImpl* m_Folders;
	QStringList m_Excluded;
	QStringList m_Blacklist;
	QStringList m_RecentImages;
	QList<int> m_RecentThumbnails;

	static const QString KEY_CURR_BACK;
	static const QString KEY_RESIZE_MODE;
	static const QString KEY_CHANGE_EVERY;
	static const QString KEY_CHANGE_IN;
	static const QString KEY_RECENT_STORE_NUM;
	static const QString KEY_REPEAT_LIMIT;
	static const QString KEY_THUMB_WIDTH;
	static const QString KEY_THUMB_HEIGHT;
	static const QString KEY_DIAGNOSTICS;
};

#endif // RANDBACKIMPL_H
