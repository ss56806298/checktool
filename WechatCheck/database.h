#ifndef DATABASE_H
#define DATABASE_H

#include "define.h"
#include "adoconnection.h"
#include "adorecordset.h"


class QtRedis;

class Widget;

class Database
{
public:
    Database(Widget *);
    Database(Widget *, QtRedis*);
    ~Database();
private:
    AdoConnection * m_pAdoConnection;

    Widget * m_pWidget;

    QtRedis * m_pRedis = NULL;
public:
    bool Init(QString& DBName, QString& DBAddr, QString& UserName, QString& DBPass, QString& DBPort);

    QSet<QString> GetWaitCheckDomains();

    bool GetDomainByGameHallId(QString GameHallId, QString& domain);

    bool ModifyDomain(QString domain);

    bool ModifyHallId(QString HallId);
private:
    bool GetFreeDomain(QString& freeDomain);

    bool RecoverDomain(QString& domain);
};

#endif // DATABASE_H
