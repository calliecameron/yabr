#include <QDir>
#include <QFile>
#include <QStack>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "FolderListImpl.h"


FolderListImpl::FolderListImpl()
{

}

FolderListImpl::~FolderListImpl()
{
	for (int i = 0; i < m_Profiles.count(); ++i)
		delete m_Profiles[i];
}

FolderList* FolderListImpl::duplicate() const
{
	FolderListImpl* temp = new FolderListImpl;

	for (int i = 0; i < m_Profiles.count(); ++i)
		temp->m_Profiles.append(new Profile(m_Profiles[i]));

	return temp;
}

int FolderListImpl::getNumProfiles() const
{
	return m_Profiles.count();
}

void FolderListImpl::addProfile(QString name, bool active, QStringList folders)
{
	m_Profiles.append(new Profile(name, active, cleanFolderList(folders)));
}

void FolderListImpl::removeProfile(int i)
{
	if (i >= 0 && i < m_Profiles.count())
		delete m_Profiles.takeAt(i);
}

void FolderListImpl::moveUp(int i)
{
	if (i > 0 && i < m_Profiles.count())
		m_Profiles.swap(i, i - 1);
}

void FolderListImpl::moveDown(int i)
{
	if (i >= 0 && i < m_Profiles.count() - 1)
		m_Profiles.swap(i, i + 1);
}

QString FolderListImpl::getProfileName(int i) const
{
	if (i >= 0 && i < m_Profiles.count())
		return m_Profiles[i]->name;
	else
		return QString();
}

void FolderListImpl::setProfileName(int i, QString name)
{
	if (i >= 0 && i < m_Profiles.count())
		m_Profiles[i]->name = name;
}

bool FolderListImpl::getProfileActive(int i) const
{
	if (i >= 0 && i < m_Profiles.count())
		return m_Profiles[i]->active;
	else
		return false;
}

void FolderListImpl::setProfileActive(int i, bool active)
{
	if (i >= 0 && i < m_Profiles.count())
		m_Profiles[i]->active = active;
}

QStringList FolderListImpl::getProfileFolders(int i) const
{
	if (i >= 0 && i < m_Profiles.count())
		return m_Profiles[i]->folders;
	else
		return QStringList();
}

void FolderListImpl::setProfileFolders(int i, QStringList folders)
{
	if (i >= 0 && i < m_Profiles.count())
		m_Profiles[i]->folders = cleanFolderList(folders);
}

bool FolderListImpl::getActiveFolders(QStringList& uselist, ChoiceFunc choiceFunc)
{
	bool modified = false;
	QString str;
	uselist.clear();

	for (int i = 0; i < m_Profiles.count(); ++i)
	{
		if (m_Profiles[i]->active)
		{
			for (int j = m_Profiles[i]->folders.count() - 1; j >= 0; --j)
			{
				str = m_Profiles[i]->folders[j];

				if (QDir(str).exists())
				{
					uselist.append(str);
					continue;
				}

				if (choiceFunc && (*choiceFunc)("The folder '" + QDir::toNativeSeparators(str) + "' does not exist. Would you like to remove it from the folder list?"))
				{
					modified = true;
					m_Profiles[i]->folders.removeAt(j);
				}
			}
		}
	}

	return modified;
}

QStringList FolderListImpl::cleanFolderList(QStringList list)
{
	QString str;
	QStringList clean;

	for (int i = 0; i < list.count(); ++i)
	{
		str = QDir::fromNativeSeparators(list[i]);

		if (!clean.contains(str, Qt::CaseInsensitive))
			clean << str;
	}

	return clean;
}

bool FolderListImpl::save(QString filename) const
{
	QFile file(filename);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;

	QXmlStreamWriter xml(&file);
	xml.setAutoFormatting(true);
	xml.writeStartDocument();

	xml.writeStartElement(QString(), "folders");

	for (int i = 0; i < m_Profiles.count(); ++i)
		m_Profiles[i]->writeXml(xml);

	xml.writeEndElement();
	xml.writeEndDocument();

	file.close();

	return true;
}

void FolderListImpl::Profile::writeXml(QXmlStreamWriter& xml) const
{
	xml.writeStartElement(QString(), "profile");

	xml.writeTextElement(QString(), "name", name);

	if (active)
		xml.writeEmptyElement(QString(), "active");

	for (int i = 0; i < folders.count(); ++i)
		xml.writeTextElement(QString(), "folder", folders[i]);

	xml.writeEndElement();
}

