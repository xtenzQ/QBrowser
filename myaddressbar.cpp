#include "myaddressbar.h"
#include <QProgressBar>

myAddressBar::myAddressBar(QWidget *parent) : UrlLineEdit(parent),
     m_progressBar(new QProgressBar(this))
{
    m_progressBar->setSize(this->width(), this->height());
}
