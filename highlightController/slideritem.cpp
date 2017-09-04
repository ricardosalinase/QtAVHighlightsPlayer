#include "slideritem.h"
#include <QDebug>
#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QDrag>
#include <QMimeData>

//Límits parameters
#define LEFT_LIMIT 0//0.25598
#define RIGHT_LIMIT 100//99.667
#define MIN_SIZE 0.16667
#define timeSeek 200
#define MAPTOTIMECNT 65.4

sliderItem::sliderItem(QGraphicsItem *parent ):
    QGraphicsObject( parent )
  , tickOnInit(true)
  , prevLeftValue(-1)
  , prevRightValue(-1)
  , selColor(QColor(255,255,255))
  , commonColor(QColor(241,196,13,200))
  , currZone(NONE)
  , selected(false)
  , line(QLineF())
  , intersected(false)
  , lastSeek(-1)
  , rightLimitFlag(false)
  , leftLimitFlag(false)
  , hltTime("XX/XX")
  , textFactor(1.0)
  , font( QFont("Frutiger CE 45 Light", 8) )
  , leftContained(true)
  , rightContained(true)
  , sliderButtonRatio(0.25)
  , radius(3.0)
  , dragPixmapSize(50,50)
  //, speed(1.0)
  , times("NULL")
  , path("NULL")
{
    initItem();
}

sliderItem::sliderItem(const sliderItem &item):
    QGraphicsObject( 0 )
  , tickOnInit(item.tickOnInit)
  , prevLeftValue(item.prevLeftValue)
  , prevRightValue(item.prevRightValue)
  , selColor(item.selColor)
  , commonColor(item.commonColor)
  , currZone(item.currZone)
  , selected(item.selected)
  , line(item.line)
  , intersected(item.intersected)
  , lastSeek(-1)
  , rightLimitFlag(false)
  , leftLimitFlag(false)
  , hltTime("XX/XX")
  , textFactor(1.0)
  , font( QFont("Frutiger CE 45 Light", 8) )
  , leftContained(true)
  , rightContained(true)
  , sliderButtonRatio(item.sliderButtonRatio)
  , radius(item.radius)
  , dragPixmapSize(item.dragPixmapSize)
{
   initItem();
}

sliderItem& sliderItem::operator=(const sliderItem &item)
{
    tickOnInit = item.tickOnInit;
    prevLeftValue=item.prevLeftValue;
    prevRightValue=item.prevRightValue;
    selColor=item.selColor;
    commonColor=item.commonColor;
    currZone=item.currZone;
    selected=item.selected;
    line=item.line;
    intersected=item.intersected;
    rightContained = item.rightContained;
    leftContained = item.leftContained;
    font = item.font;
    sliderButtonRatio = item.sliderButtonRatio;
    radius = item.radius;
    dragPixmapSize = item.dragPixmapSize;
    return *this;
}

void sliderItem::initItem()
{
    setCursor(QCursor(Qt::PointingHandCursor));
    this->setAcceptHoverEvents(true);
    this->setAcceptedMouseButtons(Qt::LeftButton);
}

void sliderItem::setLocalLimits()
{
    QGraphicsScene *scene = this->scene();
    if(scene != NULL)
    {
        this->local_limitLeft = (LEFT_LIMIT*scene->width())/100.0;
        this->local_limitRight = (RIGHT_LIMIT*scene->width())/100.0;
        this->local_limitWidth = (MIN_SIZE*scene->width())/100.0;
    }
}

