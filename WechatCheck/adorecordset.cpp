#include "adorecordset.h"
#include "adoconnection.h"
#include "ado.h"

#include "define.h"
#include "log.h"

AdoRecordset::AdoRecordset(const QVariant & connection, QObject * parent)
    : QObject(parent)
    , dbConnection(connection)
{
    object = new QAxObject(this);
    object->setControl("ADODB.Recordset");

    connect(object, SIGNAL(exception(int, const QString&, const QString&, const QString&)),
            this, SLOT(exception(int, const QString&, const QString&, const QString&)));
}

AdoRecordset::AdoRecordset(AdoConnection * adoConnection, QObject * parent)
    : QObject(parent)
    , dbConnection(adoConnection->connection())
{
    object = new QAxObject(this);
    object->setControl("ADODB.Recordset");

    connect(object, SIGNAL(exception(int, const QString&, const QString&, const QString&)),
            this, SLOT(exception(int, const QString&, const QString&, const QString&)));
}

void AdoRecordset::exception(int code, const QString& source, const QString& desc, const QString& help)
{
    LogNotice("Database exception");
    LogNotice("Code:" + QString::number(code));
    LogNotice("Source:" + source);
    LogNotice("Desc:" + desc);
    LogNotice("Help:" + help);
}

bool AdoRecordset::open(const QString & sql)
{
    HRESULT hr = object->dynamicCall("Open(QString, QVariant, int, int, int)", sql, dbConnection, adOpenStatic, adLockOptimistic, adCmdText).toInt();
    initial = true;
    return SUCCEEDED(hr);
}

int AdoRecordset::recordCount() const
{
    return object->property("RecordCount").toUInt();
}

bool AdoRecordset::next()
{
    bool ret = false;

    fieldNames.clear();
    fieldValues.clear();

    if (object->property("RecordCount").toInt() < 1) return false;

    if (initial)
    {
        ret = move(First);
        initial = false;
    }
    else
        ret = move(Next);

    if (object->property("EOF").toBool()) return false;

    QAxObject * adoFields = object->querySubObject("Fields");
    if (adoFields)
    {
        int count = adoFields->property("Count").toInt();
        for (int i = 0; i < count; i++)
        {
            QAxObject * adoField = adoFields->querySubObject("Item(int)", i);
            if (adoField)
            {
                fieldNames += adoField->property("Name").toString();
                fieldValues += adoField->property("Value");
                ADO_DELETE(adoField);
            }
        }
        ADO_DELETE(adoFields);
    }

    return ret;
}

bool AdoRecordset::move(MoveAction action)
{
    static const char * actions[] =
    {
        "MoveFirst(void)",
        "MoveNext(void)",
        "MovePrevious(void)",
        "MoveLast(void)"
    };

    HRESULT hr = object->dynamicCall(actions[action]).toInt();

    return SUCCEEDED(hr);
}

int AdoRecordset::fieldCount() const
{
    return fieldNames.count();
}

QString AdoRecordset::fieldName(int index) const
{
    if (index < 0 || index >= fieldNames.count()) return QString();

    return fieldNames[index];
}

QVariant AdoRecordset::fieldValue(int index) const
{
    if (index < 0 || index >= fieldValues.count()) return QVariant();

    return fieldValues[index];
}

void AdoRecordset::close()
{
    object->dynamicCall("Close");
}
