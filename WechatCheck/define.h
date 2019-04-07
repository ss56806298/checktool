#ifndef DEFINE_H
#define DEFINE_H

#include <QDebug>
#include <QTextBrowser>
#include <QString>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonParseError>
#include <QTimer>

#include "windows.h"

class Log;

enum LOG_LEVEL
{
    INFO_LEVEL,
    NOTICE_LEVEL,
    ERROR_LEVEL
};

#define LogInfo(Data) Log::Write(Data, INFO_LEVEL, false)
#define LogNotice(Data) Log::Write(Data, NOTICE_LEVEL, false)
#define LogError(Data) Log::Write(Data, ERROR_LEVEL, false)

#define LogInfoF(Data) Log::Write(Data, INFO_LEVEL, true)
#define LogNoticeF(Data) Log::Write(Data, NOTICE_LEVEL, true)
#define LogErrorF(Data) Log::Write(Data, ERROR_LEVEL, true)

#endif // DEFINE_H
