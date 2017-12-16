#include "mysettings.h"

MySettings::MySettings(QSettings *mysettings, QWidget *parent=0):QDialog(parent) {
    settings = mysettings;
    setModal(true);
    setWindowTitle(tr("Browser settings"));
    setAttribute(Qt::WA_DeleteOnClose);

    // Поля / чекбоксы
    useDefaultBox = new QCheckBox();
    useProxyBox = new QCheckBox();
    hostnameEdit = new QLineEdit();
    startpageEdit = new QLineEdit();
    portEdit = new QLineEdit();
    userNameEdit = new QLineEdit();
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);

    // Валидатор IP-адреса
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    hostnameEdit->setValidator(ipValidator);

    // Кнопки
    QPushButton *okButton = new QPushButton(tr("OK"));
    QPushButton *cancelButton = new QPushButton(tr("Cancel"));

    QGridLayout *mainLayout = new QGridLayout();

    mainLayout->addWidget(new QLabel(tr("Host")),0,0);
    mainLayout->addWidget(new QLabel(tr("Port")),1,0);
    mainLayout->addWidget(new QLabel(tr("Username")),2,0);
    mainLayout->addWidget(new QLabel(tr("Password")),3,0);
    mainLayout->addWidget(new QLabel(tr("Proxy enabled")),4,0);
    mainLayout->addWidget(new QLabel(tr("Homepage")),5,0);
    mainLayout->addWidget(new QLabel(tr("Default")),6,0);

    mainLayout->addWidget(hostnameEdit,0,1);
    mainLayout->addWidget(portEdit,1,1);
    mainLayout->addWidget(userNameEdit,2,1);
    mainLayout->addWidget(passwordEdit,3,1);
    mainLayout->addWidget(useProxyBox,4,1);
    mainLayout->addWidget(startpageEdit,5,1);
    mainLayout->addWidget(useDefaultBox,6,1);

    mainLayout->addWidget(okButton,7,0);
    mainLayout->addWidget(cancelButton,7,1);

    setLayout(mainLayout);

    hostnameEdit->setEnabled(false);
    portEdit->setEnabled(false);
    userNameEdit->setEnabled(false);
    passwordEdit->setEnabled(false);

    connect(okButton,SIGNAL(clicked(bool)),this,SLOT(okClicked()));
    connect(cancelButton,SIGNAL(clicked(bool)),this,SLOT(cancelClicked()));

    connect(useProxyBox, SIGNAL(clicked(bool)), this, SLOT(manageButtonsEnabled(bool)));
    connect(useDefaultBox, SIGNAL(clicked(bool)), this, SLOT(defaultClicked(bool)));

    useProxyBox->setChecked(settings->value("proxy/useproxy").toBool());
    hostnameEdit->setText(settings->value("proxy/host").toString());
    portEdit->setText(settings->value("proxy/port").toString());
    userNameEdit->setText(settings->value("proxy/user").toString());
    passwordEdit->setText(settings->value("proxy/password").toString());
    startpageEdit->setText(settings->value("page/default").toString());

    this->setFixedSize(QSize(300,300));

}

void MySettings::manageButtonsEnabled(bool flag) {
    hostnameEdit->setEnabled(flag);
    portEdit->setEnabled(flag);
    userNameEdit->setEnabled(flag);
    passwordEdit->setEnabled(flag);
}

void MySettings::okClicked() {
    if (!(startpageEdit->text().startsWith("http://") || startpageEdit->text().startsWith("https://"))) {
        startpageEdit->setText("http://"+startpageEdit->text());
     }
     settings->setValue("proxy/useproxy",useProxyBox->isChecked());
     settings->setValue("proxy/host",hostnameEdit->text());
     settings->setValue("proxy/port",portEdit->text());
     settings->setValue("proxy/user",userNameEdit->text());
     settings->setValue("proxy/password",passwordEdit->text());
     settings->setValue("page/default",startpageEdit->text());
     settings->sync();
     emit settingsChanged();
     close();
}

void MySettings::cancelClicked() {
     close();
}

void MySettings::defaultClicked(bool checked) {
    if (checked) {
         useProxyBox->setChecked(true);
         hostnameEdit->setText("172.27.100.5");
         portEdit->setText("4444");
         userNameEdit->setText("student.istu");
         passwordEdit->setText("student");
         startpageEdit->setText("https://xtenzq.github.io/");
    }
    else {
        useProxyBox->setText("");
        hostnameEdit->setText("");
        portEdit->setText("");
        userNameEdit->setText("");
        passwordEdit->setText("");
        startpageEdit->setText("");
    }
}

void MySettings::defaultClicked() {

}
