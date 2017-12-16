#ifndef BOOKMARKS_H
#define BOOKMARKS_H

class XbelTree;

class Bookmarks : public QDialog
{
    Q_OBJECT

public:
    Bookmarks(QWidget *parent);

public slots:
    void open();
    void saveAs();
    void about();

private:
    void createMenus();
    QStatusBar *statusBar;
    QMenuBar *menuBar;

    XbelTree *xbelTree;
};

#endif
