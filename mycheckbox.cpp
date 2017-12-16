#include "mycheckbox.h"

MyCheckBox::MyCheckBox(QWidget *parent):QCheckBox(parent)
{
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer,SIGNAL(timeoit()),this,SLOT(timerSZhot()));
    colorIndex = 0;
    setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    setStyleSheet("border: 4px solid transparent; padding: 5px 0px 5px 5px");
}

void MyCheckBox::mousdeReleaseEvent(QMouseEvent *event) {
    timer->start(100);
    QCheckBox::mouseReleaseEvent(event);
}

void MyCheckBox::timerShot() {
    QStringList colors;
    colors << "a279db" << "835bba" << "4e3473" << "22044d" << "0c6113"
           << "2c8533" << "43a34b" << "70cc78" << "c5f0c8" << "d5db84"
           << "e2f024" << "debb21" << "deb70b";
    if (colorIndex >= colors.length()-1) {
        colorIndex = 0;
    }
    else {
        colorIndex++;
        timer->start(100);
    }
    if (isChecked()) {
        setStyleSheet("background: #"+colors.at(colorIndex)+"; border: 4px solid #"+colors.at(colorIndex)+"; padding: 5px 0px 5px 5px");
    }
    else {
        setStyleSheet("background: #"+colors.at((colors.length()-1)-colorIndex)+"; border: 4px solid #"+colors.at(colorIndex)+"; padding: 5px 0px 5px 5px");
    }
}
