#include "niceDragItemRenderer.h"

#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>

niceDragItemRenderer::niceDragItemRenderer(QGraphicsItem *item):
    QtAV::GraphicsItemRenderer(item)
    , modelIndex(-1)
    //, speed(1.0)
    , path("NULL")
    , times("NULL")
    , dragPixmapSize(150,150)
{
    setAcceptedMouseButtons(Qt::LeftButton);
}

void niceDragItemRenderer::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    qDebug()<<"Video Double Clicked";
    emit videoRendererClicked();
}

void niceDragItemRenderer::setTimes(qint32 init, qint32 end)
{
    times = "(" + QString::number(init) + "-" + QString::number(end) + ")";
}

//void niceDragItemRenderer::setSpeed(const qreal &value)
//{
//    speed = value;
//}

void niceDragItemRenderer::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug()<<"niceDragItemRenderer::MouseMoveEvent";
    if(!(event->buttons() & Qt::LeftButton))
        return;

//    qDebug()<<"Drag Start Position: "<<dragStartPosition;
    if ((event->pos() - dragStartPosition).manhattanLength()
             < QApplication::startDragDistance())
            return;

    if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton))
        .length() < QApplication::startDragDistance()) {
        //qDebug()<<"niceDragItemRenderer::No Drag";
        return;
    }

    QDrag *drag = new QDrag(event->widget());
    QMimeData *mime = new QMimeData;
    drag->setMimeData(mime);

    if(times != "NULL")
        mime->setText(path + "," + times);

    drag->setPixmap(this->toPixmap());
    drag->setHotSpot(QPoint(15, 20));

    qDebug()<<"niceDragItemRenderer::Init Drag" << mime->text();
    drag->exec();

}

void niceDragItemRenderer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
//    qDebug()<<"niceDragItemRenderer::MousePressEvent";
    if (event->button() == Qt::LeftButton)
            dragStartPosition = event->pos();

    if(path == "NULL")
        return;
}

void niceDragItemRenderer::setPixmapSize(const QSize &value)
{
    dragPixmapSize = value;
}

void niceDragItemRenderer::setPath(const QString &value)
{
    path = value;
}

void niceDragItemRenderer::setModelIndex(int value)
{
    modelIndex = value;
}

QPixmap niceDragItemRenderer::toPixmap() const
{
    if (!scene()) {
        qWarning() << "[ControlItem::toPixmap] scene is null.";
        return QPixmap();
    }

    QRectF r = boundingRect();
    QPixmap pixmap(r.width(), r.height());
    pixmap.fill(QColor(0, 0, 0, 0));
    QPainter painter(&pixmap);
    //painter.setBrush(QBrush(QColor(0, 0, 0, 0)));
    painter.drawRect(r);
    scene()->render(&painter, QRectF(), sceneBoundingRect());
    painter.end();
    return pixmap.scaled(dragPixmapSize,Qt::KeepAspectRatio);
}

QString niceDragItemRenderer::getMimeDataText()
{
    if( path != "NULL" )
        return QString(path + "," + times);
    else
        return QString();
}
