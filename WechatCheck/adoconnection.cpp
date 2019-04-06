#include "adoconnection.h"
#include "ado.h"

#include "define.h"
#include "log.h"

#include <QtDebug>
#include <QTimer>

AdoConnection::AdoConnection(QObject * parent)
    :QObject(parent)
{
    timer = new QTimer(this);
    Q_CHECK_PTR(timer);
    connect(timer, SIGNAL(timeout()), this, SLOT(disconnect()));

    object = new QAxObject(this);
    object->setControl("ADODB.Connection");
    object->setProperty("ConnectionTimeout", 5000);
    connect(object, SIGNAL(exception(int, const QString&, const QString&, const QString&)),
            this, SLOT(exception(int, const QString&, const QString&, const QString&)));
}

void AdoConnection::exception(int code, const QString& source, const QString& desc, const QString& help)
{
    LogNotice("Database exception");
    LogNotice("Code:" + QString::number(code));
    LogNotice("Source:" + source);
    LogNotice("Desc:" + desc);
    LogNotice("Help:" + help);
}

bool AdoConnection::open(const QString& connectString)
{
    if(isOpen()) return true;

    openString = connectString;
    HRESULT hr = object->dynamicCall("Open(QString, QString, QString, int)", connectString, "", "", adConnectUnspecified).toInt();
    return SUCCEEDED(hr);
}

bool AdoConnection::open()
{
    if (openString.isEmpty()) return false;

    bool ret = open(openString);

    if (timer && timer->isActive()) timer->stop();

    return ret;
}

bool AdoConnection::execute(const QString & sql)
{
    if (!open()) return false;

    HRESULT hr = object->dynamicCall("Execute(QString)", sql).toInt();

    return SUCCEEDED(hr);
}

void AdoConnection::disconnect()
{
    if (isOpen()) object->dynamicCall("Close");

    if (timer) timer->stop();
}

void AdoConnection::close()
{
    if (timer)
    {
        if (timer->isActive()) timer->stop();
        timer->start(5000);
    }
    else
        disconnect();
}

QVariant AdoConnection::connection()
{
    return object->asVariant();
}

bool AdoConnection::isOpen() const
{
    return (bool)(object->property("State").toInt() != adStateClosed);
}
