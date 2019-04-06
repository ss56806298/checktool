#include "database.h"

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
