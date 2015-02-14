#ifndef FOLDERLIST_H
#define FOLDERLIST_H

#include <QStringList>


class FolderList
{
public:

    virtual ~FolderList() {}

    virtual FolderList* duplicate() const = 0;

    virtual int getNumProfiles() const = 0;

    virtual void addProfile(QString name, bool active, QStringList folders) = 0;
    virtual void removeProfile(int i) = 0;
    virtual void moveUp(int i) = 0;
    virtual void moveDown(int i) = 0;

    virtual QString getProfileName(int i) const = 0;
    virtual void setProfileName(int i, QString name) = 0;

    virtual bool getProfileActive(int i) const = 0;
    virtual void setProfileActive(int i, bool active) = 0;

    virtual QStringList getProfileFolders(int i) const = 0;
    virtual void setProfileFolders(int i, QStringList folders) = 0;


};

#endif // FOLDERLIST_H
