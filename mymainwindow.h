#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QSettings>
#include <QtWidgets>
#include <QtWebKitWidgets>
#include <urllineedit.h>
#include <htmlhighlighter.h>
#include <myhomepage.h>

class MyMainWindow : public QWidget
{
    Q_OBJECT

public:
    MyMainWindow(QWidget *parent = 0);
    ~MyMainWindow();

    const static QString browserName;
private slots:
    void goButtonClickedSlot();
    void linkClickedSlot(QUrl);
    void urlChangedSlot(QUrl);
    void titleChangedSlot(QString);
    void prevButtonClickedSlot();
    void nextButtonClickedSlot();
    void refreshButtonClickedSlot();
    void stopButtonClickedSlot();
    void loadStartedSlot();
    void homeButtonClickedSlot();

    void closeClickedSlot();
    void settingsClickedSlot();
    void loadSettings();

    void adjustOnProgress();
    void setProgress(int p);

protected slots:
    void viewSource();
    void slotSourceDownloaded();


private:
    // Вкладки
    QTabWidget *tabWidget;
    // Кнопки
    QPushButton *prevButton;
    QPushButton *nextButton;
    QPushButton *refreshButton;
    QPushButton *stopButton;
    QPushButton *goButton;
    QPushButton *homeButton;
    QPushButton *menuButton;
    //
    QPushButton *showBookmarks;
    //
    UrlLineEdit *addressBar;
    QWebView *webView;
    QLabel *label1;

    MyHomepage *dialog;

    QSettings *settings;
    void openSettings();
    void qSleep(int ms);

    HtmlHighLighter *m_htmlHightLighter;
    int progress;
    QString title;
};

#endif // WIDGET_H
