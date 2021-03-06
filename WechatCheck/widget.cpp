#include "define.h"
#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle("自动检测");

    m_pTimer = new QTimer();

    m_pTargetTimer = new QTimer();

    m_pModifyTimer = new QTimer();

    m_pCheckTableBTimer = new QTimer();

    m_pNumValidator = new QRegExpValidator(QRegExp("[0-9]+$"));
    ui->checkTimerEdit->setValidator(m_pNumValidator);
    ui->secondEdit->setValidator(m_pNumValidator);
    ui->gameHallIdEdit->setValidator(m_pNumValidator);
    ui->secondEdit2->setValidator(m_pNumValidator);
    ui->modifyHallEdit->setValidator(m_pNumValidator);
    ui->tableBCheckEdit->setValidator(m_pNumValidator);

    connect(ui->startButton, &QPushButton::clicked, this, &Widget::Start);
    connect(ui->targetStartButton, &QPushButton::clicked, this, &Widget::TargetStart);
    connect(ui->modifyStartButton, &QPushButton::clicked, this, &Widget::TargetModify);
    connect(ui->tableBCheckButton, &QPushButton::clicked, this, &Widget::CheckTableB);
    connect(m_pModifyTimer, &QTimer::timeout , this, &Widget::DelayTargetModify);
    connect(m_pTimer, &QTimer::timeout, this, &Widget::DelayAllCheck);
    connect(m_pTargetTimer, &QTimer::timeout, this, &Widget::DelayTargetCheck);
    connect(m_pCheckTableBTimer, &QTimer::timeout, this, &Widget::DelayCheckTableB);
}

Widget::~Widget()
{
    delete ui;

    delete m_pSettings;

    delete m_pDatabase;

    delete m_pNumValidator;

    delete m_pTimer;

    delete m_pTargetTimer;

    delete m_pModifyTimer;

    delete m_pCheckTableBTimer;
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
    if (m_pDatabase != NULL) return true;

    LogInfo("正在初始化数据库链接..");
    QString DBAddr = m_pSettings->value("/DB/Address").toString();
    QString UserName = m_pSettings->value("/DB/UserName").toString();
    QString DBName = m_pSettings->value("/DB/DatabaseName").toString();
    QString DBPass = m_pSettings->value("/DB/Password").toString();
    QString DBPort = m_pSettings->value("/DB/Port").toString();

    m_pDatabase = new Database(this, m_pRedis);

    if(!m_pDatabase->Init(DBName, DBAddr, UserName, DBPass, DBPort))
    {
        LogInfo("数据库初始化失败..");
        return false;
    }

    LogInfo("数据库初始化成功..");
    return true;
}

//初始化redis
bool Widget::InitRedis()
{

    if (m_pRedis != NULL) return true;

    LogInfo("正在初始化Redis链接..");

    QString Address = m_pSettings->value("/Redis/Address").toString();
    int Port = m_pSettings->value("/Redis/Port").toInt();
    QString Password = m_pSettings->value("/Redis/Password").toString();

    m_pRedis = new QtRedis(Address, Port);

    if (!m_pRedis->openConnection())
    {
        LogInfo("Redis初始化失败");

        return false;
    }

    if (Password != "")
    {
        if (!m_pRedis->auth(Password))
        {
            LogInfo("Redis密码错误");

            return false;
        }
    }

    LogInfo("Redis初始化成功");

    return true;
}

//检测启动
bool Widget::Start()
{
    if (!InitRedis()) return false;

    if (!InitDatabase()) return false;

    check();

    return true;
}

//指定大厅的启动
bool Widget::TargetStart()
{
    if (!InitRedis()) return false;

    if (!InitDatabase()) return false;

    ui->targetStartButton->setDisabled(true);

    targetCheck();

    return true;
}

//指定大厅的定时更换
bool Widget::TargetModify()
{
    if (!InitRedis()) return false;

    if (!InitDatabase()) return false;

    ui->modifyStartButton->setDisabled(true);

#ifdef _DEBUG
    QString sec = ui->secondEdit2->text();

    m_iModifyDelaySec = sec.toInt() + 1;

    m_pModifyTimer->start(1000);
#else
    targetModify();
#endif

    return true;
}

//b表检查
bool Widget::CheckTableB()
{
    if (!InitRedis()) return false;

    if (!InitDatabase()) return false;

    m_szMinuteTableB = m_pSettings->value("/Param/TableBCheckMinute").toString();

    ui->tableBCheckButton->setDisabled(true);

    checkTableB();

    return true;
}

bool Widget::DelayAllCheck()
{
    m_iAllCheckDelaySec -= 1;

    ui->allCheckDelayLable->setText(QString::number(m_iAllCheckDelaySec));

    if (m_iAllCheckDelaySec == 0)
    {
        check();

        return true;
    }

    m_pTimer->start(1000);

    return true;
}

