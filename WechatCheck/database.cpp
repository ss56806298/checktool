#include "database.h"
#include "define.h"
#include "log.h"
#include "qtredis.h"
#include "widget.h"

#include <QDebug>

Database::Database(Widget * widget)
{
    m_pWidget = widget;

    m_pAdoConnection = new AdoConnection;

    m_pAdoConnection2 = new AdoConnection;
}

Database::Database(Widget * widget, QtRedis* pRedis)
{
    m_pWidget = widget;

    m_pAdoConnection = new AdoConnection;

    m_pAdoConnection2 = new AdoConnection;

    m_pRedis = pRedis;
}

Database::~Database()
{
    //delete m_pAdoConnection;
}

bool Database::Init(QString& DBName, QString& DBAddr, QString& UserName, QString& DBPass, QString& DBPort)
{
    //SQLNCLI.1 SQLOLEDB.1
    QString openString = QString("Provider=SQLOLEDB.1;Password=%1;Persist Security Info=True;User ID=%2;Initial Catalog=%3;Data Source=%4;")
            .arg(DBPass)
            .arg(UserName)
            .arg(DBName)
            .arg(DBAddr + ", " + DBPort);

    m_pAdoConnection->open(openString);

    if (!m_pAdoConnection->isOpen()) return false;

    m_pAdoConnection2->open(openString);

    if (!m_pAdoConnection2->isOpen()) return false;

    return true;
}
bool Database::GetDomainByGameHallId(QString GameHallId, QString &domain)
{
    QString sql = "SELECT AppBindDomain FROM GameHallConfig WHERE GameHallId = " + GameHallId;

    AdoRecordset recordset(m_pAdoConnection);

    recordset.open(sql);

    if (!recordset.next())
    {
        LogNotice("指定大厅id不存在");
        return false;
    }

    domain = recordset.fieldValue(0).toString();

    return true;
}

QSet<QString> Database::GetWaitCheckDomains()
{
    QSet<QString> domains;

    QString sql = "SELECT AppBindDomain FROM GameHallConfig WHERE GameHallId > 3 ORDER BY GameHallId ASC";
    AdoRecordset recordset(m_pAdoConnection);

    recordset.open(sql);

    while(recordset.next())
    {
        QString value = recordset.fieldValue(0).toString();

        if (!domains.contains(value)) domains.insert(value);
    }

    return domains;
}

bool Database::GetWaitCheckDomains(QMap<QString, QString> & waitCheckDomainsMap)
{
    QString sql = "SELECT GameHallId, AppBindDomain FROM GameHallConfig WHERE GameHallId > 3 ORDER BY GameHallId ASC";
    AdoRecordset recordset(m_pAdoConnection);

    recordset.open(sql);

    while(recordset.next())
    {
        QString key = recordset.fieldValue(0).toString();

        QString value = recordset.fieldValue(1).toString();

        waitCheckDomainsMap[key] = value;
    }

    return true;
}

bool Database::ModifyDomain(QString domain)
{
    QString newDomain;

    if (!GetFreeDomain(newDomain))
    {
        return false;
    }

    AdoRecordset recordset(m_pAdoConnection);

    QString sql = QString("SELECT a.GameHallId, a.AppBindDomain, b.Code FROM GameHallConfig a, GameHallInfo b WHERE a.GameHallId = b.GameHallId AND a.AppBindDomain = '%1'")
            .arg(domain);

   recordset.open(sql);
   QStringList sqlList;

   while (recordset.next())
   {
//        if (m_pRedis != NULL)
//        {
//            m_pRedis->del(REDIS_GAMEHALL + recordset.fieldValue(0).toString());
//        }
        sqlList.append(QString("INSERT INTO GameHallReuseDomain(Domain, GameHallId, Code) VALUES ('%1', %2, '%3')")
                       .arg(recordset.fieldValue(1).toString())
                       .arg(recordset.fieldValue(0).toString())
                       .arg(recordset.fieldValue(2).toString()));
   }

   recordset.close();

   foreach(const QString& sql2, sqlList)
   {
       recordset.open(sql2);
   }

    sql = QString("UPDATE GameHallConfig SET AppRootDomain='%1', AppBindDomain='%2', AppCookieDomain='%3' WHERE AppBindDomain='%4'")
            .arg(newDomain)
            .arg("wx." + newDomain)
            .arg("." + newDomain)
            .arg(domain);

    recordset.open(sql);

    sql = QString("UPDATE GameHallDomainPool SET State=1, UpdateTime=GETDATE() WHERE Domain='%1'")
            .arg(newDomain);

    recordset.open(sql);

    RecoverDomain(domain);

    return true;
}

bool Database::ModifyDomain(QString domain, QString gameHallId)
{
    QString newDomain;

    if (!GetFreeDomain(newDomain, gameHallId))
    {
        return false;
    }

    AdoRecordset recordset(m_pAdoConnection);

    QString sql = QString("SELECT a.GameHallId, a.AppBindDomain, b.Code FROM GameHallConfig a, GameHallInfo b WHERE a.GameHallId = b.GameHallId AND a.AppBindDomain = '%1'")
            .arg(domain);

   recordset.open(sql);
   QStringList sqlList;

   while (recordset.next())
   {
        if (m_pRedis != NULL)
        {
            m_pRedis->del(REDIS_GAMEHALL + recordset.fieldValue(0).toString());
        }
        sqlList.append(QString("INSERT INTO GameHallReuseDomain(Domain, GameHallId, Code) VALUES ('%1', %2, '%3')")
                       .arg(recordset.fieldValue(1).toString())
                       .arg(recordset.fieldValue(0).toString())
                       .arg(recordset.fieldValue(2).toString()));
   }

   recordset.close();

   foreach(const QString& sql2, sqlList)
   {
       recordset.open(sql2);
   }

    sql = QString("UPDATE GameHallConfig SET AppRootDomain='%1', AppBindDomain='%2', AppCookieDomain='%3' WHERE AppBindDomain='%4'")
            .arg(newDomain)
            .arg("wx." + newDomain)
            .arg("." + newDomain)
            .arg(domain);

    recordset.open(sql);

    sql = QString("UPDATE GameHallDomainPool SET State=1, UpdateTime=GETDATE() WHERE Domain='%1'")
            .arg(newDomain);

    recordset.open(sql);

    RecoverDomain(domain);

    return true;
}

