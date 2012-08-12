#ifndef LOWLEVEL_H
#define LOWLEVEL_H

#include "RandomBackgroundLib.h"

class QString;

namespace lowlevel
{
    QString getBackground();
    bool setBackground(QString filename, QString& err);
    void setResizeMode(RandomBackgroundLib::ResizeMode mode);
    bool convertJpegsToBmp();
}

#endif // LOWLEVEL_H
