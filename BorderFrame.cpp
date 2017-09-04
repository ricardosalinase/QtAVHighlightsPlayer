#include "BorderFrame.h"

BorderFrame::BorderFrame(QWidget *parent):
    QFrame(parent),
    border(2)
{

}

void BorderFrame::paintEvent(QPaintEvent *e)
{
    if(border > 0){
        QPainterPath path;
        path.addRoundedRect(rect(), border, border);
        QRegion region = QRegion(path.toFillPolygon().toPolygon());
        this->setMask(region);
    }
    QFrame::paintEvent(e);
}

void BorderFrame::setBorder(int value)
{
    border = value;
}
