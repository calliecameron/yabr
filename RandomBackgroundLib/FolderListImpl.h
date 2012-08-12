#ifndef FOLDERLISTIMPL_H
#define FOLDERLISTIMPL_H

#include "FolderList.h"
#include "RandomBackgroundLib.h"

class QXmlStreamReader;
class QXmlStreamWriter;


class FolderListImpl : public FolderList
{
public:

	virtual ~FolderListImpl();

	virtual FolderList* duplicate() const;

	virtual int getNumProfiles() const;

	virtual void addProfile(QString name, bool active, QStringList folders);
	virtual void removeProfile(int i);
	virtual void moveUp(int i);
	virtual void moveDown(int i);

	virtual QString getProfileName(int i) const;
	virtual void setProfileName(int i, QString name);

	virtual bool getProfileActive(int i) const;
	virtual void setProfileActive(int i, bool active);

	virtual QStringList getProfileFolders(int i) const;
	virtual void setProfileFolders(int i, QStringList folders);


	bool getActiveFolders(QStringList& uselist, ChoiceFunc choiceFunc);

	bool save(QString filename) const;
	static FolderListImpl* load(QString filename, WarnFunc warnFunc);

private:

	struct Profile
	{
		Profile(QString name=QString(), bool active=true, QStringList folders=QStringList()) : name(name), active(active), folders(folders) {}
		Profile(Profile* p) : name(p->name), active(p->active), folders(p->folders) {}

		void writeXml(QXmlStreamWriter& xml) const;

		QString name;
		bool active;
		QStringList folders;
	};


	FolderListImpl();

	static QStringList cleanFolderList(QStringList list);


	bool xmlParseAll(QXmlStreamReader& xml, WarnFunc warnFunc);
	void xmlProcessFolders(QXmlStreamReader& xml, WarnFunc warnFunc);
	void xmlProcessProfile(QXmlStreamReader& xml, WarnFunc warnFunc);
	void xmlProcessName(Profile* profile, QXmlStreamReader& xml, WarnFunc warnFunc);
	void xmlProcessFolder(Profile* profile, QXmlStreamReader& xml, WarnFunc warnFunc);
	void xmlIgnoreCurrentTag(QXmlStreamReader& xml);

	void xmlUnrecognisedWarn(QString tag, QXmlStreamReader& xml, WarnFunc warnFunc);
	void xmlUnassociatedWarn(QString tag, QXmlStreamReader& xml, WarnFunc warnFunc);


	QList<Profile*> m_Profiles;

};

#endif // FOLDERLISTIMPL_H
