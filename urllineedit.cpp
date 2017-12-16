#include "urllineedit.h"
#include <QToolButton>
#include <QUrl>
#include <QPainter>
#include <QProgressBar>
#include <QMovie>
#include <QStyle>
#include <QThread>
#include <QStyleOptionFrame>

/*!
 * \brief UrlLineEdit::UrlLineEdit Кастомная реализация элемента QLineEdit
 * \param parent QWidget
 */
UrlLineEdit::UrlLineEdit(QWidget *parent)
    : QLineEdit(parent),
      //
      m_favButton(new QToolButton(this)),
      m_clearButton(new QToolButton(this))
{
    /*
    * Возможности:
    * 1. Отображение загрузки страницы как при помощи иконки, так и при помощи progressbar
    * 2. Кнопка очищения адресной строки
    * 3. Если соединение защищено (SSL), устанавливается иконка замка
    */
    setFocusPolicy(this->focusPolicy());
    setAttribute(Qt::WA_InputMethodEnabled);
    setSizePolicy(this->sizePolicy());
    setBackgroundRole(this->backgroundRole());
    setMouseTracking(true);
    setAcceptDrops(true);
    setAttribute(Qt::WA_MacShowFocusRect, true);
    QPalette p = this->palette();
    setPalette(p);

    // Устанавливаем иконку очищения адресной строки
    m_clearButton->setIcon(QIcon(QStringLiteral(":exitButton.png")));
    // По умолчанию (когда адресная строка пуста) она неактивна
    m_clearButton->setVisible(false);
    m_clearButton->setCursor(Qt::ArrowCursor);
    QString style(QStringLiteral("QToolButton { border: none; padding: 1px; }"));
    m_clearButton->setStyleSheet(style);
    m_favButton->setStyleSheet(style);
    setStyleSheet(QStringLiteral("QLineEdit { padding-left: %1px; padding-right: %2px; padding-top: 5px; padding-bottom: 5px; } ")
                  .arg(m_clearButton->sizeHint().width())
                  .arg(m_favButton->sizeHint().width()));
    int minIconHeight = qMax(m_favButton->sizeHint().height(), m_clearButton->sizeHint().height());
    setMinimumSize(minimumSizeHint().width() +
                   m_favButton->sizeHint().width() +
                   m_clearButton->sizeHint().width(),
                   qMax(minimumSizeHint().height(), minIconHeight));

    connect(m_clearButton, &QToolButton::clicked, this, &QLineEdit::clear);
    connect(this, &QLineEdit::textChanged, [this](const QString &text) {
        m_clearButton->setVisible(!text.isEmpty() && !isReadOnly());
    });

    // Реализуем анимированную иконки для индикации загрузки страницы
    movie = new QMovie(":load.gif");
    // Будем устанавливать иконку по сигналу frameChanged
    connect(movie,SIGNAL(frameChanged(int)),this,SLOT(setLoadingIcon(int)));
    // Для того, чтобы заставить гифку вращаться бесконечно, сделаем бесконечный цикл
    if (movie->loopCount() != -1)
        // и по мере завершения проигрывания анимации будем крутить ее снова
        connect(movie,SIGNAL(finished()),movie,SLOT(start()));
    movie->start();
}

/*!
 * \brief UrlLineEdit::setWebView Устанавливает webView для того, чтобы впоследствие передавать прогресс для отрисовки progressbar
 * \param webView
 */
void UrlLineEdit::setWebView(QWebView *webView) {
    // Ассерт для отладки, как-нибудь удалю
    Q_ASSERT(!m_webView);
    m_webView = webView;
    //connect(webView, SIGNAL(urlChanged(QUrl)), this, SLOT(webViewUrlChanged(QUrl)));
    //connect(webView, SIGNAL(iconChanged(QIcon)), this, SLOT(webViewIconChanged(QIcon)));
    // Обновляем по мере прогресса
    connect(webView, SIGNAL(loadProgress(int)), this, SLOT(update()));
}

/*!
 * \brief UrlLineEdit::paintEvent Отрисовка элемента UrlLineEdit
 * \param e контекст отрисовки
 */
