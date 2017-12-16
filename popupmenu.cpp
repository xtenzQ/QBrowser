#include "popupmenu.h"
#include <QPushButton>

/**
 * @brief PopupMenu::PopupMenu
 * @param button
 * @param parent
 */
PopupMenu::PopupMenu(QPushButton* button, QWidget* parent) : QMenu(parent), b(button) {
    QPoint p = this->pos();
    QRect geo = b->geometry();
    this->move(p.x()+geo.width()-this->geometry().width(), p.y());
}

void PopupMenu::showEvent(QShowEvent* event) {
    QPoint p = this->pos();
    QRect geo = b->geometry();
    this->move(p.x()+geo.width()-this->geometry().width(), p.y());
}