void sliderItem::setSliderGeometry(QRectF m_rect, qreal bRatio, qreal m_width, qreal m_bwidth)
{
    sliderButtonWidth = m_bwidth;
    sliderWidth = m_width;
    sliderLengthFactor = m_width + m_bwidth;

    //Bounding Rect Geometry
    rect = QRectF(m_rect.x() - (sliderLengthFactor),
                  m_rect.y(),
                  m_rect.width() + 2.0*(sliderLengthFactor),
                  m_rect.height()
                  );

    toLeftDistance = rect.center().x()-rect.topLeft().x();
    toRightDistance = rect.topRight().x()-rect.center().x();

    //Min distance between slider ends
    limitSlidersDistance = (sliderLengthFactor)*2.5;
    sliderButtonRatio = bRatio;

//    qDebug()<<"slider Button Width: "<<sliderButtonWidth;
//    qDebug()<<"slider Width: "<<sliderWidth;

    setBarPos();
}

void sliderItem::setSlider(qreal init, qreal end)
{
//    qDebug()<<"sliderItem:: setSlider to"<<"("<<init<<", "<< end<< ")";
    prepareGeometryChange();

    qreal oldright = rect.right();
    qreal oldleft = rect.left();

    if(end-init > local_limitWidth){
        rect.setRight(end);
        rect.setLeft(init);
        //qDebug()<<"setSlider WIDTH"<< rect.width();
        //tamaño mínimo
        if(rect.width() < limitSlidersDistance)
        {

            qDebug() << "Limite de ancho alcanzado";
            rect.setRight(oldright);
            rect.setLeft(oldleft);
        }
        //limite izquierdo
        if(rect.left() < local_limitLeft){

            rightLimitFlag = false;
            leftLimitFlag = true;
            qDebug() << "Limite izquierdo alcanzado";
            rect.setRight(oldright);
            rect.setLeft(local_limitLeft+1);
        }
        //limite derecho
        else if(rect.right() > local_limitRight ){
            rightLimitFlag = true;
            leftLimitFlag = false;
            qDebug() << "Limite derecho alcanzado";
            rect.setRight(local_limitRight-1);
            rect.setLeft(oldleft);
        }
        else {
            //qDebug() << "Ningún límite alcanzado";
            rightLimitFlag = false;
            leftLimitFlag = false;
        }

        //update();
        setBarPos();

    }
}

QString sliderItem::convertSliderTimeToMMSS(qint32 diff){
    QString durationHltText;
    textFactor = 1;
    if(diff < 10){
        durationHltText = "0" + QString::number(diff);

    }else if (diff < 60){
        durationHltText = QString::number(diff);
    }
    else
    {
    if(diff % 60 < 10){
        durationHltText = '0'+ QString::number(diff/60)+":0"+QString::number(diff%60);
        textFactor = 2.5;
        }
        else{
          durationHltText = '0'+ QString::number(diff/60)+':'+QString::number(diff%60);
          textFactor = 2.5;
        }
    }
    return durationHltText;
}


void sliderItem::setHltTime(QString value)
{
    //qDebug()<<"Set hlt time"<<value;
    if(hltTime == value)
        return;

    hltTime = value;
    if(isVisible())
        this->update();
    //    hltTime = convertSliderTimeToMMSS(value);
}

void sliderItem::setPath(const QString &value)
{
    path = value;
}

void sliderItem::setTimes(qint32 init, qint32 end)
{
    times = "(" + QString::number(init) + "-" + QString::number(end) + ")";
}

//void sliderItem::setSpeed(const qreal &value)
//{
//    speed = value;
//}

qreal sliderItem::getSliderLenghtFactor() const
{
    return sliderLengthFactor;
}


void sliderItem::setFontSize(int fontsize)
{
    font.setPointSize(fontsize);
}

QString sliderItem::getHltTime() const
{
    return hltTime;
}

bool sliderItem::onLeftSliderFromScene(QPointF e)
{
    Q_UNUSED(e)

    return this->isUnderMouse();
}

bool sliderItem::onRightSliderFromScene(QPointF e)
{
    QPointF p = e - boundingRect().topLeft();
    return onRightSlider(p);
}

bool sliderItem::getTickOnInit() const
{
    return tickOnInit;
}

void sliderItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    qDebug("sliderItem::Mouse Press Event");

    if(this->selected)
    {
        QGraphicsScene *sc = this->scene();
        this->setZValue(2.01);
        QList<QGraphicsItem*> colItems = sc->collidingItems(this);
        qDebug()<<"Items colisionando: "<<colItems.size();
        setSlidersToBack(colItems);

        QPointF pos = event->pos();
        //seekTimer.start();
        QPointF p = pos - boundingRect().topLeft();
        QRectF currRect = boundingRect();

    //    qDebug()<<"sliderItem::Slider Clickeado en: "<<p.x()<<", "<<p.y();
    //    QGraphicsScene *scene = this->scene();
    //    qDebug()<<"Scene size "<< scene->width();

        if(onLeftSlider(p)) currZone = LEFT;
        else if(onRightSlider(p)) currZone = RIGHT;
        else if (onCenterSlider(p)) currZone = CENTER;

        toLeftDistance = qAbs(pos.x()-currRect.left());
        toRightDistance = qAbs(currRect.right()-pos.x());

        qDebug()<<"Zone: "<<currZone;

        this->update();

        //clicked = true;

        dragStartPosition = pos.toPoint();
    }

    emit sliderPress(selected);
    //emit signal
}

void sliderItem::changeSlider()
{
    if(getSliderValue(LEFT) != prevLeftValue)
        tickOnInit = true;

    if(getSliderValue(RIGHT) != prevRightValue)
        tickOnInit = false;

    if(getSliderValue(RIGHT) != prevRightValue && getSliderValue(LEFT) != prevLeftValue)
        tickOnInit = true;

    prevLeftValue = getSliderValue(LEFT);
    prevRightValue = getSliderValue(RIGHT);

    if(leftLimitFlag || rightLimitFlag)
        emit sliderChange(true);
    else
        emit sliderChange(false);
}

void sliderItem::setBarPos()
{
    //No definir posiciones cuando el slider sobrepasa el Zoom
    QRectF currentRect = boundingRect();

//    posSlider[LEFT] = currentRect.left() + (widthInnerRect*2.0);
//    posSlider[RIGHT] = currentRect.right() - (widthInnerRect*2.0);
    posSlider[LEFT] = currentRect.left() + (sliderLengthFactor);
    posSlider[RIGHT] = currentRect.right() - (sliderLengthFactor);
    posSlider[CENTER] = currentRect.center().x();

    //qDebug()<<"Current Width: "<<posSlider[RIGHT] - posSlider[LEFT];
    //qDebug()<<"Current Left: "<< posSlider[LEFT];
    //qDebug()<<"Current Right: "<< posSlider[RIGHT];
}

bool sliderItem::onCenterSlider(QPointF p)
{
    if(p.y() >= (boundingRect().height() - sliderWidth) ) return true;
    else return false;
}

bool sliderItem::onLeftSlider(QPointF p)
{
    if(p.x() <= sliderLengthFactor) return true;
    else return false;
}

bool sliderItem::onRightSlider(QPointF p)
{
    if(p.x() >= boundingRect().width() - (sliderLengthFactor)) return true;
    else return false;
}

qreal sliderItem::getSliderValue(sliderZone z)
{
    return posSlider[z];
}

QRectF sliderItem::boundingRect() const
{
    return rect;
}

QPainterPath sliderItem::shape() const
{
    QPainterPath path;
    //QRectF bRect = this->boundingRect();
    //QPointF initPos = bRect.topLeft();
    QRectF rInner(getInnerRectangle());

    RoundedPolygon poly;
    poly.SetRadius(this->radius);
    getSliderPointList(poly);
    path = poly.GetPath();
//    path.moveTo(pts.front());
//    pts.removeFirst();

//    QList< QPointF >::iterator it = pts.begin();
//    while(it != pts.end())
//    {
//        path.lineTo(*it);
//        ++it;
//    }
//    path.lineTo(pts.back());
    //path.closeSubpath();

    //path.addRect(bRect);

    QPainterPath inner;
    inner.addRect(rInner);
    path = path.subtracted(inner);

    return path;
}

void sliderItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    Q_UNUSED(option)
    Q_UNUSED(widget)

    QRectF currentRect = boundingRect();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);

    qreal textWidht = currentRect.height()*0.4*textFactor;
    //qreal textHeight = currentRect.height()*0.15;

    bool drawTextFlag = textWidht < currentRect.width() ? true :false;

    QPoint start(currentRect.x(),0);
    QPoint end(currentRect.x()+currentRect.width(),0);
    QColor firstColor(selColor);
    QColor secondColor(selColor);
    qreal firstStop = 0;
    qreal secondStop = 1;

    if(selected){
        firstColor = commonColor;
        secondColor = commonColor;

        //painter->setBrush(selColor);
        //dibuja el cambio de color al hacer click en currZone
        if(currZone == LEFT)
        {
            //Gradient Work
            start = QPoint(currentRect.x() + (sliderLengthFactor),0);
            end = QPoint(currentRect.x() + (sliderLengthFactor)+5,0);
            firstColor = QColor(Qt::white);
            //painter->drawRect(r);
        }else if(currZone == RIGHT)
        {
            //Gradient Work
            start = QPoint(currentRect.x() + currentRect.width() - (sliderLengthFactor) - 5,0);
            end = QPoint(currentRect.x() + currentRect.width() - (sliderLengthFactor),0);
            secondColor = QColor(Qt::white);
            //painter->drawRect(r);
        }
        else if(currZone == CENTER)
        {
            //Gradient Work
            start = QPoint(0,currentRect.y());
            end = QPoint(0,currentRect.y() + currentRect.height());
            firstColor = QColor(Qt::white);
            //painter->drawRect(rbottom);
        }
    }

    if(!leftContained && !rightContained)
    {
        start = QPoint(currentRect.x(),0);
        end = QPoint(currentRect.x() + currentRect.width(),0);
        firstColor = QColor(Qt::transparent);
    }
    else if(!leftContained)
    {
        start = QPoint(currentRect.x() + (sliderLengthFactor),0);
        end = QPoint(currentRect.x() + (sliderLengthFactor)+5,0);
        firstColor = QColor(Qt::transparent);

    }else if(!rightContained){
        start = QPoint(currentRect.x() + currentRect.width() - (sliderLengthFactor) - 5,0);
        end = QPoint(currentRect.x() + currentRect.width() - (sliderLengthFactor),0);
        secondColor = QColor(Qt::transparent);
    }

    QLinearGradient g(start,end);
    g.setColorAt(firstStop, firstColor);
    if(!leftContained && !rightContained){
        g.setColorAt(0.5, secondColor);
        g.setColorAt(secondStop, firstColor);
    }else
        g.setColorAt(secondStop, secondColor);
    painter->setBrush(g);
    painter->drawPath(shape());

    //Draw Vertical lines within button sliders
    //RoundedPolygon opts;
    //getSliderPointList(opts);
    QRectF lButton;
    QRectF rButton;

    getButtonRect(lButton, rButton);

    QPainterPath path1, path2, path3, path4;
    qreal horDist = lButton.width()/4.0;
    qreal verDist = lButton.height()/4.0;

    qreal y0 = lButton.top() + verDist;
    qreal y1 = lButton.top() + (lButton.height()/2.0);
    qreal y2 = lButton.bottom() - verDist;

    //Left Right Arrow
    QPointF p0(lButton.right() - horDist, y0);
    QPointF p1(lButton.right() - (horDist*2), y1);
    QPointF p2(lButton.right() - horDist, y2);

    createArrowPath(path1, p0, p1, p2);
    //Left Left Arrow
    QPointF p3(lButton.left() + (horDist*2), y0);
    QPointF p4(lButton.left() +  horDist, y1);
    QPointF p5(lButton.left() + (horDist*2), y2);

    createArrowPath(path2, p3, p4, p5);

    //Right Right Arrow
    QPointF p6(rButton.right() - (horDist*2), y0);
    QPointF p7(rButton.right() -  horDist, y1);
    QPointF p8(rButton.right() - (horDist*2), y2);

    createArrowPath(path3, p6, p7, p8);

    //Right Left Arrow
    QPointF p9(rButton.left() + horDist, y0);
    QPointF p10(rButton.left() +  (horDist*2), y1);
    QPointF p11(rButton.left() + horDist, y2);

    createArrowPath(path4, p9, p10, p11);

    painter->setPen(QColor(Qt::black));
    painter->drawPath(path1);
    painter->drawPath(path2);
    painter->drawPath(path3);
    painter->drawPath(path4);


    if(drawTextFlag){
        //QRectF textRect = QRectF(currentRect.bottomRight().x()-textWidht,currentRect.bottomRight().y()-textHeight,textWidht,textHeight);
        painter->setPen(Qt::white);
        painter->setFont(font);

        this->drawText(*painter, this->getInnerRectangle().bottomRight(),Qt::AlignBottom | Qt::AlignRight,hltTime);
        //painter->drawText(currentRect.bottomLeft(),hltTime, QTextOption(Qt::AlignBottom | Qt::AlignRight));
    }
}

