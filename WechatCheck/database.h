#ifndef DATABASE_H
#define DATABASE_H

#include "define.h"
#include "adoconnection.h"
#include "adorecordset.h"

class QtRedis;

class Database
{
public:
    Database();
    Database(QtRedis*);
    ~Database();
private:
    AdoConnection * m_pAdoConnection;

    QtRedis * m_pRedis = NULL;
public:
    bool Init(QString& DBName, QString& DBAddr, QString& UserName, QString& DBPass, QString& DBPort);

    QSet<QString> GetWaitCheckDomains();

    bool GetDomainByGameHallId(QString GameHallId, QString& domain);

    bool ModifyDomain(QString domain);

    bool ModifyHallId(QString HallId);
};

#endif // DATABASE_H
