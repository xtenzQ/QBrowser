#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QtWidgets>
#include <QtWebKitWidgets>

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    MyWidget(QWidget *parent = 0);
    ~MyWidget();

private slots:
    void goButtonClickedSlot();
    void linkClickedSlot(QUrl);
    void urlChangedSlot(QUrl);
    void titleChangedSlot(QString);
    void prevButtonClickedSlot();
    void nextButtonClickedSlot();
    void refreshButtonClickedSlot();
    void stopButtonClickedSlot();
    void loadFinishedSlot(bool);
    void loadStartedSlot();

    void closeClickedSlot();
    void settingsClickedSlot();
private:
    QPushButton *prevButton;
    QPushButton *nextButton;
    QPushButton *refreshButton;
    QPushButton *stopButton;
    QPushButton *goButton;

    QLineEdit *addressBar;

    QWebView *webView;

    QLabel *label1;

    QSettings *settings;

};

#endif // WIDGET_H
