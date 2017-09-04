#include "NiceTimeSlider.h"

#include <QMouseEvent>
#include <QStyleOptionSlider>
#include <QDebug>
#include <QPainter>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include "hltviewutils.h"
NiceTimeSlider::NiceTimeSlider(Qt::Orientation o, QFrame *parent):
    QSlider(o, parent)
  , m_extUnit(-1)
  , m_extInitValue(-1)
  , selectedColor( QColor(214, 174, 10, 200))
  , unselectedColor( QColor(255, 255, 255, 100))
  , aboveColor( QColor(214, 174, 10, 200))
  , useTicks(true)
  , tickWidth(6)
  , tickHeight(16)
  , drawLargerTick(false)
  , firstDragParam("NULL")
  , dragIndexParam(-1)
  , dragTimes("NULL")
  , srcDuration(-1)
{
    setEnabled(false);
    setMouseTracking(true);
}

NiceTimeSlider::NiceTimeSlider(QWidget* parent):
    QSlider(parent)
  , m_extUnit(-1)
  , m_extInitValue(-1)
  , selectedColor( QColor(214, 174, 10, 255))
  , unselectedColor( QColor(255, 255, 255, 100))
  , aboveColor( QColor(214, 174, 10, 150))
  , useTicks(true)
  , tickWidth(6)
  , tickHeight(16)
  , drawLargerTick(false)
  , firstDragParam("NULL")
  //, currentSpeed(1.0)
  , dragIndexParam(-1)
  , dragTimes("NULL")
{
    setMouseTracking(true);
}

void NiceTimeSlider::insertTick(tick p, QVector<tick> &tickVector)
{
//    qDebug()<<"Tick: "<<useTicks;
    if(useTicks){
//        qDebug()<<"Position: "<<p.pos;
        p.selected = false;
        p.over = false;
        tickVector.push_back(p);
    }
}

void NiceTimeSlider::updateTick(tick p,int idx, QVector<tick> &tickVector){

    //We assume tickVector size is not zero
    if(useTicks){
       p.selected = true;
       p.over = false;
       QList<qreal>::const_iterator it;
       for(it = tickVector.at(idx).marks.begin(); it != tickVector.at(idx).marks.end(); ++it)
       {
            p.marks.push_back(*it);
       }
       tickVector[idx] = p;
       update();
    }
}

void NiceTimeSlider::deleteTick(int id, QVector<tick> &tickVector)
{
    QMutableVectorIterator<tick> i(tickVector);
    int count = 0;
    while (i.hasNext()) {
        if (i.next().id == id){
            i.remove();
            qDebug()<<"Remove Item "<<count;
            count++;
        }
    }
}

void NiceTimeSlider::deleteAllTicks()
{
    highlights.clear();
}

qreal NiceTimeSlider::getTickWidth() const
{
    return tickWidth;
}

void NiceTimeSlider::setTickWidth(const qreal &value)
{
    tickWidth = value;
}

qreal NiceTimeSlider::getTickHeight() const
{
    return tickHeight;
}

void NiceTimeSlider::setTickHeight(const qreal &value)
{
    tickHeight = value;
}

int NiceTimeSlider::timeToSliderPos(const qreal &value)
{
    qreal aux = (value - ((qreal)m_extInitValue))/(qreal)m_extUnit;

    return QStyle::sliderPositionFromValue(minimum(),
                                           maximum(),
                                           (int)aux,
                                           width());
}

void NiceTimeSlider::setFirstDragParam(const QString &value)
{
    firstDragParam = value;
}

void NiceTimeSlider::setSrcDuration(const qint32 &value)
{
    srcDuration = value;
}

//void NiceTimeSlider::setSpeed(const qreal &value)
//{
//    currentSpeed = value;
//}


void NiceTimeSlider::drawTick(QPainter &p)
{
    if(m_extUnit > 0 && m_extInitValue > -1 && useTicks){
        p.setRenderHint(QPainter::Antialiasing);

        int center = height()/2;

        //Draw highlights
        QVector<tick> hltsCopy(highlights);
        QVectorIterator<tick> i(hltsCopy);

        int idx = 0;
        while (i.hasNext()) {
            QPixmap icon;
            tick cTick = i.next();
            qreal tickWidthTemp = tickWidth;
            qreal tickHeightTemp = tickHeight;

            int position = timeToSliderPos((qreal)cTick.pos);

            if(cTick.over){
                p.setPen(aboveColor);
                p.setBrush(aboveColor);
                if(drawLargerTick){
                    tickWidthTemp *= 2.0;
                    tickHeightTemp *= 2.0;
                }

                icon = QPixmap(trickToPixmapPath(cTick.typeName) + "-activo.png");
            }else if(cTick.selected)
            {
                p.setPen(selectedColor);
                p.setBrush(selectedColor);

                icon = QPixmap(trickToPixmapPath(cTick.typeName) + "-activo.png");
            }else
            {
                p.setPen(unselectedColor);
                p.setBrush(unselectedColor);
                icon = QPixmap(trickToPixmapPath(cTick.typeName) + ".png");
            }

            //Set Position
//           QRect rect;
//            if(!cTick.isRect){
//                //Set Rect
//                rect = QRect(position-(int)(tickWidthTemp/2.0),center-(int)(tickHeightTemp/2.0),tickWidthTemp,tickHeightTemp);

//            }else{
//                int lposition = timeToSliderPos(cTick.lpos);
//                int rposition = timeToSliderPos(cTick.rpos);
//                if(rposition - lposition < tickWidthTemp)
//                    rect = QRect(position-(int)(tickWidthTemp/2.0),center-(int)(tickHeightTemp/2.0),tickWidthTemp,tickHeightTemp);
//                else
//                    rect = QRect(lposition, center-(int)(tickHeightTemp/2.0), rposition-lposition, tickHeightTemp);
//            }

//            //Draw
//            if(drawLargerTick)
//                p.drawRoundedRect(rect,4,4);
//            else
//                p.drawRoundedRect(rect,2,2);

            QRect rect(position - (icon.width()/2), center - (icon.height()/2), icon.width(), icon.height());
            p.drawPixmap(rect.topLeft(),icon);

            highlights[idx].boundingRect = rect;
            idx++;
        }
    }
}

