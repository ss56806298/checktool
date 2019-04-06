#ifndef DEFINE_H
#define DEFINE_H

#include <QDebug>
#include <QTextBrowser>
#include <QString>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

#include "windows.h"

class Log;

enum LOG_LEVEL
{
    INFO_LEVEL,
    NOTICE_LEVEL,
    ERROR_LEVEL
};

#define LogInfo(Data) Log::Write(Data, INFO_LEVEL)
#define LogNotice(Data) Log::Write(Data, NOTICE_LEVEL)
#define LogError(Data) Log::Write(Data, ERROR_LEVEL)

#endif // DEFINE_H
