#ifndef WIDGET_H
#define WIDGET_H
#include "database.h"
#include "log.h"
#include "qtredis.h"

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
    //指定检测倒计时
    QTimer *m_pTargetTimer;
    //指定更换倒计时
    QTimer *m_pModifyTimer;
    //检测b表倒计时
    QTimer *m_pCheckTableBTimer;
    //全局检测倒计时
    int m_iAllCheckDelaySec;
    //指定检测倒计时
    int m_iTargetCheckDelaySec;
    //指定更换倒计时秒数
    int m_iModifyDelaySec;
    //指定检测b表倒计时
    int m_iCheckTableBSec;

//系统服务对象
protected:
    //DB
    Database * m_pDatabase = NULL;
    //redis
    QtRedis * m_pRedis = NULL;
    //B表检查时间间隔
    QString m_szMinuteTableB;


//界面调用
    //开始运行
    bool Start();
    //指定大厅的开始运行
    bool TargetStart();
    //指定大厅更换
    bool TargetModify();
    //检查b表
    bool CheckTableB();
    //全局检测倒计时
    bool DelayAllCheck();
    //指定检测倒计时
    bool DelayTargetCheck();
    //指定大厅更换倒计时
    bool DelayTargetModify();
    //检测b表倒计时
    bool DelayCheckTableB();

//系统函数
public:
    //初始化配置
    bool Init();
private:
    //初始化log
    bool InitLog();
    //初始化DB
    bool InitDatabase();
    //初始化redis
    bool InitRedis();

public:
    //检测
    bool check();
    //指定域名检测
    bool targetCheck();
    //指定大厅更换
    bool targetModify();
    //b表检查
    bool checkTableB();

    //检测域名
    bool checkDomain(QString domain);
    //获取wechat的accesstoken
    bool getWechatAccessToken(QString& token);
    //更换域名
    bool modifyDomain(QString domain);
    //更换域名 指定GameHallId ver
    bool modifyDomain(QString domain, QString GameHallId);
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
