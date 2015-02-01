#ifndef DLGRECENT_H
#define DLGRECENT_H

#include <QDialog>
#include <QList>

class QAbstractButton;
class QButtonGroup;
class QDialogButtonBox;
class QGridLayout;
class QPushButton;
class QScrollArea;
class QVBoxLayout;
class RandomBackgroundLib;

class DlgRecent : public QDialog
{
    Q_OBJECT

public:
    DlgRecent(RandomBackgroundLib* lib, QWidget* parent=0);
    virtual ~DlgRecent();


private slots:

    void changeNow();
    void buttonClicked(QAbstractButton* button);
    void contextMenu(QPoint pos);
    void openFile();
    void openFolder();

private:

    void populateGrid();
    QPushButton* createButton(QString image, QString thumbnail, int w, int h);
    QPushButton* createDisabledButton(int w, int h);

    QVBoxLayout* layMain;

    QPushButton* cmdChangeNow;

    QScrollArea* scrGrid;
    QWidget* wdgGrid;
    QGridLayout* layGrid;
    QList<QPushButton*> m_Buttons;
    QButtonGroup* grpGrid;

    QDialogButtonBox* dbbMain;

    RandomBackgroundLib* m_Lib;
    QPushButton* m_Sender;

};

#endif // DLGRECENT_H
