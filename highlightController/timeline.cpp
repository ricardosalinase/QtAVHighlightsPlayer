#include "timeline.h"
#include <QDebug>
#include <QApplication>
#include <QGraphicsSceneMouseEvent>

/*Custom Class for Zoomed Line-Time Zine of Highlights Visualization
 * Autor: Nice Inc
 * Date : 05/11/2015
 * */

timeLine::timeLine(QGraphicsItem *parent) :
    QGraphicsItem(parent)
  , barColor(QColor(255,255,255,0))
{
    //setCursor(Qt::PointingHandCursor);
    this->setAcceptHoverEvents(true);
}

void timeLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QPen linePen(barColor);
    //QPen hltPen(hltColor);
    linePen.setWidth(height);
    //hltPen.setWidth(widthInnerRect);

    painter->setPen(linePen);

    painter->drawLine(line);
}

void timeLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"Clicked on Time Line";
    QPointF pos  = this->mapToScene(event->pos());
    emit release(pos.x());
    QGraphicsItem::mousePressEvent(event);
}


void timeLine::setView(QPointF m_p1, QPointF m_p2, int m_height)
{
    line = QLineF(m_p1,m_p2);
    height = m_height;
}


QRectF timeLine::boundingRect() const
{
   QRectF rect(line.p1().x(),line.p1().y()-height/2,line.p2().x()-line.p1().x(),height);
   return rect;
}
