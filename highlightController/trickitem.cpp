#include "trickitem.h"
#include <QCursor>
#include <QDebug>

trickItem::trickItem(QGraphicsItem *parent):
    QGraphicsPixmapItem(parent) ,
    key(-1) ,
    isSelected(false)
{
    init();
}

trickItem::trickItem(const QPixmap &pixmap, QGraphicsItem *parent):
    QGraphicsPixmapItem(pixmap,parent) ,
    key(-1) ,
    isSelected(false)
{
    init();
}

trickItem::trickItem(const trickItem &item):
    QGraphicsPixmapItem( 0 ),
    key(item.key) ,
    isSelected(item.isSelected)
{
    init();
}

trickItem& trickItem::operator=(const trickItem &item){

   key = item.key;
   isSelected = item.isSelected;
   return *this;
}

bool trickItem::operator==(const trickItem &item)
{
    return (this->key == item.key);
}

void trickItem::init()
{
    setCursor(QCursor(Qt::PointingHandCursor));
    this->setAcceptHoverEvents(true);
    this->setAcceptedMouseButtons(Qt::LeftButton);
}

int trickItem::getKey() const
{
    return key;
}

void trickItem::setKey(int value)
{
    key = value;
}

bool trickItem::getIsSelected() const
{
    return isSelected;
}

void trickItem::setIsSelected(bool value)
{
    isSelected = value;
}

QPixmap trickItem::getNormalPixmap() const
{
    return normalPixmap;
}

void trickItem::setNormalPixmap(const QPixmap &value, bool set)
{
    normalPixmap = value;
    if(set)
        this->setPixmap(normalPixmap);
}

QPixmap trickItem::getSelectedPixmap() const
{
    return selectedPixmap;
}

void trickItem::setSelectedPixmap(const QPixmap &value, bool set)
{
    selectedPixmap = value;

    if(set)
        this->setPixmap(selectedPixmap);
}

hltPar trickItem::getTimes() const
{
    return times;
}

void trickItem::setTimes(const hltPar &value)
{
    times = value;
}

QString trickItem::getPath() const
{
    return path;
}

void trickItem::setPath(const QString &value)
{
    path = value;
}

void trickItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
    Q_UNUSED(e)
    qDebug()<<"Enter to item "<<this->key;
    this->setPixmap(selectedPixmap);
}

void trickItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    Q_UNUSED(e)
    qDebug()<<"Leave from item "<<this->key;

    if(!this->isSelected)
        this->setPixmap(normalPixmap);
}

//void trickItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    Q_UNUSED(event)
//    qDebug()<<"TrickItem::Mouse Press Event";
//}

//void trickItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{
//    if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton))
//        .length() < QApplication::startDragDistance()) {
//        //qDebug()<<"niceDragItemRenderer::No Drag";
//        return;
//    }

//    qDebug()<<"niceDragItemRenderer::Init Drag";
//    QDrag *drag = new QDrag(event->widget());
//    QMimeData *mime = new QMimeData;
//    drag->setMimeData(mime);
//    mime->setText("Test Data");

//    drag->setPixmap(normalPixmap);
//    drag->setHotSpot(QPoint(15, 20));

//    drag->exec();
//}
