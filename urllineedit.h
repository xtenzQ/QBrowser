#ifndef URLLINEEDIT_H
#define URLLINEEDIT_H

#include <QLineEdit>
#include <QProgressBar>
#include <QWebView>

class QToolButton;

class UrlLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    UrlLineEdit(QWidget *parent = nullptr); 
    void setWebView(QWebView *webView);

public:
    QUrl url() const;
    void setUrl(const QUrl &url);
    void setFavIcon(const QIcon &icon);
    QProgressBar *m_progressBar;
    QMovie *movie;

    void setValue(int setValue);

    void setLoadingIcon();
    void setDeafultIcon();

protected:
    void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *e);

private:
    QToolButton *m_favButton;
    QToolButton *m_clearButton;

    QLinearGradient generateGradient(const QColor &color) const;
    QWebView *m_webView;
    QColor m_defaultBaseColor;

    int maximum = 100;
    int minimum = 0;
    int value = 0;
};

#endif // URLLINEEDIT_H