QRectF sliderItem::getInnerRectangle() const
{
    QRectF bRect = this->boundingRect();
    QPointF initPos = bRect.topLeft();
    //qreal innerRectWidth = (sliderLengthFactor/2.0);
    QRectF rInner(initPos + QPointF(sliderLengthFactor,sliderWidth),
                  bRect.size()-QSizeF(2.0*(sliderLengthFactor),2.0*sliderWidth));

    return rInner;
}

void sliderItem::createArrowPath(QPainterPath &path, const QPointF &p0, const QPointF &p1, const QPointF &p2)
{
    path.moveTo(p0);
    path.lineTo(p1);
    path.lineTo(p2);
}

void sliderItem::getButtonRect(QRectF &r1, QRectF &r2) const
{
    //QVector< QPointF > pts;
    RoundedPolygon opts;
    getSliderPointList(opts);

    if(!opts.empty()){
        QPointF p1_lt( opts.at(2) /*+ QPointF(-radius,0)*/);
        QPointF p1_br( opts.at(4) /*+ QPointF(+radius,0)*/);

        QPointF p2_lt( opts.at(10) /*+ QPointF(-radius,0)*/);
        QPointF p2_br( opts.at(8) /*+ QPointF(+radius,0)*/);

        r1 = QRectF(p1_lt, p1_br);
        r2 = QRectF(p2_lt, p2_br);
    }
}

void sliderItem::getSliderPointList(RoundedPolygon &rPol) const
{
    QRectF bRect = this->boundingRect();
    qreal h = bRect.height();
    qreal bh1 = h*sliderButtonRatio;
    qreal bh2 = h*(sliderButtonRatio*3.0);

    //1° Point ( |- )
    QPoint p0(bRect.left() + sliderButtonWidth, bRect.top());

    //2° Point ( _| )
    QPoint p1(p0.x(), p0.y() + bh1);

    //3° Point ( |- )
    QPoint p2(bRect.left(), p1.y());

    //4° Point ( |_ )
    QPoint p3(p2.x(), bRect.bottom() - bh2);

    //5° Point ( -| )
    QPoint p4(p0.x(), p3.y());

    //6° Point ( |_ )
    QPoint p5(p0.x(), bRect.bottom());

    //7° Point ( _| )
    QPoint p6(bRect.right() - sliderButtonWidth, p5.y());

    //8° Point ( |- )
    QPoint p7(p6.x(), p4.y());

    //9° Point ( _| )
    QPoint p8(bRect.right(), p7.y());

    //10° Point ( -| )
    QPoint p9(p8.x(), p1.y());

    //11° Point ( |_ )
    QPoint p10(p7.x(), p9.y());

    //12° Point ( -| )
    QPoint p11(p10.x(), bRect.top());

    rPol << p0 << p1 << p2 << p3 << p4 << p5 << p6 << p7 << p8 << p9 << p10 << p11;
}

