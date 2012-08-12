#ifndef DLGCONFIG_H
#define DLGCONFIG_H

#include <QDialog>

class QCheckBox;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QRadioButton;
class QSpinBox;
class QVBoxLayout;
class RandomBackgroundLib;


class DlgConfig : public QDialog
{
	Q_OBJECT

public:
	DlgConfig(QWidget* parent=0);
	virtual ~DlgConfig();

private slots:
	void setRegEnabled(bool b);
	void curBackClicked(QString link);
	void curBackMenu(QPoint pos);
	void openCurBack();
	void openCurBackFolder();
	void setChangeEvery(int i);
	void changeNow();
	void keepFor();
	void setImage();
	void addToBlacklist();
	void setRecentStoreNum(int i);
	void setRepeatLimit(int i);
	void recentImages();
	void manageFolders();
	void manageExcluded();
	void manageBlacklist();
	void styleChanged(bool checked);

	

private:

	// Private functions
	void setLblCurBackText(QString background);
	void setLblChangeInText();
	bool getRegEnabled();

	static void warnCallback(QString str);
	static bool choiceCallback(QString str);


	// GUI members
	QVBoxLayout* layMain;

	// Top section; information labels
	QLabel* lblCurBack;
	QLabel* lblChangeIn;

	// Next section; timing controls
	QGroupBox* fraTiming;
	QLabel* lblTiming1;
	QLabel* lblTiming2;
	QPushButton* cmdChangeNow;
	QPushButton* cmdKeepFor;
	QSpinBox* spnChangeEvery;
	QPushButton* cmdChooseImage;
	QPushButton* cmdAddToBlacklist;
	QSpinBox* spnRecentStore;
	QSpinBox* spnRepeatLimit;
	QPushButton* cmdRecent;


	QVBoxLayout* layTiming1;
	QHBoxLayout* layTiming2;
	QHBoxLayout* layTiming3;
	QHBoxLayout* layTiming4;
	QHBoxLayout* layTiming5;
	QHBoxLayout* layTiming6;

	// Next section; folder lists
	QGroupBox* fraFolders;
	QHBoxLayout* layFolders;
	QPushButton* cmdFolders;
	QPushButton* cmdExcluded;
	QPushButton* cmdBlacklist;


	// Next section; resize style
	QGroupBox* fraStyle;
	QLabel* lblStyle;
	QRadioButton* optSmart;
	QRadioButton* optCentre;
	QRadioButton* optStretch;
	QRadioButton* optTile;

	QVBoxLayout* layStyle1;
	QHBoxLayout* layStyle2;

	// Bottom section; enabled checkbox and close button
	QCheckBox* chkEnabled;
	QPushButton* cmdClose;


	RandomBackgroundLib* m_Lib;


};


#endif // DLGCONFIG_H
