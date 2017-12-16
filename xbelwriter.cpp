#include <QWidget>

#include "xbelwriter.h"
#include "xbelreader.h"

static inline QString yesValue() {
    return QStringLiteral("yes");
}

static inline QString noValue() {
    return QStringLiteral("no");
}

static inline QString titleElement() {
    return QStringLiteral("title");
}

XbelWriter::XbelWriter(const QTreeWidget *treeWidget) : treeWidget(treeWidget) {
    xml.setAutoFormatting(true);
}

bool XbelWriter::writeFile(QIODevice *device) {
    xml.setDevice(device);

    xml.writeStartDocument();
    xml.writeDTD(QStringLiteral("<!DOCTYPE xbel>"));
    xml.writeStartElement(QStringLiteral("xbel"));
    xml.writeAttribute(XbelReader::versionAttribute(), QStringLiteral("1.0"));
    for (int i = 0; i < treeWidget->topLevelItemCount(); i++)
        writeItem(treeWidget->topLevelItem(i));

    xml.writeEndDocument();
    return true;
}

void XbelWriter::writeItem(const QTreeWidgetItem *item) {
    QString tagName = item->data(0, Qt::UserRole).toString();
    if (tagName == QLatin1String("folder")) {
        bool folded = !treeWidget->isItemExpanded(item);
        xml.writeStartElement(tagName);
        xml.writeAttribute(XbelReader::foldedAttribute(), folded ? yesValue() : noValue());
        xml.writeTextElement(titleElement(), item->text(0));
        for (int i = 0; i < item->childCount(); i++)
            writeItem(item->child(i));
        xml.writeEndElement();
    } else if (tagName == QLatin1String("bookmark")) {
        xml.writeStartElement(tagName);
        if (!item->text(1).isEmpty())
            xml.writeAttribute(XbelReader::hrefAttribute(), item->text(1));
        xml.writeTextElement(titleElement(), item->text(0));
        xml.writeEndElement();
    } else if (tagName == QLatin1String("separator")) {
        xml.writeEmptyElement(tagName);
    }
}