void sliderItem::drawText(QPainter & painter, qreal x, qreal y, Qt::Alignment flags,
              const QString & text, QRectF * boundingRect)
{

   const qreal size = 32767.0;
   QPointF corner(x, y - size);
   if (flags & Qt::AlignHCenter) corner.rx() -= size/2.0;
   else if (flags & Qt::AlignRight) corner.rx() -= size;
   if (flags & Qt::AlignVCenter) corner.ry() += size/2.0;
   else if (flags & Qt::AlignTop) corner.ry() += size;
   else flags |= Qt::AlignBottom;
   QRectF rect(corner, QSizeF(size, size));
   painter.drawText(rect, flags, text, boundingRect);
}

void sliderItem::drawText(QPainter & painter, const QPointF & point, Qt::Alignment flags,
              const QString & text, QRectF * boundingRect)
{
   drawText(painter, point.x(), point.y(), flags, text, boundingRect);
}

QPixmap sliderItem::toPixmap() const
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

void sliderItem::setSlidersToBack(QList<QGraphicsItem *> itemList)
{
    QList<QGraphicsItem *>::iterator it;

    for(it = itemList.begin(); it != itemList.end(); ++it)
    {
        if((*it)->type() == (*it)->UserType+2)
        {
            (*it)->setZValue(2.0);
        }
    }
}

void sliderItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->pos();
//    qDebug()<<"sliderItem::Mouse Move Event";
    //    qDebug()<<"pos Mouse: "<<pos.x()<<" , "<<pos.y();
    //    qDebug()<<"Zone: "<<currZone;

        //Implementar nueva posición del slider en función de la distancia entra la posición actual (pos) y los límites del slider.

        qreal currentLeft = rect.left();
        qreal currentRight = rect.right();
        posX = pos.x();
    //    qDebug()<<"Right: "<<currentRight;
    //    qDebug()<<"Left: "<<currentLeft;
    //    qDebug()<<"Width: "<<width;
    //    qDebug()<<"Pos "<<pos.x();
        //solucion actual
        if(currZone == RIGHT){
            //qDebug() <<"RIGHT";
            setSlider(currentLeft, pos.x());

            emit movingSlider();
            return;
        }
        else if(currZone == LEFT){
            //qDebug()<<"LEFT";
            setSlider(pos.x(), currentRight);

            emit movingSlider();
            return;
        }
//        else if(currZone == CENTER)
//        {
//            //qDebug()<<"CENTER";
//            setSlider(pos.x()-toLeftDistance,pos.x()+toRightDistance);
//        }
        else if(path != "NULL" && times != "NULL")
        {
            //qDebug()<<"sliderItem::Distance "<< (pos.toPoint() - dragStartPosition).manhattanLength();
            if ((pos.toPoint() - dragStartPosition).manhattanLength()
                < QApplication::startDragDistance())
            return;

            QDrag *drag = new QDrag(this);
            QMimeData *mime = new QMimeData;
            drag->setMimeData(mime);
            //mime->setText(path + "," + times + "," + QString::number(speed));
            mime->setText(path + "," + times);
            qDebug()<<"slideritem::Init Drag -> "<<mime->text();
            drag->setPixmap(this->toPixmap());
            drag->setHotSpot(QPoint(15, 30));
            drag->exec();
        }
}

void sliderItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    qDebug()<<"sliderItem::Mouse Release Event";
    if(leftLimitFlag || rightLimitFlag)
        qDebug()<<"ZOOM!!";
//    if(seekTimer.isActive())
//        seekTimer.stop();

    currZone = NONE;
    //mouseRelease();
    changeSlider();
    this->update();
}
