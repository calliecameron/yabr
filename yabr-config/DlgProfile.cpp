#include <QApplication>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "DlgProfile.h"
#include "DlgList.h"
#include "FolderList.h"


DlgProfile::DlgProfile(FolderList* list, QWidget* parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint), m_List(list)
{
    setWindowTitle("Desktop Background Randomiser Configuration");

    layMain = new QVBoxLayout;

    lblHeading = new QLabel("Manage search folder profiles here. Click 'Edit' to change the folders in a profile.", this);

    lstItems = new QListWidget(this);

    cmdAdd = new QPushButton("Add", this);
    connect(cmdAdd, SIGNAL(clicked()), this, SLOT(add()));

    cmdRemove = new QPushButton("Remove", this);
    connect(cmdRemove, SIGNAL(clicked()), this, SLOT(remove()));

    cmdRename = new QPushButton("Rename", this);
    connect(cmdRename, SIGNAL(clicked()), this, SLOT(rename()));

    cmdEdit = new QPushButton("Edit", this);
    connect(cmdEdit, SIGNAL(clicked()), this, SLOT(edit()));

    cmdMoveUp = new QPushButton("Move up", this);
    connect(cmdMoveUp, SIGNAL(clicked()), this, SLOT(moveUp()));

    cmdMoveDown = new QPushButton("Move down", this);
    connect(cmdMoveDown, SIGNAL(clicked()), this, SLOT(moveDown()));

    layItems1 = new QHBoxLayout;
    layItems2 = new QVBoxLayout;

    layItems2->addWidget(cmdAdd);
    layItems2->addWidget(cmdRemove);
    layItems2->addWidget(cmdRename);
    layItems2->addWidget(cmdEdit);
    layItems2->addWidget(cmdMoveUp);
    layItems2->addWidget(cmdMoveDown);

    layItems1->addWidget(lstItems);
    layItems1->addLayout(layItems2);

    layMain->addWidget(lblHeading);
    layMain->addLayout(layItems1);

    // Bottom area; buttons
    dbbMain = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);

    connect(dbbMain, SIGNAL(rejected()), this, SLOT(reject()));
    connect(dbbMain, SIGNAL(accepted()), this, SLOT(accept()));

    layMain->addWidget(dbbMain);

    setLayout(layMain);

    populateList();
}

DlgProfile::~DlgProfile()
{
    //
}

void DlgProfile::populateList()
{
    for (int i = 0; i < m_List->getNumProfiles(); ++i)
        addListItem(m_List->getProfileName(i), m_List->getProfileActive(i));

    updateButtonState();
}

void DlgProfile::addListItem(QString name, bool active)
{
    int prevCount = lstItems->count();

    QListWidgetItem* item = new QListWidgetItem(name, lstItems);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    item->setCheckState(active ? Qt::Checked : Qt::Unchecked);

    if (prevCount == 0)
        lstItems->setCurrentRow(0);
}

void DlgProfile::updateButtonState()
{
    bool enabled = lstItems->count() > 0;

    cmdRemove->setEnabled(enabled);
    cmdRename->setEnabled(enabled);
    cmdEdit->setEnabled(enabled);
    cmdMoveUp->setEnabled(enabled);
    cmdMoveDown->setEnabled(enabled);
}

void DlgProfile::add()
{
    bool ok;
    QString str = QInputDialog::getText(0, qApp->applicationName(), "Enter a name for the new profile:", QLineEdit::Normal, QString(), &ok);

    if (!ok)
        return;

    if (!str.isEmpty())
    {
        m_List->addProfile(str, true, QStringList());
        addListItem(str, true);
        updateButtonState();
    }
}

void DlgProfile::remove()
{
    if (lstItems->currentRow() >= 0)
    {
        if (QMessageBox::question(0, qApp->applicationName(), "Are you sure you want to remove the profile '" + lstItems->currentItem()->text() + "' from the list?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            m_List->removeProfile(lstItems->currentRow());
            delete lstItems->currentItem();
            updateButtonState();
        }
    }
}

void DlgProfile::rename()
{
    if (lstItems->currentRow() >= 0)
    {
        bool ok;
        QString str = QInputDialog::getText(0, qApp->applicationName(), "Enter a new name for the profile:", QLineEdit::Normal, QString(), &ok);

        if (!ok)
            return;

        if (!str.isEmpty())
        {
            m_List->setProfileName(lstItems->currentRow(), str);
            lstItems->currentItem()->setText(str);
        }
    }
}

void DlgProfile::edit()
{
    int row = lstItems->currentRow();

    if (row >= 0 && row < lstItems->count())
    {
        DlgList* dlgList = new DlgList(m_List->getProfileFolders(row), "Select the folders to choose images from:", "folder", true, this);

        if (dlgList->exec() == QDialog::Accepted)
            m_List->setProfileFolders(row, dlgList->getList());

        delete dlgList;
    }
}

void DlgProfile::moveUp()
{
    int row = lstItems->currentRow();

    if (row > 0)
    {
        m_List->moveUp(row);
        QString str = lstItems->item(row - 1)->text();
        lstItems->item(row - 1)->setText(lstItems->item(row)->text());
        lstItems->item(row)->setText(str);
        lstItems->setCurrentRow(row - 1);
    }
}

void DlgProfile::moveDown()
{
    int row = lstItems->currentRow();

    if (row >= 0 && row < lstItems->count() - 1)
    {
        m_List->moveDown(row);
        QString str = lstItems->item(row + 1)->text();
        lstItems->item(row + 1)->setText(lstItems->item(row)->text());
        lstItems->item(row)->setText(str);
        lstItems->setCurrentRow(row + 1);
    }
}

void DlgProfile::accept()
{
    for (int i = 0; i < lstItems->count(); ++i)
        m_List->setProfileActive(i, lstItems->item(i)->checkState() == Qt::Checked);

    QDialog::accept();
}
