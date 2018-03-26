# QBrowser

## Contents

1. [Before we start](#before-we-start)
2. [IDEs and plugins used](#ides-and-plugins-used)
3. [Custom elements](#custom-elements)
4. [References](#references)

## Before we start

The application is developed as the part of the project for programming course at [Irkutsk National Research Techincal University](http://www.istu.edu/eng/).

## IDEs and plugins used
- Qt Creator Community Edition ([Download](https://www.qt.io/download))
- QtWebkit ([Download](https://github.com/wkhtmltopdf/qtwebkit))


## Custom elements

There are a few custom elements I created to follow the task:
1. Custom address bar displaying loading progress
2. Source code of the page [[1](#references)]
3. Settings window

### Custom address bar
```C++
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
```
# References
1. [Qt/C++ - Lesson 058. Syntax highlighting of HTML code in QTextEdit](https://evileg.com/en/post/218/) by Eugene Legotskoy