FolderListImpl* FolderListImpl::load(QString filename, WarnFunc warnFunc)
{
	QFile file(filename);

	if (!file.exists())
		return new FolderListImpl;

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		if (warnFunc)
			(*warnFunc)("Unable to open the folder list for reading. Using a blank folder list instead.");
		return new FolderListImpl;
	}

	QXmlStreamReader xml(&file);

	FolderListImpl* folders = new FolderListImpl;

	if (!folders->xmlParseAll(xml, warnFunc))
	{
		delete folders;
		folders = new FolderListImpl;
	}

	file.close();
	return folders;
}


// Main parsing function
bool FolderListImpl::xmlParseAll(QXmlStreamReader& xml, WarnFunc warnFunc)
{
	QXmlStreamReader::TokenType tokentype;
	QString temp;
	bool unrecognisedWarn = false;
	bool unassociatedWarn = false;


	// --------Parse the file--------
	while (!xml.atEnd())
	{
		tokentype = xml.readNext();

		if (tokentype == QXmlStreamReader::StartElement)
		{
			// --------Start tags--------
			temp = xml.name().toString();

			if (temp == "folders")
			{
				xmlProcessFolders(xml, warnFunc);
			}
			else
			{
				if (!unrecognisedWarn)
				{
					unrecognisedWarn = true;
					xmlUnrecognisedWarn(temp, xml, warnFunc);
				}
				xmlIgnoreCurrentTag(xml);
			}
		}
		else if (tokentype == QXmlStreamReader::Characters)
		{
			// --------Text--------

			if (!xml.isWhitespace())
			{
				if (!unassociatedWarn)
				{
					unassociatedWarn = true;
					xmlUnassociatedWarn("folders", xml, warnFunc);
				}
			}


		} // End token type

	} // End while

	if(xml.hasError())
	{
		if (warnFunc)
			(*warnFunc)("An error occurred parsing the folder definition file: line " + QString::number(xml.lineNumber()) + ", column "
			+ QString::number(xml.columnNumber()) + ".\n" + xml.errorString());
		return false;
	}

	return true;
}

// Private - Process the folders tag
void FolderListImpl::xmlProcessFolders(QXmlStreamReader& xml, WarnFunc warnFunc)
{
	QString temp;
	QXmlStreamReader::TokenType tokentype;
	bool unrecognisedWarn = false;
	bool unassociatedWarn = false;

	while (!xml.atEnd())
	{
		tokentype = xml.readNext();

		if (tokentype == QXmlStreamReader::StartElement)
		{

			// --------Start Elements--------

			temp = xml.name().toString();

			if (temp == "profile")
			{
				// Profile - folders/profile
				xmlProcessProfile(xml, warnFunc);
			}
			else
			{
				if (!unrecognisedWarn)
				{
					unrecognisedWarn = true;
					xmlUnrecognisedWarn(temp, xml, warnFunc);
				}
				xmlIgnoreCurrentTag(xml);
			}

		}
		else if (tokentype == QXmlStreamReader::EndElement)
		{
			// --------End Elements--------

			return;

		}
		else if (tokentype == QXmlStreamReader::Characters)
		{
			// --------Text--------

			if (!xml.isWhitespace())
			{
				if (!unassociatedWarn)
				{
					unassociatedWarn = true;
					xmlUnassociatedWarn("folders", xml, warnFunc);
				}
			} // End is whitespace

		} // End main if

	} // End while

}

// Private - Process the profile tag
void FolderListImpl::xmlProcessProfile(QXmlStreamReader& xml, WarnFunc warnFunc)
{
	QString temp;
	QXmlStreamReader::TokenType tokentype;
	bool unrecognisedWarn = false;
	bool unassociatedWarn = false;
	Profile* profile = new Profile("Profile", false);

	while (!xml.atEnd())
	{
		tokentype = xml.readNext();

		if (tokentype == QXmlStreamReader::StartElement)
		{

			// --------Start Elements--------

			temp = xml.name().toString();

			if (temp == "name")
			{
				// Name - profile/name
				xmlProcessName(profile, xml, warnFunc);
			}
			else if (temp == "active")
			{
				// Active - profile/active
				profile->active = true;
				xmlIgnoreCurrentTag(xml);
			}
			else if (temp == "folder")
			{
				// Folder - profile/folder
				xmlProcessFolder(profile, xml, warnFunc);
			}
			else
			{
				if (!unrecognisedWarn)
				{
					unrecognisedWarn = true;
					xmlUnrecognisedWarn(temp, xml, warnFunc);
				}
				xmlIgnoreCurrentTag(xml);
			}

		}
		else if (tokentype == QXmlStreamReader::EndElement)
		{
			// --------End Elements--------

			profile->folders = cleanFolderList(profile->folders);
			m_Profiles.append(profile);
			return;

		}
		else if (tokentype == QXmlStreamReader::Characters)
		{
			// --------Text--------

			if (!xml.isWhitespace())
			{
				if (!unassociatedWarn)
				{
					unassociatedWarn = true;
					xmlUnassociatedWarn("profile", xml, warnFunc);
				}
			} // End is whitespace

		} // End main if

	} // End while

	// Only get here by invalid XML
	delete profile;
}

