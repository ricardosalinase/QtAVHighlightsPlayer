#include "CustomPushButton.h"
#include <QPaintEvent>

CustomPushButton::CustomPushButton(QWidget *parent):
    QPushButton(parent)
{
}

void CustomPushButton::setBorder(int value)
{
    border = value;
}

void CustomPushButton::paintEvent(QPaintEvent *event)
{
    event->accept();
    QPainterPath path;
    path.addRoundedRect(rect(), border, border);
    QRegion region = QRegion(path.toFillPolygon().toPolygon());
    this->setMask(region);
    QPushButton::paintEvent(event);
}
