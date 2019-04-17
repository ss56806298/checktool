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

    AdoConnection * m_pAdoConnection2;

    Widget * m_pWidget;

    QtRedis * m_pRedis = NULL;
public:
    bool Init(QString& DBName, QString& DBAddr, QString& UserName, QString& DBPass, QString& DBPort);

    QSet<QString> GetWaitCheckDomains();

    bool GetWaitCheckDomains(QMap<QString, QString>&);

    bool GetDomainByGameHallId(QString GameHallId, QString& domain);

    bool ModifyDomain(QString domain);

    bool ModifyDomain(QString domain, QString GameHallId);

    bool ModifyHallId(QString HallId);

    bool CheckTableB(QString Minute);
private:
    //获取空闲域名
    bool GetFreeDomain(QString& freeDomain);
    //获取空闲域名,GameHallId ver.
    bool GetFreeDomain(QString& freeDomain, QString GameHallId);
    //回收域名
    bool RecoverDomain(QString& domain);
    //删除b表数据
    bool DeleteTableBOverdateDomain(QString ID);
};

#endif // DATABASE_H
