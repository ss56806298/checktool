#include "ui_widget.h"
#include "log.h"

static QTextBrowser * m_pLogBrowser = NULL;

static QFile * m_pLogFile = NULL;

bool Log::SetLogPath()
{
    QString fileName = QDateTime::currentDateTime().toString("yyyyMMddhhmm") + ".log";

    m_pLogFile = new QFile(fileName);

    if(!m_pLogFile->open(QIODevice::ReadWrite|QIODevice::Text))
    {
        m_pLogFile = NULL;

        return false;
    }

    return true;
}

bool Log::SetLogBrowser(QTextBrowser * logBrowser)
{
    m_pLogBrowser = logBrowser;

    return true;
}

bool Log::Write(QString logData, LOG_LEVEL level, bool onlyFile)
{
    if (m_pLogBrowser != NULL && !onlyFile) m_pLogBrowser->append(logData);

    if (m_pLogFile != NULL)
    {
        QTextStream in(m_pLogFile);
        QString head = "";
        switch (level)
        {
            case INFO_LEVEL:
                head = "[INFO]";
                break;
            case NOTICE_LEVEL:
                head = "[NOTICE]";
                break;
            case ERROR_LEVEL:
                head = "[ERROR]";
                break;
        }

        in << head << QDateTime::currentDateTime().toString("[yyyy.MM.dd hh:mm:ss]") << logData << "\n";
    }

    QApplication::processEvents();

    return true;
}
