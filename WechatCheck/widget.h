#ifndef WIDGET_H
#define WIDGET_H
#include "database.h"
#include "log.h"

#include <QWidget>
#include <QTextBrowser>
#include <QSettings>
#include <QValidator>

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
    //数字过滤器
    QValidator *m_pNumValidator;
    //倒计时
    QTimer *m_pTimer;

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
    bool checkDomain(QString domain);
    //获取wechat的accesstoken
    bool getWechatAccessToken(QString& token);
    //更换域名
    bool modifyDomain(QString domain);
    //获取http返回
    bool getHttpResponse(QString url, QByteArray& response);
    //获取http返回
    bool postHttpResponse(QString url, QByteArray postData, QByteArray& response);
    //获取json某个字段
    bool getJsonContent(QByteArray jsonString, QString key, QString& value);
    //获取短域名
    bool getShortUrl(QString token, QString domain, QString& shortUrl);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
