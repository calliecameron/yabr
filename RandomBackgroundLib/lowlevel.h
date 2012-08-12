#ifndef LOWLEVEL_H
#define LOWLEVEL_H

#include "RandomBackgroundLib.h"

class QString;

namespace lowlevel
{
    QString getBackground();
    bool setBackground(QString filename);
    void setResizeMode(RandomBackgroundLib::ResizeMode mode);
    bool convertJpegsToBmp();
    QString getErrorMsg();
}

#endif // LOWLEVEL_H
