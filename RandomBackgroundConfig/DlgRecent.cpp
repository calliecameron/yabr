#include <QApplication>
#include <QButtonGroup>
#include <QDialogButtonBox>
#include <QFileInfo>
#include <QGridLayout>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <RandomBackgroundLib.h>

#include "DlgRecent.h"
#include "sysfuncs.h"

#define ICONS_X 5


DlgRecent::DlgRecent(RandomBackgroundLib* lib, QWidget* parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint), m_Lib(lib), m_Sender(0)
{
    setWindowTitle("Recent Desktop Backgrounds");

    layMain = new QVBoxLayout;

    layMain->addWidget(new QLabel("Click an image to set it as the desktop background.", this));

    layMain->addWidget(cmdChangeNow = new QPushButton("Choose a random background now", this));
    connect(cmdChangeNow, SIGNAL(clicked()), this, SLOT(changeNow()));

    scrGrid = new QScrollArea(this);
    scrGrid->setWidgetResizable(true);
    scrGrid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    wdgGrid = new QWidget(scrGrid);
    layGrid = new QGridLayout;

    wdgGrid->setLayout(layGrid);
    layMain->addWidget(scrGrid);
    grpGrid = new QButtonGroup(wdgGrid);
    scrGrid->setWidget(wdgGrid);
    connect(grpGrid, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));

    // Bottom area; buttons
    dbbMain = new QDialogButtonBox(QDialogButtonBox::Close, Qt::Horizontal, this);

    connect(dbbMain, SIGNAL(rejected()), this, SLOT(reject()));

    layMain->addWidget(dbbMain);

    setLayout(layMain);

    populateGrid();
}

DlgRecent::~DlgRecent()
{
    //
}

void DlgRecent::populateGrid()
{
    for (int i = 0; i < m_Buttons.count(); ++i)
    {
        grpGrid->removeButton(m_Buttons[i]);
        delete m_Buttons[i];
    }

    m_Buttons.clear();


    QStringList images = m_Lib->getRecentImages();
    QStringList thumbs = m_Lib->getRecentImageThumbnails();
    int w = m_Lib->getThumbnailWidth();
    int h = m_Lib->getThumbnailHeight();

    int x = 0;
    int y = 0;

    for (int i = 0; i < images.count(); ++i)
    {
        layGrid->addWidget(createButton(images[i], thumbs[i], w, h), y, x);

        x = (x + 1) % ICONS_X;
        if (x == 0)
            ++y;
    }

    if (x > 0 || (x == 0 && y == 0))
    {
        while (x < ICONS_X)
        {
            layGrid->addWidget(createDisabledButton(w, h), y, x);
            ++x;
        }
    }

    wdgGrid->adjustSize();
    scrGrid->setMinimumWidth(wdgGrid->width());
}

QPushButton* DlgRecent::createButton(QString image, QString thumbnail, int w, int h)
{
    QPushButton* button = new QPushButton(QString(), wdgGrid);
    button->setMinimumSize(w + 6, h + 6);
    button->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(button, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));

    QPixmap icon;

    if (!thumbnail.isEmpty())
        icon.load(thumbnail, "JPG");

    if (icon.isNull())
        icon.load(":/WarningDull.png", "PNG");

    button->setIconSize(QSize(w, h));
    button->setIcon(icon);

    button->setToolTip(image);
    grpGrid->addButton(button);
    m_Buttons.append(button);
    return button;
}

QPushButton* DlgRecent::createDisabledButton(int w, int h)
{
    QPushButton* button = new QPushButton(wdgGrid);
    button->setMinimumSize(w + 6, h + 6);
    button->setEnabled(false);

    m_Buttons.append(button);
    return button;
}

void DlgRecent::changeNow()
{
    RandomBackgroundLib::Result result = m_Lib->changeBackground(true);

    if (result.code == RandomBackgroundLib::PARTIAL_SUCCESS)
        QMessageBox::warning(0, qApp->applicationName(), result.message);
    else if (result.code == RandomBackgroundLib::FAILURE)
        QMessageBox::critical(0, qApp->applicationName(), "Unable to change the desktop background. " + result.message + ".");

    populateGrid();
}

void DlgRecent::buttonClicked(QAbstractButton* button)
{
    RandomBackgroundLib::Result result = m_Lib->setBackground(button->toolTip());

    if (result.code == RandomBackgroundLib::PARTIAL_SUCCESS)
        QMessageBox::warning(0, qApp->applicationName(), result.message);
    else if (result.code == RandomBackgroundLib::FAILURE)
        QMessageBox::critical(0, qApp->applicationName(), "Unable to change the desktop background. " + result.message + ".");

    populateGrid();
}

void DlgRecent::contextMenu(QPoint pos)
{
    m_Sender = (QPushButton*)sender();
    QMenu* menu = new QMenu;

    menu->addAction("Open file", this, SLOT(openFile()));
    menu->addAction("Open containing folder", this, SLOT(openFolder()));
    menu->exec(m_Sender->mapToGlobal(pos));
    delete menu;
    m_Sender = 0;
}

void DlgRecent::openFile()
{
    QString err;
    if (m_Sender == 0 || !sys::shellExecute(m_Sender->toolTip(), err))
        QMessageBox::warning(0, qApp->applicationName(), "Could not open the specified path (system message: " + err + ").");
}

void DlgRecent::openFolder()
{
    QString err;
    if (m_Sender == 0 || !sys::shellExecute(QFileInfo(m_Sender->toolTip()).canonicalPath(), err))
        QMessageBox::warning(0, qApp->applicationName(), "Could not open the specified path (system message: " + err + ").");
}
