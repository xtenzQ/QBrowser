#include "mywidget.h"
#include "mydialog.h"
//#include <QWidget>
#ifdef Q_OS_WIN32
#define _WIN32_IE 0x0400
#include <shiobj.h>
#undef _WIN32_IE
#endif

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("MyBrowser"));

    prevButton = new QPushButton(tr("<<"));
    nextButton = new QPushButton(tr(">>"));
    refreshButton = new QPushButton(tr("Refresh"));
    stopButton = new QPushButton(tr("Stop"));
    goButton = new QPushButton(tr("Go!"));

    goButton->setStyleSheet("QPushButton:pressed { border: 3px dotted green; padding: 10px; }");

    prevButton->setEnabled(false);
    nextButton->setEnabled(false);
    refreshButton->setEnabled(false);
    stopButton->setEnabled(false);

    addressBar = new QLineEdit();

    webView = new QWebView();
    //webView->page()->networkAccessManager()->setProxy(QNetworkProxy(QNetworkProxy::HttpProxy,"172.27.100.5",4444,
    //                                                                "student.istu","student"));
    webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    //QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
    //QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    //QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanOpenWindows,false);
    //QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanCloseWindows,false);
    //QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard,false);

    webView->load(QUrl("http://txt.newsru.com"));
    webView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QMenuBar *menuBar = new QMenuBar();
    menuBar->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);
    QMenu *mainMenu = new QMenu(tr("Menu"));
    menuBar->addMenu(mainMenu);
    QAction *settingsAction = new QAction(tr("Settings"),this);
    QAction *closeAction = new QAction(tr("Close"),this);
    mainMenu->addAction(settingsAction);
    mainMenu->addAction(closeAction);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);

    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    //toolbarLayout->setContentsMargins(5,5,5,5);

    label1 = new QLabel();

    mainLayout->addWidget(menuBar);

    QVBoxLayout *innerLayout = new QVBoxLayout();
    innerLayout->setContentsMargins(5,0,5,5);

    innerLayout->addLayout(toolbarLayout);
    innerLayout->addWidget(webView);
    innerLayout->addWidget(label1);
    mainLayout->addLayout(innerLayout);

    toolbarLayout->addWidget(prevButton);
    toolbarLayout->addWidget(nextButton);
    toolbarLayout->addWidget(refreshButton);
    toolbarLayout->addWidget(stopButton);
    toolbarLayout->addWidget(addressBar);
    toolbarLayout->addWidget(goButton);

    setLayout(mainLayout);

    connect(goButton,SIGNAL(clicked(bool)),this,SLOT(goButtonClickedSlot()));
    connect(addressBar,SIGNAL(returnPressed()),this,SLOT(goButtonClickedSlot()));
    connect(webView,SIGNAL(linkClicked(QUrl)),this,SLOT(linkClickedSlot(QUrl)));
    connect(webView,SIGNAL(titleChanged(QString)),this,SLOT(titleChangedSlot(QString)));
    connect(webView,SIGNAL(urlChanged(QUrl)),this,SLOT(urlChangedSlot(QUrl)));
    connect(webView,SIGNAL(loadFinished(bool)),this,SLOT(loadFinishedSlot(bool)));
    connect(webView,SIGNAL(loadStarted()),this,SLOT(loadStartedSlot()));

    connect(prevButton,SIGNAL(clicked(bool)),this,SLOT(prevButtonClickedSlot()));
    connect(nextButton,SIGNAL(clicked(bool)),this,SLOT(nextButtonClickedSlot()));
    connect(refreshButton,SIGNAL(clicked(bool)),this,SLOT(refreshButtonClickedSlot()));
    connect(stopButton,SIGNAL(clicked(bool)),this,SLOT(stopButtonClickedSlot()));


    connect(settingsAction,SIGNAL(triggered(bool)),this,SLOT(settingsClickedSlot()));
    connect(closeAction,SIGNAL(triggered(bool)),this,SLOT(closeClickedSlot()));

    settings->value("proxy/host").toString(),
    settings->value("proxy/port").toInt(),
    settings->value("proxy/user").toString(),
    settings->value("proxy/password").toString()));

}

MyWidget::~MyWidget()
{

}

void MyWidget::openSettings() {
    QString settingsPath = "./";
#ifdef Q_OS_WIN32
    wchar_t commonAppDataPath(MAX_PATH);
    if (SHGetSpecialFolderPath(0, commonAppDataPath, CSIDL_COMMON_APPDATA, FALSE)) {
        settingsPath = QString::fromWCharArray(commonAppDataPath)+QDir::separator()+"MyBrowser"+QDir::separator();
        if (!QDir(SettingsPath).exists()) {
            QDir(QString::fromWCharArray(commonAppDataPath)).mkpath(settingsPath);
        }
    }
#endif
    settings = new QSettings(QSettings::IniFormat, settingsPath);
    loadSettings();
}

void MyWidget::loadSettings() {
    if (settings->value("proxy/useproxy").toBool()) {
        webView->page()->networkAccessManager()->setProxy(QNetworkProxy(QNetworkProxy::HttpProxy,
                                                                        settings->value("proxy/host").toString(),
                                                                        settings->value("proxy/port").toInt(),
                                                                        settings->value("proxy/user").toString(),
                                                                        settings->value("proxy/password").toString()));
    }
    else {
        webView->page()->networkAccessManager()->setProxy(QNetworkProxy(QNetworkProxy::NoProxy));
    }
}

void MyWidget::goButtonClickedSlot() {
     QString addr = addressBar->text();
     if (!(addr.startsWith("http://") || addr.startsWith("https://"))) {
         addr = "http://"+addr;
        }
     webView->load(addr);
}

void MyWidget::linkClickedSlot(QUrl url) {
     webView->load(url);
}

void MyWidget::urlChangedSlot(QUrl url) {
     addressBar->setText(url.toString());
     prevButton->setEnabled(webView->history()->canGoBack());
     nextButton->setEnabled(webView->history()->canGoForward());
}
void MyWidget::titleChangedSlot(QString title) {
     if (title.length() > 0) {
         title = " :: "+title;
        }
     title = tr("MyBrowser")+title;
     setWindowTitle(title);
}
void MyWidget::loadFinishedSlot(bool) {
     refreshButton->setEnabled(true);
     stopButton->setEnabled(false);
}
void MyWidget::loadStartedSlot() {
     refreshButton->setEnabled(true);
     stopButton->setEnabled(true);
}

void MyWidget::prevButtonClickedSlot() {
     webView->back();
}
void MyWidget::nextButtonClickedSlot() {
     webView->forward();
}
void MyWidget::refreshButtonClickedSlot() {
     webView->reload();
}
void MyWidget::stopButtonClickedSlot() {
     webView->stop();
}
void MyWidget::closeClickedSlot() {
     close();
}
void MyWidget::settingsClickedSlot() {
     MyDialog *dialog = new MyDialog(this);
     dialog->show();
}
