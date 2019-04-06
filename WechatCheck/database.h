#ifndef DATABASE_H
#define DATABASE_H

#include "define.h"
#include "adoconnection.h"
#include "adorecordset.h"

class Database
{
public:
    Database();
    ~Database();
private:
    AdoConnection * m_pAdoConnection;
public:
    bool Init(QString& DBName, QString& DBAddr, QString& UserName, QString& DBPass, QString& DBPort);

    QSet<QString> GetWaitCheckDomains();
};

#endif // DATABASE_H