// Private - Process the name tag
void FolderListImpl::xmlProcessName(Profile* profile, QXmlStreamReader& xml, WarnFunc warnFunc)
{
	QXmlStreamReader::TokenType tokentype;
	bool unrecognisedWarn = false;

	while (!xml.atEnd())
	{
		tokentype = xml.readNext();

		if (tokentype == QXmlStreamReader::StartElement)
		{

			// --------Start Elements--------

			if (!unrecognisedWarn)
			{
				unrecognisedWarn = true;
				xmlUnrecognisedWarn(xml.name().toString(), xml, warnFunc);
			}
			xmlIgnoreCurrentTag(xml);
		}
		else if (tokentype == QXmlStreamReader::EndElement)
		{
			// --------End Elements--------

			return;
		}
		else if (tokentype == QXmlStreamReader::Characters)
		{
			// --------Text--------

			if (!xml.isWhitespace())
			{
				profile->name = xml.text().toString();
			}

		} // End main if

	} // End while

}

// Private - Process the folder tag
void FolderListImpl::xmlProcessFolder(Profile* profile, QXmlStreamReader& xml, WarnFunc warnFunc)
{
	QXmlStreamReader::TokenType tokentype;
	bool unrecognisedWarn = false;

	while (!xml.atEnd())
	{
		tokentype = xml.readNext();

		if (tokentype == QXmlStreamReader::StartElement)
		{

			// --------Start Elements--------

			if (!unrecognisedWarn)
			{
				unrecognisedWarn = true;
				xmlUnrecognisedWarn(xml.name().toString(), xml, warnFunc);
			}
			xmlIgnoreCurrentTag(xml);
		}
		else if (tokentype == QXmlStreamReader::EndElement)
		{
			// --------End Elements--------

			return;
		}
		else if (tokentype == QXmlStreamReader::Characters)
		{
			// --------Text--------

			if (!xml.isWhitespace())
			{
				profile->folders.append(xml.text().toString());
			}

		} // End main if

	} // End while

}

// Private - ignore the current tag completely
void FolderListImpl::xmlIgnoreCurrentTag(QXmlStreamReader& xml)
{
	QXmlStreamReader::TokenType tokentype;
	int numtags = 0;

	while (!xml.atEnd())
	{
		tokentype = xml.readNext();

		if (tokentype == QXmlStreamReader::StartElement)
			++numtags;
		else if (tokentype == QXmlStreamReader::EndElement)
		{
			if (numtags <= 0)
				return;
			else
				--numtags;
		}
	}
}

// Private - warn about an unrecognised tag at the current position in the document
void FolderListImpl::xmlUnrecognisedWarn(QString tag, QXmlStreamReader& xml, WarnFunc warnFunc)
{
	if (warnFunc)
	{
		(*warnFunc)("An unrecognised tag '" + tag + "' was found at line "	+ QString::number(xml.lineNumber())
			+ ", column " + QString::number(xml.columnNumber()) + " of the folder definition file. The contents of this tag "
			+ "and all its child tags will be ignored. No further warnings will be given for any subsequent unrecognised tags "
			+ "found at this level in the document.");
	}
}

// Private - warn about unassociated text at the current position in the document
void FolderListImpl::xmlUnassociatedWarn(QString tag, QXmlStreamReader& xml, WarnFunc warnFunc)
{
	if (warnFunc)
	{
		(*warnFunc)("Unassociated text in the '" + tag + "' tag at line " + QString::number(xml.lineNumber())
		+ ", column " + QString::number(xml.columnNumber()) + " of the folder definition file. This text will be ignored. No "
		+ "further warnings will be given for any subsequent unassociated text found at this level in the document.");
	}
}
