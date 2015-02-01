#include <QApplication>
#include <QDialogButtonBox>
#include <QDir>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "DlgList.h"


DlgList::DlgList(QStringList list, QString heading, QString itemType, bool isDirList, QWidget* parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint), m_List(list), m_ItemType(itemType), m_IsDirList(isDirList)
{
    setWindowTitle("Desktop Background Randomiser Configuration");

    layMain = new QVBoxLayout;

    lblHeading = new QLabel(heading, this);

    lstItems = new QListWidget(this);

    cmdAdd = new QPushButton("Add\n" + itemType, this);
    connect(cmdAdd, SIGNAL(clicked()), this, SLOT(addItem()));

    cmdRemove = new QPushButton("Remove\n" + itemType, this);
    connect(cmdRemove, SIGNAL(clicked()), this, SLOT(removeItem()));

    layItems1 = new QHBoxLayout;
    layItems2 = new QVBoxLayout;

    layItems2->addWidget(cmdAdd);
    layItems2->addWidget(cmdRemove);

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

DlgList::~DlgList()
{
    //
}

QStringList DlgList::getList() const
{
    return m_List;
}

void DlgList::populateList()
{
    QString str;
    QListWidgetItem* item;

    for (int i = 0; i < m_List.count(); ++i)
    {
        str = m_List[i];

        item = new QListWidgetItem(QDir::toNativeSeparators(str), lstItems);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        if (!(m_IsDirList ? QDir(str).exists() : QFile::exists(str)))
        {
            item->setBackground(QBrush(QColor(255, 0, 0)));
            item->setToolTip("This " + m_ItemType + " does not exist.");
        }
    }

    if (lstItems->count() > 0)
        lstItems->setCurrentRow(0);
}

void DlgList::addItem()
{
    QListWidgetItem* item;

    QString str;

    if (m_IsDirList)
        str = QFileDialog::getExistingDirectory(this, "Select a " + m_ItemType + " to add to the list:", QDir::homePath(), QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly);
    else
        str = QFileDialog::getOpenFileName(this, "Select a " + m_ItemType + " to add to the list:", QDir::homePath(), "Images (*.bmp *.jpg *.jpeg)");

    if (!str.isEmpty())
    {
        if (m_List.contains(str, Qt::CaseInsensitive))
            QMessageBox::information(0, qApp->applicationName(), "The specified " + m_ItemType + " is already in the list.");
        else
        {
            m_List << QDir::fromNativeSeparators(str);

            item = new QListWidgetItem(QDir::toNativeSeparators(str), lstItems);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        }
    }
}

void DlgList::removeItem()
{
    if (lstItems->currentRow() < 0)
        QMessageBox::warning(0, qApp->applicationName(), "No " + m_ItemType + " selected.");
    else
    {
        if (QMessageBox::question(0, qApp->applicationName(), "Are you sure you want to remove the " + m_ItemType + " '" + lstItems->currentItem()->text() + "' from the list?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            m_List.removeAt(lstItems->currentRow());
            delete lstItems->currentItem();
        }
    }
}