//Selección de Tick, se setea un tick en seleccionado y el resto en no seleccionado
void NiceTimeSlider::setTickSelected(int id)
{
    QMutableVectorIterator<tick> i(highlights);
    while (i.hasNext()) {
        if (i.next().id == id){
            i.value().selected = true;
        }else
            i.value().selected = false;
    }
}

//Tick ingresado, indica que se a ingresado en alguno de los ticks
void NiceTimeSlider::setTickHovered(int id)
{
    QMutableVectorIterator<tick> i(highlights);
    while (i.hasNext()) {
        if (i.next().id == id){
            i.value().over = true;
        }else
            i.value().over = false;
    }
}

//Cuando se encuentra el tick que se busca se selecciona e inmediatamente se sale del método
void NiceTimeSlider::setTickSelectedRelax(int id)
{
    QMutableVectorIterator<tick> i(highlights);
    while (i.hasNext()) {
        if (i.next().id == id){
            i.value().selected = true;
            return;
        }
    }
}

//Selección de más de un tick a través del método de selección de tick relajado
void NiceTimeSlider::setSeveralTickSelected(QVector<int> idxs)
{
    foreach(int id,idxs)
    {
        setTickSelectedRelax(id);
    }
}

void NiceTimeSlider::leaveEvent(QEvent *e)
{
    emit leave();
    QSlider::leaveEvent(e);
}

int NiceTimeSlider::getExtUnit() const
{
    return m_extUnit;
}

void NiceTimeSlider::setExtUnit(const int &value)
{
    if(value > 0)
        m_extUnit = value;
    else{
        qDebug()<<"NiceSlider::Unidad externa no correctamente definida, debe ser mayor a 0";
        m_extUnit = -1;
    }
}

void NiceTimeSlider::setUseTicks(bool state)
{
    useTicks = state;
}

inline int NiceTimeSlider::pick(const QPoint &pt) const
{
    return orientation() == Qt::Horizontal ? pt.x() : pt.y();
}

// Function copied from qslider.cpp and modified to make it compile
int NiceTimeSlider::pixelPosToRangeValue(int pos) const
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect gr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    int sliderMin, sliderMax, sliderLength;
    if (orientation() == Qt::Horizontal) {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
    } else {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }
    return QStyle::sliderValueFromPosition(minimum(), maximum(), pos - sliderMin,
                                           sliderMax - sliderMin, opt.upsideDown);
}

