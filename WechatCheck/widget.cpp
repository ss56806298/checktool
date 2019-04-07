#include "define.h"
#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_pTimer = new QTimer();

    m_pNumValidator = new QRegExpValidator(QRegExp("[0-9]+$"));
    ui->checkTimerEdit->setValidator(m_pNumValidator);

    connect(ui->startButton, &QPushButton::clicked, this, &Widget::Start);
}

Widget::~Widget()
{
    delete ui;

    delete m_pSettings;
}

bool Widget::Init()
{   //log提示框
    m_pLogBrowser = ui->logBrowser;

    Log::SetLogPath();
    Log::SetLogBrowser(m_pLogBrowser);

    //ini设置
    m_pSettings = new QSettings("./settings.ini", QSettings::IniFormat);

    return true;
}

//初始化数据库
bool Widget::InitDatabase()
{
    LogInfo("正在初始化数据库链接..");
    QString DBAddr = m_pSettings->value("/DB/Address").toString();
    QString UserName = m_pSettings->value("/DB/UserName").toString();
    QString DBName = m_pSettings->value("/DB/DatabaseName").toString();
    QString DBPass = m_pSettings->value("/DB/Password").toString();
    QString DBPort = m_pSettings->value("/DB/Port").toString();

    m_pDatabase = new Database();

    if(!m_pDatabase->Init(DBName, DBAddr, UserName, DBPass, DBPort))
    {
        LogInfo("数据库初始化失败..");
        return false;
    }

    LogInfo("数据库初始化成功..");
    return true;
}

//检测启动
bool Widget::Start()
{
    if (!InitDatabase()) return false;

    check();

    return true;
}

//检测
bool Widget::check()
{
    QSet<QString> domains = m_pDatabase->GetWaitCheckDomains();

    if (domains.size() == 0)
    {
        LogInfo("无可检查的域名..");

        return true;
    }

    foreach(const QString & domain, domains)
    {
        LogInfo("检查域名:" + domain);
        if (!checkDomain(domain))
        {
            LogInfo("域名" + domain + "检测已被封禁");
            modifyDomain(domain);
        }
        else
        {
            LogInfo("域名" + domain + "检测正常");
        }
    }

    connect(m_pTimer, &QTimer::timeout , this, &Widget::check);

    QString sec = ui->checkTimerEdit->text();

    LogInfo("检测完毕,距离下次检测还有" + sec + "秒");
    m_pTimer->start(sec.toInt() * 1000);

    return true;
}

//更换域名
bool Widget::modifyDomain(QString domain)
{
    m_pDatabase->ModifyDomain(domain);

    return true;
}

//检测域名
bool Widget::checkDomain(QString domain)
{
    QString token;

    if (!getWechatAccessToken(token))
    {
        LogError("获取access_token失败,请检查配置文件中的wechat appid和secret");
        return true;
    }

    QString shortUrl;

    if (!getShortUrl(token, domain, shortUrl))
    {
        LogError("获取短链接失败");
        return true;
    }

    QByteArray response;

    if (!getHttpResponse(shortUrl, response))
    {
        LogError("短链接请求失败");
        return true;
    }

    QString string = response;

    if (string.contains("已停止访问该网页"))
    {
        return false;
    }

    return true;
}

bool Widget::getWechatAccessToken(QString &token)
{
    QString appid = m_pSettings->value("/Wechat/AppID").toString();

    QString appsecret = m_pSettings->value("Wechat/AppSecret").toString();

    QString url = QString("https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=%1&secret=%2")
            .arg(appid)
            .arg(appsecret);

    QByteArray response;

    if (!getHttpResponse(url, response))
    {
        return false;
    }

    if (!getJsonContent(response, "access_token", token))
    {
        return false;
    }

    return true;
}

bool Widget::getHttpResponse(QString url, QByteArray &response)
{
    QNetworkAccessManager * pNetManager = new QNetworkAccessManager;

    QNetworkReply* reply;

    QNetworkRequest request;

    QEventLoop loop;

    LogInfoF("请求url:" + url);

    request.setUrl(QUrl(url));

    connect(pNetManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    reply = pNetManager->get(request);

    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        LogNotice("网络异常," + reply->errorString());
        return false;
    }

    response = reply->readAll();

    LogInfoF("请求返回:" + response);

    return true;
}

bool Widget::postHttpResponse(QString url, QByteArray postData, QByteArray &response)
{
    QNetworkAccessManager * pNetManager = new QNetworkAccessManager;

    QNetworkReply* reply;

    QNetworkRequest request;

    QEventLoop loop;

    LogInfoF("请求url:" + url);

    request.setUrl(QUrl(url));
    request.setRawHeader("Content-Type", "application/json");

    connect(pNetManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    reply = pNetManager->post(request, postData);

    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        LogNotice("网络异常," + reply->errorString());
        return false;
    }

    response = reply->readAll();

    LogInfoF("请求返回:" + response);

    return true;
}

bool Widget::getJsonContent(QByteArray jsonString, QString key, QString &value)
{
    QJsonParseError error;

    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString, &error);

    if (error.error != QJsonParseError::NoError || !jsonDocument.isObject())
    {
        LogErrorF("Json analyse error");
        return false;
    }

    QVariantMap result = jsonDocument.toVariant().toMap();

    QString tempValue = result[key].toString();

    LogInfoF(key + ":" + tempValue);

    if (tempValue == "")
    {
        return false;
    }

    value = tempValue;

    return true;
}

bool Widget::getShortUrl(QString token, QString domain, QString &shortUrl)
{
    QString url = QString("https://api.weixin.qq.com/cgi-bin/shorturl?access_token=%1")
            .arg(token);

    QVariantMap rawJson;

    rawJson["action"] = "long2short";
    rawJson["long_url"] = domain;

    QByteArray postData = QJsonDocument::fromVariant(rawJson).toJson(QJsonDocument::Compact);

    QByteArray response;

    if (!postHttpResponse(url, postData, response))
    {
        return false;
    }

    if (!getJsonContent(response, "short_url", shortUrl))
    {
        return false;
    }

    return true;
}
