#ifndef POPUPMENU_H
#define POPUPMENU_H

#include <QMenu>

class QPushButton;
class QWidget;

class PopupMenu : public QMenu
{
    Q_OBJECT
public:
    explicit PopupMenu(QPushButton* button, QWidget* parent = 0);
    void showEvent(QShowEvent* event);
private:
    QPushButton* b;
protected:
    // void resizeEvent(QResizeEvent *event);
};

#endif // POPUPMENU_H