bool Widget::DelayTargetCheck()
{
    m_iTargetCheckDelaySec -= 1;

    ui->targetCheckDelayLable->setText(QString::number(m_iTargetCheckDelaySec));

    if (m_iTargetCheckDelaySec == 0)
    {
        targetCheck();

        return true;
    }

    m_pTargetTimer->start(1000);

    return true;
}

bool Widget::DelayTargetModify()
{
    m_iModifyDelaySec -= 1;

    ui->modifyDelayLabel->setText(QString::number(m_iModifyDelaySec));

    if (m_iModifyDelaySec == 0)
    {
        targetModify();

        return true;
    }

    m_pModifyTimer->start(1000);

    return true;
}

bool Widget::DelayCheckTableB()
{
    m_iCheckTableBSec -= 1;

    ui->tableBCheckDelayLabel->setText(QString::number(m_iCheckTableBSec));

    if (m_iCheckTableBSec == 0)
    {
        checkTableB();

        return true;
    }

    m_pCheckTableBTimer->start(1000);

    return true;
}

//b表检查
bool Widget::checkTableB()
{
    if (!m_pDatabase->CheckTableB(m_szMinuteTableB))
    {
        return false;
    }

    QString sec = ui->tableBCheckEdit->text();

    m_iCheckTableBSec = sec.toInt() + 1;

    LogInfo("B表过期数据检查完毕距离下次更换还有" + sec + "秒");

    m_pCheckTableBTimer->start(1000);

    return true;
}

//指定大厅id更换
bool Widget::targetModify()
{
    QString domain;

    if (!m_pDatabase->ModifyHallId(ui->modifyHallEdit->text()))
    {
        return false;
    }

    LogInfo("更换大厅ID" + ui->modifyHallEdit->text());
    QString sec = ui->secondEdit2->text();

    m_iModifyDelaySec = sec.toInt() + 1;

    LogInfo("更换完毕,距离下次更换还有" + sec + "秒");
    m_pModifyTimer->start(1000);

    return true;
}

//指定大厅id检测
bool Widget::targetCheck()
{
    QString domain;

    QString gameHallId = ui->gameHallIdEdit->text();

    if (!m_pDatabase->GetDomainByGameHallId(gameHallId, domain))
    {
        return false;
    }

    LogInfo("检查域名:" + domain);
    if (!checkDomain(domain))
    {
        LogInfo("域名" + domain + "检测已被封禁");
        modifyDomain(domain, gameHallId);
    }
    else
    {
        LogInfo("域名" + domain + "检测正常");
    }

    QString sec = ui->secondEdit->text();

    m_iTargetCheckDelaySec = sec.toInt() + 1;

    LogInfo("检测完毕,距离下次检测还有" + sec + "秒");
    m_pTargetTimer->start(1000);

    return true;
}

//检测
bool Widget::check()
{
    QMap<QString, QString> waitCheckDomainsMap;

    if (m_pDatabase->GetWaitCheckDomains(waitCheckDomainsMap)) return false;

    if (waitCheckDomainsMap.size() == 0)
    {
        LogInfo("无可检查的域名..");

        return true;
    }

    QMapIterator<QString, QString> iter(waitCheckDomainsMap);

    while (iter.hasNext())
    {
        QString gameHallId = iter.key();

        QString domain = iter.value();

        LogInfo("检查域名:" + domain);
        if (!checkDomain(domain))
        {
            LogInfo("域名" + domain + "检测已被封禁");
            modifyDomain(domain, gameHallId);
        }
        else
        {
            LogInfo("域名" + domain + "检测正常");
        }

        QElapsedTimer t;
        t.start();

        while(t.elapsed()<2000)
            QCoreApplication::processEvents();
    }

    QString sec = ui->checkTimerEdit->text();

    m_iAllCheckDelaySec = sec.toInt() + 1;

    LogInfo("检测完毕,距离下次检测还有" + sec + "秒");
    m_pTimer->start(1000);

    return true;
}

//更换域名
bool Widget::modifyDomain(QString domain)
{
    m_pDatabase->ModifyDomain(domain);

    return true;
}

//更换域名 GameHallId ver
bool Widget::modifyDomain(QString domain, QString GameHallId)
{
    m_pDatabase->ModifyDomain(domain, GameHallId);

    return true;
}

//检测域名
bool Widget::checkDomain(QString domain)
{
    QString token;

    if (!getWechatAccessToken(token))
    {
        LogError("获取access_token失败,请检查日志文件");
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

    QString string = QString(response);

    if (string.contains("已停止访问该网页"))
    {
        return false;
    }

    return true;
}

bool Widget::getWechatAccessToken(QString &token)
{
    if (m_pRedis->exists(WECHAT_TOKEN))
    {
        token = m_pRedis->get(WECHAT_TOKEN);

        return true;
    }

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

    m_pRedis->set(WECHAT_TOKEN, token, 7100);

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

    int StatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    response = reply->readAll();

    LogInfoF("返回code:" + QString::number(StatusCode) + " 请求返回:" + QString(response));

    if (StatusCode == 302)
    {
        getHttpResponse("http://" + QString(reply->rawHeader("Location")), response);
    }

    delete pNetManager;

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
