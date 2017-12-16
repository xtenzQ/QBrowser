#include "HTMLHighLighter.h"
#include <QTextCharFormat>
#include <QBrush>
#include <QColor>

// https://evileg.com/en/post/218/
// Автор кода - Евгений Легоцкой

HtmlHighLighter::HtmlHighLighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
    /**
     * Один блок - это одна строка
        Когда делаем подсветку - при переносе надо проверять состояние (с подсветкой синтаксиса или без неё) предыдущего блока
    */

    // Правило подсветки
    HighlightingRule rule;

    edgeTagFormat.setForeground(QBrush(QColor("#32a9dd")));

    insideTagFormat.setForeground(Qt::blue);
    insideTagFormat.setFontWeight(QFont::Bold);

    openTag = QRegExp("<");
    closeTag = QRegExp(">");

    tagsFormat.setForeground(Qt::darkBlue);
    tagsFormat.setFontWeight(QFont::Bold);

    // Лист паттернов ключевых слов (открывающие теги) - устаревшие и HTML5
    QStringList keywordPatterns;
    keywordPatterns << "<\\ba\\b" << "<\\babbr\\b" << "<\\bacronym\\b" << "<\\baddress\\b" << "<\\bapplet\\b"
                    << "<\\barea\\b" << "<\\barticle\\b" << "<\\baside\\b" << "<\\baudio\\b" << "<\\bb\\b"
                    << "<\\bbase\\b" << "<\\bbasefont\\b" << "<\\bbdi\\b" << "<\\bbdo\\b" << "<\\bbgsound\\b"
                    << "<\\bblockquote\\b" << "<\\bbig\\b" << "<\\bbody\\b" << "<\\bblink\\b" << "<\\bbr\\b"
                    << "<\\bbutton\\b" << "<\\bcanvas\\b" << "<\\bcaption\\b" << "<\\bcenter\\b" << "<\\bcite\\b"
                    << "<\\bcode\\b" << "<\\bcol\\b" << "<\\bcolgroup\\b" << "<\\bcommand\\b" << "<\\bcomment\\b"
                    << "<\\bdata\\b" << "<\\bdatalist\\b" << "<\\bdd\\b" << "<\\bdel\\b" << "<\\bdetails\\b"
                    << "<\\bdfn\\b" << "<\\bdialog\\b" << "<\\bdir\\b" << "<\\bdiv\\b" << "<\\bdl\\b"
                    << "<\\bdt\\b" << "<\\bem\\b" << "<\\bembed\\b" << "<\\bfieldset\\b" << "<\\bfigcaption\\b"
                    << "<\\bfigure\\b" << "<\\bfont\\b" << "<\\bfooter\\b" << "<\\bform\\b" << "<\\bframe\\b"
                    << "<\\bframeset\\b" << "<\\bh1\\b" << "<\\bh2\\b" << "<\\bh3\\b" << "<\\bh4\\b"
                    << "<\\bh5\\b" << "<\\bh6\\b" << "<\\bhead\\b" << "<\\bheader\\b" << "<\\bhgroup\\b"
                    << "<\\bhr\\b" << "<\\bhtml\\b" << "<\\bi\\b" << "<\\biframe\\b" << "<\\bimg\\b"
                    << "<\\binput\\b" << "<\\bins\\b" << "<\\bisindex\\b" << "<\\bkbd\\b" << "<\\bkeygen\\b"
                    << "<\\blabel\\b" << "<\\blegend\\b" << "<\\bli\\b" << "<\\blink\\b" << "<\\blisting\\b"
                    << "<\\bmain\\b" << "<\\bmap\\b" << "<\\bmarquee\\b" << "<\\bmark\\b" << "<\\bmenu\\b"
                    << "<\\bamenuitem\\b" << "<\\bmeta\\b" << "<\\bmeter\\b" << "<\\bmulticol\\b" << "<\\bnav\\b"
                    << "<\\bnobr\\b" << "<\\bnoembed\\b" << "<\\bnoindex\\b" << "<\\bnoframes\\b" << "<\\bnoscript\\b"
                    << "<\\bobject\\b" << "<\\bol\\b" << "<\\boptgroup\\b" << "<\\boption\\b" << "<\\boutput\\b"
                    << "<\\bp\\b" << "<\\bparam\\b" << "<\\bpicture\\b" << "<\\bplaintext\\b" << "<\\bpre\\b"
                    << "<\\bprogress\\b" << "<\\bq\\b" << "<\\brp\\b" << "<\\brt\\b" << "<\\brtc\\b" << "<\\bruby\\b"
                    << "<\\bs\\b" << "<\\bsamp\\b" << "<\\bscript\\b" << "<\\bsection\\b" << "<\\bselect\\b"
                    << "<\\bsmall\\b" << "<\\bsource\\b" << "<\\bspacer\\b" << "<\\bspan\\b" << "<\\bstrike\\b"
                    << "<\\bstrong\\b" << "<\\bstyle\\b" << "<\\bsub\\b" << "<\\bsummary\\b" << "<\\bsup\\b"
                    << "<\\btable\\b" << "<\\btbody\\b" << "<\\btd\\b" << "<\\btemplate\\b" << "<\\btextarea\\b"
                    << "<\\btfoot\\b" << "<\\bth\\b" << "<\\bthead\\b" << "<\\btime\\b" << "<\\btitle\\b"
                    << "<\\btr\\b" << "<\\btrack\\b" << "<\\btt\\b" << "<\\bu\\b" << "<\\bul\\b" << "<\\bvar\\b"
                    << "<\\bvideo\\b" << "<\\bwbr\\b" << "<\\bxmp\\b";

    //
    for (const QString &pattern : keywordPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = tagsFormat;
        // Теги закидываются в качестве правил в вектор стартовых тегов
        startTagRules.append(rule);
    }

    // Лист паттернов ключевых слов (закрывающие теги)
    QStringList keywordPatterns_end;
    keywordPatterns_end << "<!\\bDOCTYPE\\b" << "</\\ba\\b" << "</\\babbr\\b" << "</\\bacronym\\b" << "</\\baddress\\b" << "</\\bapplet\\b"
                        << "</\\barea\\b" << "</\\barticle\\b" << "</\\baside\\b" << "</\\baudio\\b" << "</\\bb\\b"
                        << "</\\bbase\\b" << "</\\bbasefont\\b" << "</\\bbdi\\b" << "</\\bbdo\\b" << "</\\bbgsound\\b"
                        << "</\\bblockquote\\b" << "</\\bbig\\b" << "</\\bbody\\b" << "</\\bblink\\b" << "</\\bbr\\b"
                        << "</\\bbutton\\b" << "</\\bcanvas\\b" << "</\\bcaption\\b" << "</\\bcenter\\b" << "</\\bcite\\b"
                        << "</\\bcode\\b" << "</\\bcol\\b" << "</\\bcolgroup\\b" << "</\\bcommand\\b" << "</\\bcomment\\b"
                        << "</\\bdata\\b" << "</\\bdatalist\\b" << "</\\bdd\\b" << "</\\bdel\\b" << "</\\bdetails\\b"
                        << "</\\bdfn\\b" << "</\\bdialog\\b" << "</\\bdir\\b" << "</\\bdiv\\b" << "</\\bdl\\b"
                        << "</\\bdt\\b" << "</\\bem\\b" << "</\\bembed\\b" << "</\\bfieldset\\b" << "</\\bfigcaption\\b"
                        << "</\\bfigure\\b" << "</\\bfont\\b" << "</\\bfooter\\b" << "</\\bform\\b" << "</\\bframe\\b"
                        << "</\\bframeset\\b" << "</\\bh1\\b" << "</\\bh2\\b" << "</\\bh3\\b" << "</\\bh4\\b"
                        << "</\\bh5\\b" << "</\\bh6\\b" << "</\\bhead\\b" << "</\\bheader\\b" << "</\\bhgroup\\b"
                        << "</\\bhr\\b" << "</\\bhtml\\b" << "</\\bi\\b" << "</\\biframe\\b" << "</\\bimg\\b"
                        << "</\\binput\\b" << "</\\bins\\b" << "</\\bisindex\\b" << "</\\bkbd\\b" << "</\\bkeygen\\b"
                        << "</\\blabel\\b" << "</\\blegend\\b" << "</\\bli\\b" << "</\\blink\\b" << "</\\blisting\\b"
                        << "</\\bmain\\b" << "</\\bmap\\b" << "</\\bmarquee\\b" << "</\\bmark\\b" << "</\\bmenu\\b"
                        << "</\\bamenuitem\\b" << "</\\bmeta\\b" << "</\\bmeter\\b" << "</\\bmulticol\\b" << "</\\bnav\\b"
                        << "</\\bnobr\\b" << "</\\bnoembed\\b" << "</\\bnoindex\\b" << "</\\bnoframes\\b" << "</\\bnoscript\\b"
                        << "</\\bobject\\b" << "</\\bol\\b" << "</\\boptgroup\\b" << "</\\boption\\b" << "</\\boutput\\b"
                        << "</\\bp\\b" << "</\\bparam\\b" << "</\\bpicture\\b" << "</\\bplaintext\\b" << "</\\bpre\\b"
                        << "</\\bprogress\\b" << "</\\bq\\b" << "</\\brp\\b" << "</\\brt\\b" << "</\\brtc\\b" << "</\\bruby\\b"
                        << "</\\bs\\b" << "</\\bsamp\\b" << "</\\bscript\\b" << "</\\bsection\\b" << "</\\bselect\\b"
                        << "</\\bsmall\\b" << "</\\bsource\\b" << "</\\bspacer\\b" << "</\\bspan\\b" << "</\\bstrike\\b"
                        << "</\\bstrong\\b" << "</\\bstyle\\b" << "</\\bsub\\b" << "</\\bsummary\\b" << "</\\bsup\\b"
                        << "</\\btable\\b" << "</\\btbody\\b" << "</\\btd\\b" << "</\\btemplate\\b" << "</\\btextarea\\b"
                        << "</\\btfoot\\b" << "</\\bth\\b" << "</\\bthead\\b" << "</\\btime\\b" << "</\\btitle\\b"
                        << "</\\btr\\b" << "</\\btrack\\b" << "</\\btt\\b" << "</\\bu\\b" << "</\\bul\\b" << "</\\bvar\\b"
                        << "</\\bvideo\\b" << "</\\bwbr\\b" << "</\\bxmp\\b";

    for (const QString &pattern : keywordPatterns_end)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = tagsFormat;
        endTagRules.append(rule);
    }

    multiLineCommentFormat.setForeground(Qt::darkGray);
    commentStartExpression = QRegExp("<!--");
    commentEndExpression = QRegExp("-->");

    quotationFormat.setForeground(Qt::darkGreen);
    quotes = QRegExp("\"");
}

