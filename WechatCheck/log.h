#ifndef LOG_H
#define LOG_H
#include <QString>
#include <QFile>

#include "define.h"

class Log
{
public:
    static bool SetLogPath();

    static bool SetLogBrowser(QTextBrowser * pTextBrowser);

    static bool Write(QString logData, LOG_LEVEL level, bool onlyFile);

//    QTextBrowser * m_pLogBrowser;
};
#endif // LOG_H
