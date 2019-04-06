#ifndef WIDGET_H
#define WIDGET_H
#include "database.h"
#include "log.h"

#include <QWidget>
#include <QTextBrowser>
#include <QSettings>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

//界面对象
protected:
    //提示框
    QTextBrowser * m_pLogBrowser;
    //配置文件
    QSettings * m_pSettings;

//系统服务对象
protected:
    //DB
    Database * m_pDatabase;

//界面调用
    //开始运行
    bool Start();

//系统函数
public:
    //初始化配置
    bool Init();
private:
    //初始化log
    bool InitLog();
    //初始化DB
    bool InitDatabase();

private:
    //检测
    bool check();
    //检测域名
    bool checkDomain(QString domain, QNetworkAccessManager * pNetManager);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