/**
 * @brief HtmlHighLighter::highlightBlock Подсвечивает текст
 * @param text Входной текст
 */
void HtmlHighLighter::highlightBlock(const QString &text) {
    // Устанавливаем состояние текущего блока для начала как без подсветки
    setCurrentBlockState(HtmlHighLighter::None);

    // Если же мы находимся в теге, то считаем стартовый индекс равным нулю (самый первый символ в строке)
    int startIndex = 0;
    // Если мы находимся ни в состояние тега, ни в состоянии кавычек
    if (previousBlockState() != HtmlHighLighter::Tag && previousBlockState() != HtmlHighLighter::Quote) {
        // Пытаемся найти начало следующего тега
        // Если не найдем, будет возвращена -1
        startIndex = openTag.indexIn(text);
    }

    // Записываем состояние предыдущего тега во временную переменную (для подсветки тэгов и кавычек это необходимо)
    int subPreviousTag = previousBlockState();
    // Если находимся в теге или кавычках то начинаем подсветку
    // ИТЕРАЦИЯ ДЛЯ ОДНОГО ТЕГА
    while (startIndex >= 0) {
        // Пытаемся найти конец самого тега (закрывающий символ >)
        int endIndex = closeTag.indexIn(text, startIndex);

        int tagLength;
        // Если конец тега не найден, то устанавливаем состояние блока "в теге"
        if (endIndex == -1) {
            setCurrentBlockState(HtmlHighLighter::Tag);
            // Определяем количество символов, которые надо подсветить
            tagLength = text.length() - startIndex;
        }
        else {
            // В противном случае вычитаем начало тега плюс конец тега плюс длина закрывающего тега
            // Дело в том, что индекс начала тега необходимо учесть (поэтому плюс сравниваемая длина)
            tagLength = endIndex - startIndex + closeTag.matchedLength();
        }

        // Если текущее состояние не равняется тегу
        if (subPreviousTag != HtmlHighLighter::Tag) {
            // Устанавливаем форматирование для <
            // Сначала форматируем начало (один символ)
            setFormat(startIndex, 1, edgeTagFormat);
            // Форматируем то, что внутри тега - форматируем всё поле
            setFormat(startIndex + 1, tagLength - 1, insideTagFormat);
        }
        else {
            // Если находимся в самом начале тега - то от самого начала до конца
            setFormat(startIndex, tagLength, insideTagFormat);
            subPreviousTag = HtmlHighLighter::None;
        }
        // Форматируем символы конца тега >
        setFormat(endIndex, 1, edgeTagFormat);

        /// QUOTES ///////////////////////////////////////

        // В кавычках происходит всё то же самое, за исключением того, что крутимся внутри тега
        int startQuoteIndex = 0;
        // Если не находимся в кавычках с предыдущего блока
        if (previousBlockState() != HtmlHighLighter::Quote) {
            // Пытаемся найти начало кавычек
            startQuoteIndex = quotes.indexIn(text, startIndex);
        }

        // Подсвчиваем все кавычки внутри тега
        while (startQuoteIndex >= 0 && ((startQuoteIndex < endIndex) || (endIndex == -1))) {
            int endQuoteIndex = quotes.indexIn(text, startQuoteIndex + 1);
            int quoteLength;
            if (endQuoteIndex == -1) {
                // Если закрывающая кавычка не найдена, то устанавливаем состояние Quote для блока
                setCurrentBlockState(HtmlHighLighter::Quote);
                quoteLength = text.length() - startQuoteIndex;
            }
            else {
                quoteLength = endQuoteIndex - startQuoteIndex + quotes.matchedLength();
            }

            if ((endIndex > endQuoteIndex) || endIndex == -1) {
                // Подсвечиваем
                setFormat(startQuoteIndex, quoteLength, quotationFormat);
                startQuoteIndex = quotes.indexIn(text, startQuoteIndex + quoteLength);
            }
            else {
                break;
            }
        }
        //////////////////////////////////////////////////
        // Снова находим стартовый индекс следующего тега
        startIndex = openTag.indexIn(text, startIndex + tagLength);
    }

    // КРАЯ ТЕГОВ (начальный тег и конечный тег - без атрибутов, именно сами теги)
    // Обработка цвета самих тегов
    for (const HighlightingRule &rule : startTagRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            // Подсвечиваем нужную длину
            int length = expression.matchedLength();
            // С нужного индекса до конечного индекса
            setFormat(index + 1, length - 1, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    for (const HighlightingRule &rule : endTagRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index + 1, 1, edgeTagFormat);
            setFormat(index + 2, length - 2, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    // КОММЕНТАРИИ
    int startCommentIndex = 0;
    // Если предыдущее состояние тега не является комментарием
    if (previousBlockState() != HtmlHighLighter::Comment) {
        // Пытаемся найти начало комментария
        startCommentIndex = commentStartExpression.indexIn(text);
    }

    // Если комментарий найден
    while (startCommentIndex >= 0) {
        // Ищем конец комментария
        int endCommentIndex = commentEndExpression.indexIn(text, startCommentIndex);
        int commentLength;

        // Если конец не найден
        if (endCommentIndex == -1) {
            // Устанавливаем состояние COMMENT
            setCurrentBlockState(HtmlHighLighter::Comment);
            commentLength = text.length() - startCommentIndex;
        }
        else {
            commentLength = endCommentIndex - startCommentIndex
                            + commentEndExpression.matchedLength();
        }

        setFormat(startCommentIndex, commentLength, multiLineCommentFormat);
        startCommentIndex = commentStartExpression.indexIn(text, startCommentIndex + commentLength);
    }
}