void UrlLineEdit::paintEvent(QPaintEvent *e) {
    // В начале - отрисовываем сам элемент QLineEdit
    QLineEdit::paintEvent(e);

    /* Сначала хотел вместо иконки красить всю адресную строку в желтый цвет
    QPalette p = palette();
    if (m_webView && m_webView->url().scheme() == QLatin1String("https")) {
        QColor lightYellow(248, 248, 210);
        p.setBrush(QPalette::Base, generateGradient(lightYellow));
    } else {
        p.setBrush(QPalette::Base, palette().color(QPalette::Base));
    }
    setPalette(p);
    */

    QPainter painter(this);
    QStyleOptionFrame panel;
    initStyleOption(&panel);
    QRect backgroundRect = style()->subElementRect(QStyle::SE_LineEditContents, &panel, this);
    if (m_webView) {
        // Получаем прогресс загрузки страницы
        int progress = value;
        // Отрисовываем сам прогрессбар
        // Устанавливаем ему цвет
        QColor loadingColor = QColor(46, 204, 113);
        painter.setBrush(generateGradient(loadingColor));
        // Границы делаем прозрачными
        painter.setPen(Qt::transparent);
        // Ширина - в зависимости от процесса загрузки
        int mid = (backgroundRect.width() + 50) / 100.0f * progress;
        QRect progressRect(backgroundRect.x() - 25, backgroundRect.y() - 5, mid, backgroundRect.height() + 10);
        painter.drawRect(progressRect);
        // Так как элемент перерисовывается и ссылки видно не будет, перерисовываем текст поверх прогрессбара
        painter.setPen(Qt::black);
        // Отрисовываем текст поверх прогресса
        QRect newQR(backgroundRect.x() + 2, backgroundRect.y() + 1, mid, backgroundRect.height());
        painter.drawText(newQR, url().toString());
    }
}

/*!
 * Подстраивает размер кнопок при изменении размера самого элемента
 * \brief UrlLineEdit::resizeEvent
 * \param event
 */
void UrlLineEdit::resizeEvent(QResizeEvent *event) {
    // Отрисовываем оригинальный QLineEdit
    QLineEdit::resizeEvent(event);
    // (для себя) sizeHint() - определяет рекомендованный размер для виджета
    QSize clearButtonSize = m_clearButton->sizeHint();
    // Перемещаем кнопку очищения адресной строки
    m_clearButton->move(rect().right() - clearButtonSize.width(),
                        (rect().bottom() - clearButtonSize.height()) / 2);
    // Перемещаем favIcon
    m_favButton->move(rect().left(), (rect().bottom() - m_favButton->sizeHint().height()) / 2);
}

/*!
 * \brief UrlLineEdit::generateGradient Генерирует верхний/нижний градиент для заданного цвета
 * \param color заданный цвет прогресса адресной строки
 * \return градиент
 */
QLinearGradient UrlLineEdit::generateGradient(const QColor &color) const {
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, m_defaultBaseColor);
    gradient.setColorAt(0.15, color.lighter(120));
    gradient.setColorAt(0.5, color);
    gradient.setColorAt(0.85, color.lighter(120));
    gradient.setColorAt(1, m_defaultBaseColor);
    return gradient;
}

/**
 * @brief UrlLineEdit::setLoadingIcon Устанавливает иконку загрузки страницы
 */
void UrlLineEdit::setLoadingIcon() {
    setFavIcon(QIcon(movie->currentPixmap()));
}

/**
 * @brief UrlLineEdit::setDeafultIcon Отображает иконку в зависимости от типа соединения (защищенного или нет)
 */
void UrlLineEdit::setDeafultIcon() {
    // Если соединение - защищенное
    if (m_webView && m_webView->url().scheme() == QLatin1String("https")) {
        setFavIcon(QIcon(QStringLiteral(":ssl.png")));
    } else {
        setFavIcon(QIcon(QStringLiteral(":notssl.png")));
    }

}

/**
 * @brief UrlLineEdit::url Возвращает URL
 * @return URL
 */
QUrl UrlLineEdit::url() const {
    return QUrl::fromUserInput(text());
}

/*!
 * Устанавливаем URL
 * \brief UrlLineEdit::setUrl
 * \param url
 */
void UrlLineEdit::setUrl(const QUrl &url) {
    setText(url.toString());
    setCursorPosition(0);
}

/*!
 * Устанавливаем favIcon
 * \brief UrlLineEdit::setFavIcon
 * \param icon
 */
void UrlLineEdit::setFavIcon(const QIcon &icon) {
    QPixmap pixmap = icon.pixmap(16, 16);
    m_favButton->setIcon(pixmap);
}

/*!
 * Устанавливаем прогресс для прогресс бара
 * \brief UrlLineEdit::setValue
 * \param setValue
 */
void UrlLineEdit::setValue(int setValue) {
    // Один из самых важных методов
    value = setValue;
    setValue = 0;
}