void NiceTimeSlider::mousePressEvent(QMouseEvent *e) {

    if (e->button() == Qt::LeftButton) {

        QVectorIterator<tick> i(highlights);
        while (i.hasNext()) {
            tick cTick = i.next();
            if(cTick.boundingRect.contains(e->pos())){
                int idx = cTick.id;
                dragIndexParam = -1;
                dragTimes = "NULL";
                if(e->modifiers() == Qt::ControlModifier && cTick.selected){
                    //Highlight Released
                    //qDebug()<<"Highlight Released -> "<<idx;
                    emit tickClicked(-1);
                }
                else{
                    //Highlight Clicked
                    //qDebug()<<"Highlight Clicked -> "<<idx;
                    dragStartPosition = e->pos();
                    dragTypeName = cTick.typeName;
                    dragIndexParam = cTick.id;
                    qint32 st = std::max(0,cTick.lpos - 3000);
                    qint32 et = std::min(srcDuration,cTick.rpos + 3000);

                    dragTimes = "(" + QString::number(st) + "-" + QString::number(et) + ")";
                    //dragTimeParam = "(" + QString::number(cTick.lpos) + "-" + QString::number(cTick.rpos) + ")";
                    emit tickClicked(idx);
                }
                //QSlider::mousePressEvent(e);
                return;
            }
        }

        QStyleOptionSlider opt;
        initStyleOption(&opt);
        const QRect sliderRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
        const QPoint center = sliderRect.center() - sliderRect.topLeft();
        // to take half of the slider off for the setSliderPosition call we use the center - topLeft

        if (!sliderRect.contains(e->pos())) {
            int newVal;
            if (orientation() == Qt::Vertical) {
               double halfHandleHeight = (0.5 * sliderRect.height()) + 0.5;
               int adaptedPosY = height() - e->y();
               if ( adaptedPosY < halfHandleHeight )
                     adaptedPosY = halfHandleHeight;
               if ( adaptedPosY > height() - halfHandleHeight )
                     adaptedPosY = height() - halfHandleHeight;
               double newHeight = (height() - halfHandleHeight) - halfHandleHeight;
               double normalizedPosition = (adaptedPosY - halfHandleHeight)  / newHeight ;

               newVal = minimum() + (maximum()-minimum()) * normalizedPosition;
            } else {
                double halfHandleWidth = (0.5 * sliderRect.width()) + 0.5;
                int adaptedPosX = e->x();
                if ( adaptedPosX < halfHandleWidth )
                      adaptedPosX = halfHandleWidth;
                if ( adaptedPosX > width() - halfHandleWidth )
                      adaptedPosX = width() - halfHandleWidth;
                double newWidth = (width() - halfHandleWidth) - halfHandleWidth;
                double normalizedPosition = (adaptedPosX - halfHandleWidth)  / newWidth ;

                newVal = minimum() + ((maximum()-minimum()) * normalizedPosition);
            }

            if (invertedAppearance())
                setValue( maximum() - newVal );
            else
                setValue(newVal);

           //qDebug("accept");
            e->accept();

            int v = pixelPosToRangeValue(pick(e->pos() - center));
            setSliderPosition(v);
            triggerAction(SliderMove);
            setRepeatAction(SliderNoAction);
            emit sliderMoved(v);//TODO: ok?
            emit sliderPressed(); //TODO: ok?
        } else {
            QSlider::mousePressEvent(e);
        }
    } else {
        QSlider::mousePressEvent(e);
    }
}

void NiceTimeSlider::mouseMoveEvent(QMouseEvent *e)
{
    const int o = style()->pixelMetric(QStyle::PM_SliderLength ) - 1;
    qreal max = maximum();
    qreal min = minimum();
    int v = QStyle::sliderValueFromPosition(min, max, e->pos().x()-o/2, width()-o, false);
    qreal value = (qreal)(v*m_extUnit + m_extInitValue);

    emit onHover(e->x(), (int)value);

    QVector<tick> hltsCopy(highlights);
    QVectorIterator<tick> i(hltsCopy);
    bool drawLargerFlag = false;
    int idx = 0;
    while (i.hasNext()) {
        tick cTick = i.next();
        if(cTick.boundingRect.contains(e->pos())){
            //qDebug()<<"Over highlight "<<i.value().id;
            highlights[idx].over = true;
            emit overTick(cTick.id);
            drawLargerFlag = true;
        }else
        {
            highlights[idx].over = false;
        }
        idx++;
    }

    if(drawLargerFlag)
        drawLargerTick = true;
    else{
        drawLargerTick = false;
        emit overTick(-1);
    }

    this->update();
    if(this->firstDragParam == "NULL" || this->dragTimes == "NULL")
        return;
    if (!(e->buttons() & Qt::LeftButton))
            return;
    if ((e->pos() - dragStartPosition).manhattanLength()
         < QApplication::startDragDistance())
        return;

    QDrag *drag = new QDrag(this);
    QMimeData *mime = new QMimeData;
    drag->setMimeData(mime);

    QStringList strs;
    //strs<< this->firstDragParam << QString::number(this->dragIndexParam) << QString::number( this->currentSpeed );
    strs<< this->firstDragParam << this->dragTimes;
    QList< QStringList > strList;
    strList << strs;
    mime->setText(generateMimeDataString(strList));
    drag->setPixmap(QPixmap(trickToPixmapPath(dragTypeName) + ".png"));
    drag->setHotSpot(QPoint(15, 20));

    qDebug()<<"niceListWidget::Init Drag -> "<<mime->text();
    drag->exec();
}

void NiceTimeSlider::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
}

void NiceTimeSlider::paintEvent(QPaintEvent *ev) {
    QSlider::paintEvent(ev);
    QPainter painter(this);
    drawTick(painter);
}

void NiceTimeSlider::updateTickGeometry(qreal widthRatio, qreal heightRatio)
{
    tickWidth = widthRatio*(this->width());
    tickHeight = heightRatio*(this->height());
}

qint32 NiceTimeSlider::getExtInitValue() const
{
    return m_extInitValue;
}

void NiceTimeSlider::setExtInitValue(const qint64 &extInitValue)
{
    if(extInitValue >= 0)
        m_extInitValue = extInitValue;
    else{
        qDebug()<<"NiceSlider::Unidad externa no correctamente definida, debe ser mayor a 0";
        m_extInitValue = -1;
    }
}
