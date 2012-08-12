#ifndef DLGPROFILE_H
#define DLGPROFILE_H

#include <QDialog>

class QDialogButtonBox;
class QHBoxLayout;
class QLabel;
class QListWidget;
class QVBoxLayout;

class FolderList;


class DlgProfile : public QDialog
{
	Q_OBJECT

public:
	DlgProfile(FolderList* list, QWidget* parent=0);
	virtual ~DlgProfile();


private slots:

	void add();
	void remove();
	void rename();
	void edit();
	void moveUp();
	void moveDown();

	void accept();

private:

	void populateList();
	void addListItem(QString name, bool active);
	void updateButtonState();

	QLabel* lblHeading;
	QListWidget* lstItems;
	QPushButton* cmdAdd;
	QPushButton* cmdRemove;
	QPushButton* cmdRename;
	QPushButton* cmdEdit;
	QPushButton* cmdMoveUp;
	QPushButton* cmdMoveDown;

	QVBoxLayout* layMain;
	QHBoxLayout* layItems1;
	QVBoxLayout* layItems2;

	QDialogButtonBox* dbbMain;


	FolderList* m_List;

};

#endif // DLGPROFILE_H
