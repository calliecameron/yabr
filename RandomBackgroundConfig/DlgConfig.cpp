#include <QApplication>
#include <QCheckBox>
#include <QDir>
#include <QFileDialog>
#include <QGroupBox>
#include <QInputDialog>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <RandomBackgroundLib.h>

#include "FolderList.h"
#include "DlgConfig.h"
#include "DlgList.h"
#include "DlgProfile.h"
#include "DlgRecent.h"
#include "sysfuncs.h"


DlgConfig::DlgConfig(QWidget* parent) : QDialog(parent, Qt::Window | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint)
{
    QApplication::setApplicationName("Desktop Background Randomiser");

#ifdef Q_OS_WIN
    QString settingsDirPrefix = QApplication::applicationDirPath() + "/";
#endif
#ifdef Q_OS_LINUX
    QString settingsDirPrefix = QDir::homePath() + "/.randomBackground/";
    if (!QDir().mkpath(settingsDirPrefix))
        QMessageBox::warning(0, QApplication::applicationName(), "Unable to create the application's settings directory: '" + settingsDirPrefix + "'.");
#endif

    m_Lib = RandomBackgroundLib::init(
                settingsDirPrefix + "Background.ini",
                settingsDirPrefix + "Folders.txt",
                settingsDirPrefix + "Excluded.txt",
                settingsDirPrefix + "Blacklist.txt",
                settingsDirPrefix + "RecentImages.txt",
                settingsDirPrefix + "BackgroundResized.bmp",
                settingsDirPrefix,
                settingsDirPrefix + "RandomBackground.log",
                &warnCallback,
                &choiceCallback);


	setWindowTitle("Desktop Background Randomiser Configuration");

	layMain = new QVBoxLayout;

	// Top section; information labels
	lblCurBack = new QLabel(this);
	setLblCurBackText(m_Lib->getBackground());
	lblCurBack->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(lblCurBack, SIGNAL(linkActivated(QString)), this, SLOT(curBackClicked(QString)));
	connect(lblCurBack, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(curBackMenu(QPoint)));

	layMain->addWidget(lblCurBack);
	layMain->addWidget(lblChangeIn = new QLabel(this));
	setLblChangeInText();


	// Next section; timing controls
	fraTiming = new QGroupBox("Timing", this);
	layTiming1 = new QVBoxLayout;
	layTiming2 = new QHBoxLayout;
	layTiming3 = new QHBoxLayout;
	layTiming4 = new QHBoxLayout;
	layTiming5 = new QHBoxLayout;
	layTiming6 = new QHBoxLayout;


	lblTiming1 = new QLabel("Change background every", fraTiming);
	lblTiming2 = new QLabel("restarts.", fraTiming);

	spnChangeEvery = new QSpinBox(fraTiming);
	spnChangeEvery->setMinimum(1);
	spnChangeEvery->setValue(m_Lib->getChangeEvery());
	connect(spnChangeEvery, SIGNAL(valueChanged(int)), this, SLOT(setChangeEvery(int)));

	layTiming2->addWidget(lblTiming1);
	layTiming2->addWidget(spnChangeEvery);
	layTiming2->addWidget(lblTiming2);
	layTiming2->setStretchFactor(lblTiming1, 0);
	layTiming2->setStretchFactor(spnChangeEvery, 0);
	layTiming2->setStretchFactor(lblTiming2, 1);
	layTiming1->addLayout(layTiming2);

	cmdChangeNow = new QPushButton("Change now", fraTiming);
	connect(cmdChangeNow, SIGNAL(clicked()), this, SLOT(changeNow()));

	cmdKeepFor = new QPushButton("Keep current background...", fraTiming);
	connect(cmdKeepFor, SIGNAL(clicked()), this, SLOT(keepFor()));

	layTiming3->addWidget(cmdChangeNow);
	layTiming3->addWidget(cmdKeepFor);
	layTiming1->addLayout(layTiming3);


	cmdChooseImage = new QPushButton("Set the desktop background to a particular image...", fraTiming);
	connect(cmdChooseImage, SIGNAL(clicked()), this, SLOT(setImage()));

	cmdAddToBlacklist = new QPushButton("Add current background to the blacklist", fraTiming);
	connect(cmdAddToBlacklist, SIGNAL(clicked()), this, SLOT(addToBlacklist()));

	layTiming4->addWidget(cmdChooseImage);
	layTiming4->addWidget(cmdAddToBlacklist);
	layTiming1->addLayout(layTiming4);



	spnRecentStore = new QSpinBox(fraTiming);
	spnRecentStore->setMinimum(1);
	spnRecentStore->setValue(m_Lib->getRecentStoreNum());
	connect(spnRecentStore, SIGNAL(valueChanged(int)), this, SLOT(setRecentStoreNum(int)));

	layTiming5->addWidget(new QLabel("Remember the last", fraTiming));
	layTiming5->addWidget(spnRecentStore);
	layTiming5->addWidget(new QLabel("recent images.", fraTiming), 1);
	layTiming1->addLayout(layTiming5);


	spnRepeatLimit = new QSpinBox(fraTiming);
	spnRepeatLimit->setMinimum(1);
	spnRepeatLimit->setMaximum(m_Lib->getRecentStoreNum());
	spnRepeatLimit->setValue(m_Lib->getRepeatLimit());
	connect(spnRepeatLimit, SIGNAL(valueChanged(int)), this, SLOT(setRepeatLimit(int)));

	layTiming6->addWidget(new QLabel("Don't reuse the most recent", fraTiming));
	layTiming6->addWidget(spnRepeatLimit);
	layTiming6->addWidget(new QLabel("images.", fraTiming), 1);
	layTiming1->addLayout(layTiming6);


	cmdRecent = new QPushButton("\nRecent images...\n", fraTiming);
	connect(cmdRecent, SIGNAL(clicked()), this, SLOT(recentImages()));

	layTiming1->addWidget(cmdRecent);

	fraTiming->setLayout(layTiming1);

	layMain->addWidget(fraTiming);


	// Next section; folder lists
	fraFolders = new QGroupBox("Folders", this);
	layFolders = new QHBoxLayout;

	cmdFolders = new QPushButton("Manage source\nfolders...", fraFolders);
	connect(cmdFolders, SIGNAL(clicked()), this, SLOT(manageFolders()));

	cmdExcluded = new QPushButton("Manage excluded\nfolders...", fraFolders);
	connect(cmdExcluded, SIGNAL(clicked()), this, SLOT(manageExcluded()));

	cmdBlacklist = new QPushButton("Manage blacklisted\nfiles...", fraFolders);
	connect(cmdBlacklist, SIGNAL(clicked()), this, SLOT(manageBlacklist()));

	layFolders->addWidget(cmdFolders);
	layFolders->addWidget(cmdExcluded);
	layFolders->addWidget(cmdBlacklist);
	fraFolders->setLayout(layFolders);

	layMain->addWidget(fraFolders);


	// Next section; resize style
	fraStyle = new QGroupBox("Style", this);
	
	lblStyle = new QLabel("Select the display style for the desktop background:", fraStyle);

	optSmart = new QRadioButton("Smart Resize\n(recommended)", fraStyle);
	optSmart->setToolTip("Resizes the image intelligently so that most\nof the image is visible without distortion.");

	optCentre = new QRadioButton("Centre,\nno resize", fraStyle);
	optCentre->setToolTip("Full-size image centred on the screen; for large images this\nmeans only a small area in the centre of the image is visible.");

	optStretch = new QRadioButton("Stretch", fraStyle);
	optStretch->setToolTip("Stretches the image to the screen size; the whole image is visible but is usually distorted.");

	optTile = new QRadioButton("Tile", fraStyle);
	optTile->setToolTip("Tiles several copies of the image on the screen; only really useful for\nvery small images. (For large images, centres on the upper left corner.)");

	RandomBackgroundLib::ResizeMode mode = m_Lib->getResizeMode();

	if (mode == RandomBackgroundLib::SMART_RESIZE)
		optSmart->setChecked(true);
	else if (mode == RandomBackgroundLib::CENTRE)
		optCentre->setChecked(true);
	else if (mode == RandomBackgroundLib::STRETCH)
		optStretch->setChecked(true);
	else if (mode == RandomBackgroundLib::TILE)
		optTile->setChecked(true);

	connect(optSmart, SIGNAL(toggled(bool)), this, SLOT(styleChanged(bool)));
	connect(optCentre, SIGNAL(toggled(bool)), this, SLOT(styleChanged(bool)));
	connect(optStretch, SIGNAL(toggled(bool)), this, SLOT(styleChanged(bool)));
	connect(optTile, SIGNAL(toggled(bool)), this, SLOT(styleChanged(bool)));


	layStyle1 = new QVBoxLayout;
	layStyle2 = new QHBoxLayout;

	layStyle2->addWidget(optSmart);
	layStyle2->addWidget(optCentre);
	layStyle2->addWidget(optStretch);
	layStyle2->addWidget(optTile);

	layStyle1->addWidget(lblStyle);
	layStyle1->addLayout(layStyle2);

	fraStyle->setLayout(layStyle1);

	layMain->addWidget(fraStyle);


	// Bottom section; enabled checkbox and close button
	chkEnabled = new QCheckBox("Enable background changing on startup", this);
    chkEnabled->setChecked(sys::getRunAtStartup());
    connect(chkEnabled, SIGNAL(toggled(bool)), this, SLOT(setRunAtStartup(bool)));

	layMain->addWidget(chkEnabled);

	cmdClose = new QPushButton("Close", this);
	connect(cmdClose, SIGNAL(clicked()), qApp, SLOT(quit()));
	layMain->addWidget(cmdClose);

	setLayout(layMain);
}


