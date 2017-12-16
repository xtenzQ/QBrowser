#ifndef MYBOOKMARKS_H
#define MYBOOKMARKS_H
#include <QtWidgets>
#include <QModelIndex>

class MyBookmarks : public QDialog
{
    Q_OBJECT

    public:
        MyBookmarks(QWidget *parent = 0);

    public slots:
        void updateActions();

    private slots:
        void insertChild();
        bool insertColumn();
        void insertRow();
        bool removeColumn();
        void removeRow();
};

#endif // MYBOOKMARKS_H
