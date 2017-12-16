#ifndef MYHOMEPAGE_H
#define MYHOMEPAGE_H

#include <QSettings>
#include <QtWidgets>

class MyHomepage : public QDialog
{
    Q_OBJECT
public:
    MyHomepage(QWidget *parent = 0);

private:
     QLineEdit *homepageBox;
};

#endif // MYHOMEPAGE_H
