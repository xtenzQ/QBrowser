#ifndef MYDIALOG_H
#define MYDIALOG_H
#include <QSettings>
#include <QtWidgets>

class MySettings : public QDialog
{
        Q_OBJECT
public:
    MySettings(QSettings *mysettings, QWidget *parent);
private:
    QCheckBox *useDefaultBox;
    QCheckBox *useProxyBox;
    QLineEdit *hostnameEdit;
    QLineEdit *portEdit;
    QLineEdit *userNameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *startpageEdit;
    QSettings *settings;
private slots:
    void okClicked();
    void cancelClicked();
    void defaultClicked();
    void manageButtonsEnabled(bool checked);
    void defaultClicked(bool checked);
signals:
    void settingsChanged();
};

#endif // MYDIALOG_H