bool Database::ModifyHallId(QString HallId)
{
    QString newDomain;

    if (!GetFreeDomain(newDomain, HallId))
    {
        return false;
    }

    AdoRecordset recordset(m_pAdoConnection);

    QString sql = QString("SELECT a.GameHallId, a.AppBindDomain, b.Code FROM GameHallConfig a, GameHallInfo b WHERE a.GameHallId = b.GameHallId AND a.GameHallId = %1")
            .arg(HallId);

   recordset.open(sql);
   QStringList sqlList;
   QString oldDomain;

   while (recordset.next())
   {
        if (m_pRedis != NULL)
        {
            m_pRedis->del(REDIS_GAMEHALL + recordset.fieldValue(0).toString());
        }
        oldDomain = recordset.fieldValue(1).toString();
        sqlList.append(QString("INSERT INTO GameHallReuseDomain(Domain, GameHallId, Code) VALUES ('%1', %2, '%3')")
                       .arg(recordset.fieldValue(1).toString())
                       .arg(recordset.fieldValue(0).toString())
                       .arg(recordset.fieldValue(2).toString()));
   }

   recordset.close();

   foreach(const QString& sql2, sqlList)
   {
       recordset.open(sql2);
   }

    sql = QString("UPDATE GameHallConfig SET AppRootDomain='%1', AppBindDomain='%2', AppCookieDomain='%3' WHERE GameHallId=%4")
            .arg(newDomain)
            .arg("wx." + newDomain)
            .arg("." + newDomain)
            .arg(HallId);

    recordset.open(sql);

    sql = QString("UPDATE GameHallDomainPool SET State=1, UpdateTime=GETDATE() WHERE Domain='%1'")
            .arg(newDomain);

    recordset.open(sql);

    RecoverDomain(oldDomain);

    return true;
}

bool Database::CheckTableB(QString Minute)
{
    AdoRecordset recordset(m_pAdoConnection);

    QString sql = QString("SELECT ID FROM GameHallReuseDomain WHERE CreateTime < DATEADD(MINUTE, -%1, GETDATE())").arg(Minute);

    recordset.open(sql);

    while (recordset.next())
    {
        DeleteTableBOverdateDomain(recordset.fieldValue(0).toString());
    }

    recordset.close();

    return true;
}

bool Database::GetFreeDomain(QString& freeDomain)
{
    AdoRecordset recordset(m_pAdoConnection);

    QString sql = "SELECT Domain FROM GameHallDomainPool WHERE State = 0 ORDER BY UpdateTime ASC";

    recordset.open(sql);

    //获得空闲域名flag
    bool GetFlag = false;

    while (recordset.next())
    {
        freeDomain = recordset.fieldValue(0).toString();

        LogInfo("获取空闲域名:" + freeDomain);

        if (!m_pWidget->checkDomain("wx." + freeDomain))
        {
            LogInfo("域名" + freeDomain + "检测被封");
        }
        else
        {
            LogInfo("域名" + freeDomain + "检测正常");
            GetFlag = true;
            break;
        }
    }

    if (!GetFlag)
    {
        LogNotice("没有空闲域名了");

        return false;
    }

    recordset.close();

    return true;
}

//获取空闲域名 GameHallId ver.
bool Database::GetFreeDomain(QString& freeDomain, QString GameHallId)
{
    AdoRecordset recordset(m_pAdoConnection);

    QString sql = QString("SELECT Domain FROM GameHallDomainPool WHERE State = 0 AND GameHallId = %1 ORDER BY UpdateTime ASC")
            .arg(GameHallId);

    recordset.open(sql);

    //获得空闲域名flag
    bool GetFlag = false;

    while (recordset.next())
    {
        freeDomain = recordset.fieldValue(0).toString();

        LogInfo("获取空闲域名:" + freeDomain);

        if (!m_pWidget->checkDomain("wx." + freeDomain))
        {
            LogInfo("域名" + freeDomain + "检测被封");
        }
        else
        {
            LogInfo("域名" + freeDomain + "检测正常");
            GetFlag = true;
            break;
        }
    }

    if (!GetFlag)
    {
        LogNotice("大厅id:" + GameHallId +"没有空闲域名了");

        return false;
    }

    recordset.close();

    return true;
}

//回收域名
bool Database::RecoverDomain(QString& domain)
{
    AdoRecordset recordset(m_pAdoConnection2);

    domain = domain.right(domain.length() - 3);

    LogInfo("回收域名" + domain);

    int status = 0;

    if (!m_pWidget->checkDomain(domain)) status = 2;

    QString sql = QString("UPDATE GameHallDomainPool SET state = %1, UpdateTime = GETDATE() WHERE domain = '%2'")
            .arg(QString::number(status))
            .arg(domain);

    recordset.open(sql);

    return true;
}

//删除b表数据
bool Database::DeleteTableBOverdateDomain(QString ID)
{
    AdoRecordset recordset(m_pAdoConnection2);

    QString sql = QString("DELETE GameHallReuseDomain WHERE ID = %1").arg(ID);

    recordset.open(sql);

    LogInfo(QString("B表第%1条数据已过期,删除成功").arg(ID));

    return true;
}
