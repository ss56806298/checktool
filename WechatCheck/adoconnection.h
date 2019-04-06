#ifndef ADOCONNECTION_H
#define ADOCONNECTION_H

#include <QAxObject>
#include <QVariant>

class QTimer;

class AdoConnection : public QObject
{
    Q_OBJECT
public:
    explicit AdoConnection(QObject *parent = 0);
    bool open(const QString & connectString);
    bool open();
    bool execute(const QString & sql);
    QVariant connection();

    bool isOpen() const;
    void close();

protected slots:
    void exception(int code, const QString & source, const QString & desc, const QString & help);
    void disconnect();

private:
    QAxObject * object;
    QString openString;
    QTimer * timer;

};

#endif // ADOCONNECTION_H
