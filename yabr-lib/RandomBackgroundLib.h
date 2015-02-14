#ifndef RANDOMBACKGROUNDLIB_H
#define RANDOMBACKGROUNDLIB_H

#include <QStringList>
#include <QPixmap>

class FolderList;

typedef void (*WarnFunc)(QString);
typedef bool (*ChoiceFunc)(QString);


class RandomBackgroundLib
{
public:

    static RandomBackgroundLib* init(WarnFunc warnFunc=0, ChoiceFunc choiceFunc=0);
    static RandomBackgroundLib* instance();

    virtual ~RandomBackgroundLib() {}


    enum ResizeMode
    {
        SMART_RESIZE,
        CENTRE,
        STRETCH,
        TILE,
        DUMMY_INVALID // Must always be last
    };

    enum ResultCode
    {
        SUCCESS,
        PARTIAL_SUCCESS,
        FAILURE
    };

    struct Result
    {
    public:
        Result(ResultCode code, QString message=QString()) : code(code), message(message) {}

        ResultCode code;
        QString message;
    };


    virtual ResizeMode getResizeMode() const = 0;
    virtual void setResizeMode(ResizeMode mode) = 0;

    virtual int getChangeEvery() const = 0;
    virtual void setChangeEvery(int value) = 0;

    virtual int getChangeIn() const = 0;
    virtual void setChangeIn(int value) = 0;

    virtual int getRecentStoreNum() const = 0;
    virtual void setRecentStoreNum(int value) = 0;

    virtual int getRepeatLimit() const = 0;
    virtual void setRepeatLimit(int value) = 0;

    virtual int getThumbnailWidth() const = 0;
    virtual void setThumbnailWidth(int value) = 0;

    virtual int getThumbnailHeight() const = 0;
    virtual void setThumbnailHeight(int value) = 0;

    virtual bool getDiagnosticsOn() const = 0;
    virtual void setDiagnosticsOn(bool value) = 0;

    virtual QString getSettingsFile() const = 0;
    virtual QString getSearchFoldersFile() const = 0;
    virtual QString getExcludedFoldersFile() const = 0;
    virtual QString getBlacklistFile() const = 0;
    virtual QString getRecentImagesFile() const = 0;
    virtual QString getResizeFile() const = 0;

    virtual FolderList* getSearchFolders() const = 0;
    virtual bool setSearchFolders(FolderList* folders) = 0;

    virtual QStringList getExcludedFolders() const = 0;
    virtual bool setExcludedFolders(QStringList list) = 0;
    virtual bool addExcludedFolder(QString folder) = 0;
    virtual bool removeExcludedFolder(QString folder) = 0;
    virtual bool clearExcludedFolders() = 0;

    virtual QStringList getBlacklist() const = 0;
    virtual bool setBlacklist(QStringList list) = 0;
    virtual bool addToBlacklist(QString file) = 0;
    virtual bool removeFromBlacklist(QString file) = 0;
    virtual bool clearBlacklist() = 0;

    virtual QStringList getRecentImages() const = 0;
    virtual QStringList getRecentImageThumbnails() const = 0;


    virtual QString getBackground() const = 0;
    virtual Result setBackground(QString filename) = 0;

    virtual Result changeBackground(bool override=false) = 0;




private:

    static RandomBackgroundLib* s_Instance;

};

#endif // RANDOMBACKGROUNDLIB_H
