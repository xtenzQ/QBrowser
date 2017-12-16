#include "mydialog.h"

MyDialog::MyDialog(QWidget *parent=0):QDialog(parent) {
    settings = *mysettings;
    setModal(true);
    setWindowTitle(tr("Browser settings"));
    setAttribute(Qt::WA_DeleteOnClose);

    useProxyBox = new QCheckBox();
    hostnameEdit = new QLineEdit();
    portEdit = new QLineEdit();
    userNameEdit = new QLineEdit();
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);

    QPushButton *okButton = new QPushButton(tr("Ok"));
    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    QPushButton *defaultButton = new QPushButton(tr("Load defaults"));

    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addWidget(new QLabel(tr("Use proxy?")),0,0);
    mainLayout->addWidget(new QLabel(tr("Host")),1,0);
    mainLayout->addWidget(new QLabel(tr("Port")),2,0);
    mainLayout->addWidget(new QLabel(tr("User name")),3,0);
    mainLayout->addWidget(new QLabel(tr("Password")),4,0);
    mainLayout->addWidget(useProxyBox,0,1);
    mainLayout->addWidget(hostnameEdit,1,1);
    mainLayout->addWidget(portEdit,2,1);
    mainLayout->addWidget(userNameEdit,3,1);
    mainLayout->addWidget(passwordEdit,4,1);
    mainLayout->addWidget(okButton,5,0);
    mainLayout->addWidget(cancelButton,5,1);
    mainLayout->addWidget(defaultButton,6,0,1,2);

    setLayout(mainLayout);
    connect(okButton,SIGNAL(clicked(bool)),this,SLOT(okClicked()));
    connect(cancelButton,SIGNAL(clicked(bool)),this,SLOT(cancelClicked()));
    connect(defaultButton,SIGNAL(clicked(bool)),this,SLOT(defaultClicked()));

}

void MyDialog::okClicked() {

     close();
}

void MyDialog::cancelClicked() {
     close();
}

void MyDialog::defaultClicked() {
     useProxyBox->setChecked(true);
     hostnameEdit->setText("172.27.100.5");
     portEdit->setText("4444");
     userNameEdit->setText("student.istu");
     passwordEdit->setText("student");
}
