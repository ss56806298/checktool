#include "define.h"
#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

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

    QNetworkAccessManager * pNetManager = new QNetworkAccessManager;

    foreach(const QString & domain, domains)
    {
        checkDomain(domain, pNetManager);
    }

    delete pNetManager;

    return true;
}

bool Widget::checkDomain(QString domain, QNetworkAccessManager * pNetManager)
{
    LogInfo("检查域名:" + domain);

    QNetworkReply * reply;

    QString boundary = "WebKitFormBoundaryMpB6TGuCBoHAKJej";

    QEventLoop loop;

    connect(pNetManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    QUrl url = QUrl("http://api.weixinclup.com/index/checkurl.html");

    QNetworkRequest request;

    request.setRawHeader("X-Requested-With", "XMLHttpRequest");
    request.setRawHeader("Content-Type",QString("multipart/form-data; boundary=" + boundary).toUtf8());

    request.setUrl(url);

    QByteArray postData;
    postData.append("--" + boundary + "\r\n");
    postData.append("Content-Disposition:       form-data; name=\"url\"\r\n");
    postData.append("\r\n");
    postData.append(domain);

    reply = pNetManager->post(request, postData);

    loop.exec();

    LogInfo(reply->readAll());

    return true;
}
