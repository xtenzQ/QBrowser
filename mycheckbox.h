#ifndef MYCHECKBOX_H
#define MYCHECKBOX_H
#include <QtWidgets>
#include <QTimer>

class MyCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    MyCheckBox(QWidget *parent=0);
private:
    QTimer *timer;
    int colorIndex;

protected:
    void mousdeReleaseEvent(QMouseEvent *event);

private slots:
    void timerShot();
};

#endif // MYCHECKBOX_H
