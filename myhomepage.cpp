#include "myhomepage.h"

MyHomepage::MyHomepage(QWidget *parent = 0) : QDialog(parent)
{
    setModal(true);
    setWindowTitle(tr("Set homepage"));
    setAttribute(Qt::WA_DeleteOnClose);

    QPushButton *okButton = new QPushButton(tr("OK"));

    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addWidget(new QLabel(tr("Enter URL")),0,0);
    mainLayout->addWidget(homepageBox,0,1);
    mainLayout->addWidget(okButton,1,0,0);

    setLayout(mainLayout);
}
