#include "mymainwindow.h"
#include "mysettings.h"
#include <QWidget>
#include <QNetworkProxy>
#include <QWebHistory>
#include <QNetworkReply>
#include <QComboBox>
#ifdef Q_OS_WIN32
#define _WIN32_IE 0x0400
#include <shlobj.h>
#undef _WIN32_IE
#endif
#ifdef Q_OS_WIN
#include <windows.h> // for Sleep
#endif
#include <popupmenu.h>

// константа для названия окна браузера
const QString MyMainWindow::browserName = QString("QBrowser");

MyMainWindow::MyMainWindow(QWidget *parent): QWidget(parent) {
    progress = 0;

    setWindowIcon(QIcon(QStringLiteral(":web.png")));
    setWindowTitle(browserName);

    // 1. Убираем границы кнопок
    // 2. Добавляем рамку при наводке
    // 3. Убираем стрелочку для кнопки меню
    this->setStyleSheet("QPushButton::menu-indicator {"
                            "image: url(myindicator.png);"
                            "subcontrol-position: right center;"
                            "subcontrol-origin: padding;"
                            "left: -2px;"
                        "}");


    prevButton = new QPushButton();
    prevButton->setIcon(QIcon(QStringLiteral(":back.png")));
    prevButton->setStyleSheet("border: none; padding: 5px;");

    nextButton = new QPushButton();
    nextButton->setIcon(QIcon(QStringLiteral(":forward.png")));
    nextButton->setStyleSheet("border: none; padding: 5px;");

    refreshButton = new QPushButton();
    refreshButton->setIcon(QIcon(QStringLiteral(":reload.png")));
    refreshButton->setStyleSheet("border: none; padding: 5px;");

    stopButton = new QPushButton();
    stopButton->setIcon(QIcon(QStringLiteral(":stop.png")));
    stopButton->setStyleSheet("border: none; padding: 5px;");

    homeButton = new QPushButton();
    homeButton->setIcon(QIcon(QStringLiteral(":home.png")));
    homeButton->setStyleSheet("border: none; padding: 5px;");

    menuButton = new QPushButton();
    menuButton->setIcon(QIcon(QStringLiteral(":menu.png")));
    menuButton->setStyleSheet("border: none; padding: 5px;");

    prevButton->setEnabled(false);
    nextButton->setEnabled(false);
    refreshButton->setEnabled(false);
    stopButton->setEnabled(false);

    addressBar = new UrlLineEdit();

    webView = new QWebView();
    openSettings();
    if ((settings->value("page/default")).isNull()) {
        settings->setValue("page/default","https://xtenzq.github.io/");
        // записываем несохраненные изменения
        settings->sync();
    }
    // устанавливаем вьюху
    addressBar->setWebView(webView);
    // для правильности обработки клика по ссылке
    webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    // Стартовая страница
    webView->load(QUrl(settings->value("page/default").toString()));
    // Определяем поведение макета по умолчанию для виджета
    webView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    // Сделал элемент, чтобы QMenu не вылезало за окно
    PopupMenu* mainMenu = new PopupMenu(menuButton, this);

    //QMenu *mainMenu = new QMenu(menuButton);
    //mainMenu->setLayoutDirection(Qt::RightToLeft);

    QAction *settingsAction = new QAction(tr("Settings"),this);
    QAction *sourceAction = new QAction(tr("Source code"), this);
    QAction *closeAction = new QAction(tr("Close"),this);

    mainMenu->addAction(settingsAction);
    mainMenu->addAction(sourceAction);
    mainMenu->addAction(closeAction);

    // Устанавливаем нашему QPushButton'y меню
    menuButton->setMenu(mainMenu);


    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);

    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    toolbarLayout->setContentsMargins(5,5,5,5);

    label1 = new QLabel();

    //mainLayout->addWidget(menuBar);

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
    toolbarLayout->addWidget(homeButton);
    toolbarLayout->addWidget(addressBar);
    toolbarLayout->addWidget(menuButton);

    setLayout(mainLayout);

    // Сигналы для прогресс-бара адресной строки
    connect(webView, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
    // connect(webView, SIGNAL(loadProgress(int)), SLOT(adjustOnProgress()));
    // Сигнал, устанавливающий значение прогресса загрузки страницы, который впоследствие передается в addressBar для отрисовки прогресса
    connect(webView, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    connect(webView, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

    // По энтеру переходим по ссылке
    connect(addressBar, SIGNAL(returnPressed()),this,SLOT(goButtonClickedSlot()));

    // Переход по ссылке
    connect(webView, SIGNAL(linkClicked(QUrl)),this,SLOT(linkClickedSlot(QUrl)));
    // Изменение заголовка
    connect(webView, SIGNAL(titleChanged(QString)),this,SLOT(titleChangedSlot(QString)));
    connect(webView, SIGNAL(urlChanged(QUrl)),this,SLOT(urlChangedSlot(QUrl)));
    connect(webView, SIGNAL(loadFinished(bool)),this,SLOT(loadFinishedSlot(bool)));

    connect(webView, SIGNAL(loadStarted()),this,SLOT(loadStartedSlot()));

    connect(prevButton,SIGNAL(clicked(bool)),this,SLOT(prevButtonClickedSlot()));
    connect(nextButton,SIGNAL(clicked(bool)),this,SLOT(nextButtonClickedSlot()));
    connect(refreshButton,SIGNAL(clicked(bool)),this,SLOT(refreshButtonClickedSlot()));
    connect(stopButton,SIGNAL(clicked(bool)),this,SLOT(stopButtonClickedSlot()));
    connect(homeButton,SIGNAL(clicked(bool)),this,SLOT(homeButtonClickedSlot()));

    connect(settingsAction,SIGNAL(triggered(bool)),this,SLOT(settingsClickedSlot()));
    connect(sourceAction,SIGNAL(triggered(bool)),this,SLOT(viewSource()));
    connect(closeAction,SIGNAL(triggered(bool)),this,SLOT(closeClickedSlot()));

    //connect(menuButton,SIGNAL(clicked(bool)),this,SLOT(viewSource()));


}

MyMainWindow::~MyMainWindow() {

}

/**
 * @brief MyWidget::openSettings Инициализирует настройки
 */
void MyMainWindow::openSettings() {
    QString settingsPath = "./";

// ifdef Q_OS_WIN32 - условная компиляция (компилятор распознает ось)
// под линухом не заработает, это да
#ifdef Q_OS_WIN32
    // В commonAppDataPath будет храниться путь к системной папке с данными программ ProgramData
    wchar_t commonAppDataPath[MAX_PATH];

    // SHGetSpecialFolderPath возвращает путь папки, определяемой ее CSIDL (лист постоянных специальных индентификаторов элемента)
    // Первый аргумент - зарезервирован
    // Второй аргумент - указатель на строку с нулевых завершением, которая получает диск и путь к указанной папке
    // (буфер должен содержать не менее MAX_PATH символов)
    // Третий аргумент - CSIDL, идентифицирующий интересующую папку
    // Четвёртый аргумент - указывает, должна ли создаваться папка, если она не существует
    if (SHGetSpecialFolderPath(0, commonAppDataPath, CSIDL_COMMON_APPDATA, FALSE)) {
       settingsPath = QString::fromWCharArray(commonAppDataPath)+QDir::separator()+
               "QBrowser"+QDir::separator();
       if (!QDir(settingsPath).exists()) {
           QDir(QString::fromWCharArray(commonAppDataPath)).mkpath(settingsPath);
           }
       }
#endif

    /**
     * =======================
     * = Структура ini-файла =
     * =======================
     *   [proxy]
     *   useproxy=false
     *   host=172.27.100.5
     *   port=4444
     *   user=student.istu
     *   password=student
     *
     *   [page]
     *   default=http://www.google.ru
     *
     */

    // инициализируем настройки
     settings = new QSettings(settingsPath+"settings.ini",QSettings::IniFormat,this);
     loadSettings();
}

/**
 * @brief MyWidget::loadSettings Считывает настройки для прокси
 */
void MyMainWindow::loadSettings() {
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

/**
 * @brief MyWidget::goButtonClickedSlot Дописывает префикс и переходит по ссылке
 */
void MyMainWindow::goButtonClickedSlot() {
     QString addr = addressBar->text();
     //addressBar->setFavIcon(QIcon(QStringLiteral(":defaultIcon.png")));
     if (!(addr.startsWith("http://") || addr.startsWith("https://"))) {
         addr = "http://"+addr;
        }
     webView->load(addr);
}

/**
 * @brief MyWidget::linkClickedSlot Переходит по ссылке
 * @param url адрес сайта
 */
void MyMainWindow::linkClickedSlot(QUrl url) {
    //addressBar->setFavIcon(QIcon(QStringLiteral(":defaultIcon.png")));
     webView->load(url);
}

/**
 * @brief MyWidget::urlChangedSlot Устанавливает иконки, включает кнопки
 * @param url адрес сайта
 */
void MyMainWindow::urlChangedSlot(QUrl url) {
     // Устанавливаем текст, подгружаем иконки
     addressBar->setDeafultIcon();
     addressBar->setText(url.toString());
     // Включаем кнопки вперед/назад
     prevButton->setEnabled(webView->history()->canGoBack());
     nextButton->setEnabled(webView->history()->canGoForward());
}
void MyMainWindow::titleChangedSlot(QString title) {
     if (title.length() > 0) {
         title = " :: "+title;
        }
     this->title = title;
     title = browserName+" "+title;
     setWindowTitle(title);
}

/**
 * @brief MyWidget::loadFinishedSlot Включает/выключает кнопки
void MyWidget::loadFinishedSlot(bool) {
     refreshButton->setEnabled(true);
     stopButton->setEnabled(false);
}

/**
 * @brief MyWidget::loadStartedSlot Включает кнопки
 */
void MyMainWindow::loadStartedSlot() {
     refreshButton->setEnabled(true);
     stopButton->setEnabled(true);
}

/**
 * @brief MyWidget::prevButtonClickedSlot Переходит на предыдущую страницу в истории
 */
void MyMainWindow::prevButtonClickedSlot() {
     webView->back();
}

/**
 * @brief MyWidget::nextButtonClickedSlot Переходит на следующую страницу в истории
 */
void MyMainWindow::nextButtonClickedSlot() {
     webView->forward();
}

/**
 * @brief MyWidget::refreshButtonClickedSlot Перезагружает страницу
 */
void MyMainWindow::refreshButtonClickedSlot() {
     webView->reload();
}

/**
 * @brief MyWidget::stopButtonClickedSlot Останавливает загрузку страницы
 */
void MyMainWindow::stopButtonClickedSlot() {
     webView->stop();
}

/**
 * @brief MyWidget::closeClickedSlot Закрывает приложение
 */
void MyMainWindow::closeClickedSlot() {
     close();
}

/**
 * @brief MyWidget::homeButtonClickedSlot Загружает домашнюю страницу
 */
void MyMainWindow::homeButtonClickedSlot() {
    // подгружаем из настроек
    webView->load(QUrl(settings->value("page/default").toString()));
}

/*!
 * \brief MyWidget::viewSource Возвращает ответ с URL из адресной строки
 */
void MyMainWindow::viewSource() {
    // QNetworkAccessManager позволяет приложению отсылать запросы/принимать ответы
    QNetworkAccessManager* accessManager = webView->page()->networkAccessManager();
    // Создаем реквест
    QNetworkRequest request(webView->url());
    // Получаем ответ
    QNetworkReply* reply = accessManager->get(request);
    // После завершения обработки, вызывает слот slotSourceDownloaded()
    connect(reply, SIGNAL(finished()), this, SLOT(slotSourceDownloaded()));
}

/**
 * @brief MyWidget::slotSourceDownloaded Отображает продсвеченный исходный код страницы
 */
void MyMainWindow::slotSourceDownloaded() {
    //
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(const_cast<QObject*>(sender()));

    QDialog* myDialog = new QDialog();

    myDialog->setWindowFlags(Qt::Window);
    myDialog->resize(700, 500);

    QTextEdit* textEdit = new QTextEdit(NULL);
    myDialog->setWindowTitle(tr("Source code of ") + (webView->url()).toString());
    myDialog->setAttribute(Qt::WA_DeleteOnClose);
    myDialog->setWindowIcon(QIcon(QStringLiteral(":sourceCode.png")));

    QGridLayout *dialogLayout = new QGridLayout();
    dialogLayout->addWidget(textEdit);

    myDialog->setLayout(dialogLayout);

    textEdit->setAttribute(Qt::WA_DeleteOnClose);
    // Считываем все оставшиеся данные
    // и преобразуем текст TextEdit в текст строки
    textEdit->setPlainText(reply->readAll());

    // Обрабатываем его хайлатером
    m_htmlHightLighter = new HtmlHighLighter(textEdit->document());
    textEdit->setReadOnly(true);

    myDialog->show();
    reply->deleteLater();
}
/*!
  */

/*!
 * \brief MyWidget::settingsClickedSlot Открывает окно настроек
 */
void MyMainWindow::settingsClickedSlot() {
     MySettings *dialog = new MySettings(settings, this);
     dialog->show();
     connect(dialog,SIGNAL(settingsChanged()),this,SLOT(loadSettings()));
}

/**
 * @brief MyWidget::adjustOnProgress Устанавливает прогресс загрузки для адресной строки и иконки
 */
void MyMainWindow::adjustOnProgress() {
    if (progress <= 0 || progress >= 100) {
        addressBar->setDeafultIcon();
        setWindowTitle(browserName + " :: " + webView->title());
        addressBar->setValue(0);
    }
    else {
        setWindowTitle(QString("%1 (%2%)").arg(browserName + " :: " + webView->title()).arg(progress));
        addressBar->setLoadingIcon();
        addressBar->setValue(progress);
    }
}

/**
 * @brief MyWidget::setProgress Устанавливает определенный прогресс для адресной строки
 * @param p величина прогресса
 */
void MyMainWindow::setProgress(int p) {
    progress = p;
    adjustOnProgress();
}