DlgConfig::~DlgConfig()
{
	delete m_Lib;
}

void DlgConfig::setLblCurBackText(QString background)
{
	lblCurBack->setText("Current background: <a href=\"" + QDir::toNativeSeparators(background) + "\">" + QDir::toNativeSeparators(background) + "</a>");
}

// Set the text of lblChangeIn based on the value of ChangeIn
void DlgConfig::setLblChangeInText()
{
	int i = m_Lib->getChangeIn();

	if (i == 0)
		lblChangeIn->setText("Background will change on next restart.");
	else if (i > 0)
		lblChangeIn->setText("Background will change in " + QString::number(i + 1) + " restarts.");
	else
		lblChangeIn->setText("Background will not change.");
}

void DlgConfig::curBackClicked(QString link)
{
    QString err;
    if (!sys::shellExecute(link, err))
        QMessageBox::warning(0, qApp->applicationName(), "Could not open the specified path (system message: " + err + ").");
}

void DlgConfig::curBackMenu(QPoint pos)
{
	QMenu* menu = new QMenu;
	menu->addAction("Open file", this, SLOT(openCurBack()));
	menu->addAction("Open containing folder", this, SLOT(openCurBackFolder()));
	menu->exec(lblCurBack->mapToGlobal(pos));
	delete menu;
}

