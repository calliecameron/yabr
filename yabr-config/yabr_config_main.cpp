#include <QApplication>
#include "DlgConfig.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    DlgConfig dlgConfig;
    dlgConfig.show();

    return app.exec();
}
