#ifndef MYDIALOG_H
#define MYDIALOG_H
#include <QSettings>
#include <QtWidgets>

class MyDialog : public QDialog
{
        Q_OBJECT
public:
    MyDialog(QWidget *parent);
private:
    QCheckBox *useProxyBox;
    QLineEdit *hostnameEdit;
    QLineEdit *portEdit;
    QLineEdit *userNameEdit;
    QLineEdit *passwordEdit;
    QSettings *settings;
private slots:
    void okClicked();
    void cancelClicked();
    void defaultClicked();
};

#endif // MYDIALOG_H
