#include "database.h"
#include "define.h"
#include "log.h"

#include <QDebug>

Database::Database()
{
    m_pAdoConnection = new AdoConnection;
}

Database::~Database()
{
    delete m_pAdoConnection;
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

    return true;
}

QSet<QString> Database::GetWaitCheckDomains()
{
    QSet<QString> domains;

    QString sql = "SELECT AppBindDomain FROM GameHallConfig WHERE GameHallId > 3";
    AdoRecordset recordset(m_pAdoConnection);

    recordset.open(sql);

    while(recordset.next())
    {
        int count = recordset.fieldCount();

        for(int i = 0; i < count; i++)
        {
            QString value = recordset.fieldValue(i).toString();

            if (!domains.contains(value)) domains.insert(value);
        }
    }

    return domains;
}

bool Database::ModifyDomain(QString domain)
{
    AdoRecordset recordset(m_pAdoConnection);

    QString sql = "SELECT Domain FROM GameHallDomainPool WHERE State = 0 ORDER BY UpdateTime ASC";

    recordset.open(sql);

    if (!recordset.next())
    {
        LogNotice("域名池为空,没有闲置的域名了");

        return false;
    }

    QString newDomain = recordset.fieldValue(0).toString();

    recordset.close();

    sql = QString("SELECT a.GameHallId, a.AppBindDomain, b.Code FROM GameHallConfig a, GameHallInfo b WHERE a.GameHallId = b.GameHallId AND a.AppBindDomain = '%1'")
            .arg(domain);

   recordset.open(sql);
   QStringList sqlList;

   while (recordset.next())
   {
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

    return true;
}
