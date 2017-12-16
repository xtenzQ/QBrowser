#ifndef HTMLHIGHLIGHTER_H
#define HTMLHIGHLIGHTER_H

#include <QSyntaxHighlighter>

QT_BEGIN_NAMESPACE
class QTextDocument;
class QTextCharFormat;
QT_END_NAMESPACE

class HtmlHighLighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit HtmlHighLighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    // Состояние подсветки, в которой находится текстовый блок на момент его закрытия
    enum States {
        None,       // Без подсветки
        Tag,        // Подсветка внутри тега
        Comment,    // Внутри комментария
        Quote       // Внутри кавычек, которые внутри тега
    };

    // Структура правил (шаблон + форматирование)
    struct HighlightingRule
    {
        // Шаблон
        QRegExp pattern;
        // Форматирование
        QTextCharFormat format;
    };

    // Правила форматирования для открывающих тегов
    QVector<HighlightingRule> startTagRules;
    // Правила форматирования для закрывающих тегов
    QVector<HighlightingRule> endTagRules;
    // Символ открытия тега - "<"
    QRegExp openTag;
    // Символ закрытия тега - ">"
    QRegExp closeTag;
    // Форматирование символов < и >
    QTextCharFormat edgeTagFormat;
    // Форматирование текста внутри тега
    QTextCharFormat insideTagFormat;

    // Регулярное выражение начала комментария
    QRegExp commentStartExpression;
    // Регулярное выражения закрытия комментария
    QRegExp commentEndExpression;
    // Форматирование текста внутри комментария
    QTextCharFormat multiLineCommentFormat;

    // Регулярное выражения для текста в кавычках внутри тега
    QRegExp quotes;
    // Форматирование текста в кавычках внутри тега
    QTextCharFormat quotationFormat;
    // Форматирование самих тегов
    QTextCharFormat tagsFormat;
};

#endif // HTMLHIGHLIGHTER_H