void DlgConfig::openCurBack()
{
    QString err;
    if (!sys::shellExecute(m_Lib->getBackground(), err))
        QMessageBox::warning(0, qApp->applicationName(), "Could not open the specified path (system message: " + err + ").");
}

void DlgConfig::openCurBackFolder()
{
    QString err;
    if (!sys::shellExecute(QFileInfo(m_Lib->getBackground()).canonicalPath(), err))
        QMessageBox::warning(0, qApp->applicationName(), "Could not open the specified path (system message: " + err + ").");
}


// Set the value of ChangeEvery
void DlgConfig::setChangeEvery(int i)
{
	m_Lib->setChangeEvery(i);
}

void DlgConfig::changeNow()
{
	RandomBackgroundLib::Result result = m_Lib->changeBackground(true);

	if (result.code == RandomBackgroundLib::PARTIAL_SUCCESS)
		QMessageBox::warning(0, qApp->applicationName(), result.message);
	else if (result.code == RandomBackgroundLib::FAILURE)
		QMessageBox::critical(0, qApp->applicationName(), "Unable to change the desktop background. " + result.message + ".");

	setLblCurBackText(m_Lib->getBackground());
}

// Get a keep for value from the user and set ChangeIn to it
void DlgConfig::keepFor()
{
	bool ok = false;

	int val = QInputDialog::getInt(this, qApp->applicationName(), "Keep this background for how many restarts?\n(Normal behaviour will resume once this period is over).", m_Lib->getChangeIn(), 0, 100, 1, &ok);

	if (ok)
	{
		m_Lib->setChangeIn(val);
		setLblChangeInText();
	}
}

