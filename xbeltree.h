#ifndef XBELTREE_H
#define XBELTREE_H

#include <QDomDocument>
#include <QIcon>
#include <QTreeWidget>

class XbelTree : public QTreeWidget {
    Q_OBJECT

public:
    XbelTree(QWidget *parent = 0);

    bool read(QIODevice *device);
    bool write(QIODevice *device) const;

protected:
#if !defined(QT_NO_CONTEXTMENU) && !defined(QT_NO_CLIPBOARD)
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
#endif

private slots:
    void updateDomElement(const QTreeWidgetItem *item, int column);

private:
    void parseFolderElement(const QDomElement &element,
                            QTreeWidgetItem *parentItem = 0);
    QTreeWidgetItem *createItem(const QDomElement &element,
                                QTreeWidgetItem *parentItem = 0);

    QDomDocument domDocument;
    QIcon folderIcon;
    QIcon bookmarkIcon;
};

#endif
