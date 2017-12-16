#ifndef XBELWRITER_H
#define XBELWRITER_H

#include <QXmlStreamWriter>

class QTreeWidget;
class QTreeWidgetItem;

class XbelWriter
{
public:
    explicit XbelWriter(const QTreeWidget *treeWidget);
    bool writeFile(QIODevice *device);
    //XbelWriter();

private:
    void writeItem(const QTreeWidgetItem *item);
    QXmlStreamWriter xml;
    const QTreeWidget *treeWidget;
};


#endif // XBELWRITER_H