// Set the desktop background to an image picked from a file dialog
void DlgConfig::setImage()
{
	QString str = QFileDialog::getOpenFileName(this, "Select a desktop background", QDir::homePath(), "Images (*.bmp *.jpg *.jpeg)");

	if (!str.isEmpty())
	{
		RandomBackgroundLib::Result result = m_Lib->setBackground(str);

		if (result.code == RandomBackgroundLib::PARTIAL_SUCCESS)
			QMessageBox::warning(0, qApp->applicationName(), result.message);
		else if (result.code == RandomBackgroundLib::FAILURE)
			QMessageBox::critical(0, qApp->applicationName(), "Unable to change the desktop background. " + result.message + ".");

		setLblCurBackText(m_Lib->getBackground());
	}
}

void DlgConfig::addToBlacklist()
{
	if (!m_Lib->addToBlacklist(m_Lib->getBackground()))
		QMessageBox::warning(0, qApp->applicationName(), "Unable to save the blacklist file.");
}

void DlgConfig::setRecentStoreNum(int i)
{
	m_Lib->setRecentStoreNum(i);

	if (spnRepeatLimit->value() > i)
		spnRepeatLimit->setValue(i);

	spnRepeatLimit->setMaximum(i);
}

void DlgConfig::setRepeatLimit(int i)
{
	m_Lib->setRepeatLimit(i);
}

void DlgConfig::recentImages()
{
	DlgRecent* dlgRecent = new DlgRecent(m_Lib, this);

	dlgRecent->exec();
	setLblCurBackText(m_Lib->getBackground());

	delete dlgRecent;
}

void DlgConfig::manageFolders()
{
	FolderList* list = m_Lib->getSearchFolders();
	DlgProfile* dlgProfile = new DlgProfile(list, this);

	if (dlgProfile->exec() == QDialog::Accepted)
	{
		if (!m_Lib->setSearchFolders(list))
			QMessageBox::warning(0, qApp->applicationName(), "Unable to save the search folders file.");
	}

	delete dlgProfile;
	delete list;
}

void DlgConfig::manageExcluded()
{
	DlgList* dlgList = new DlgList(m_Lib->getExcludedFolders(), "Ignore images in the following folders:", "folder", true, this);

	if (dlgList->exec() == QDialog::Accepted)
	{
		if (!m_Lib->setExcludedFolders(dlgList->getList()))
			QMessageBox::warning(0, qApp->applicationName(), "Unable to save the excluded folders file.");
	}

	delete dlgList;
}

void DlgConfig::manageBlacklist()
{
	DlgList* dlgList = new DlgList(m_Lib->getBlacklist(), "Ignore the following images:", "file", false, this);

	if (dlgList->exec() == QDialog::Accepted)
	{
		if (!m_Lib->setBlacklist(dlgList->getList()))
			QMessageBox::warning(0, qApp->applicationName(), "Unable to save the blacklist file.");
	}

	delete dlgList;
}

// Update the background style based on the radiobutton checked
void DlgConfig::styleChanged(bool checked)
{
	if (checked)
	{
		if (optSmart->isChecked())
			m_Lib->setResizeMode(RandomBackgroundLib::SMART_RESIZE);
		else if (optCentre->isChecked())
			m_Lib->setResizeMode(RandomBackgroundLib::CENTRE);
		else if (optStretch->isChecked())
			m_Lib->setResizeMode(RandomBackgroundLib::STRETCH);
		else if (optTile->isChecked())
			m_Lib->setResizeMode(RandomBackgroundLib::TILE);

		RandomBackgroundLib::Result result = m_Lib->setBackground(m_Lib->getBackground());

		if (result.code == RandomBackgroundLib::PARTIAL_SUCCESS)
			QMessageBox::warning(0, qApp->applicationName(), result.message);
		else if (result.code == RandomBackgroundLib::FAILURE)
			QMessageBox::critical(0, qApp->applicationName(), "Unable to change the desktop background. " + result.message + ".");
		else
			setLblCurBackText(m_Lib->getBackground());
	}
}


void DlgConfig::setRunAtStartup(bool b)
{
    sys::setRunAtStartup(b);
}

void DlgConfig::warnCallback(QString str)
{
	QMessageBox::warning(0, qApp->applicationName(), str);
}

bool DlgConfig::choiceCallback(QString str)
{
	return QMessageBox::warning(0, qApp->applicationName(), str, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes;
}
