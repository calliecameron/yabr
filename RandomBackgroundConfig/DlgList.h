#ifndef DLGLIST_H
#define DLGLIST_H

#include <QDialog>
#include <QStringList>

class QDialogButtonBox;
class QHBoxLayout;
class QLabel;
class QListWidget;
class QVBoxLayout;

class DlgList : public QDialog
{
    Q_OBJECT

public:
    DlgList(QStringList list, QString heading, QString itemType, bool isDirList, QWidget* parent=0);
    virtual ~DlgList();

    QStringList getList() const;


private slots:

    void addItem();
    void removeItem();

private:

    void populateList();

    QLabel* lblHeading;
    QListWidget* lstItems;
    QPushButton* cmdAdd;
    QPushButton* cmdRemove;

    QVBoxLayout* layMain;
    QHBoxLayout* layItems1;
    QVBoxLayout* layItems2;

    QDialogButtonBox* dbbMain;


    QStringList m_List;
    QString m_ItemType;
    bool m_IsDirList;

};

#endif // DLGLIST_H
