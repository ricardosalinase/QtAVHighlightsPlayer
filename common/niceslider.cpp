#include "niceslider.h"

#include <QMouseEvent>
#include <QStyleOptionSlider>
#include <QDebug>
#include <QToolTip>

NiceSlider::NiceSlider(Qt::Orientation o, QFrame *parent):QSlider(o, parent)
{
}
NiceSlider::NiceSlider(QWidget* parent):QSlider(parent)
{

}

void NiceSlider::leaveEvent(QEvent *e)
{
    emit leave();
    QSlider::leaveEvent(e);
}

void NiceSlider::setZoom(int value, int timeValue)
{
    if(this->value() != value ){
        this->setValue(value);
        this->showZoom(timeValue);
    }
}

void NiceSlider::showZoom(int timeValue)
{
    QPoint p = mapToGlobal(QPoint(this->width()/2, this->height()));
    //qDebug()<< "NiceSlider:: zoom changed slot" << value << "Time Value" <<timeValue << "Pos ("<< p.x() <<", "<< p.y() <<")";

    QToolTip::showText(p,QString::number(timeValue/1000)+'s',this,QRect(),3000);
}

inline int NiceSlider::pick(const QPoint &pt) const
{
    return orientation() == Qt::Horizontal ? pt.x() : pt.y();
}

// Function copied from qslider.cpp and modified to make it compile
int NiceSlider::pixelPosToRangeValue(int pos) const
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

void NiceSlider::mouseReleaseEvent(QMouseEvent *ev){
    emit(zoomSliderReleased());
    QSlider::mouseReleaseEvent(ev);
}

void NiceSlider::mousePressEvent(QMouseEvent *e) {

//    qDebug("pressed (%d, %d)", e->pos().x(), e->pos().y());
    if (e->button() == Qt::LeftButton) {
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

            e->accept();

            int v = pixelPosToRangeValue(pick(e->pos() - center));
            setSliderPosition(v);
            triggerAction(SliderMove);
            setRepeatAction(SliderNoAction);

            //emit sliderMoved(v);//TODO: ok?
            emit sliderPressed(); //TODO: ok?
        } else {
            QSlider::mousePressEvent(e);
        }
    } else {
        QSlider::mousePressEvent(e);
    }
}

void NiceSlider::mouseMoveEvent(QMouseEvent *e)
{
//    qDebug("moved (%d, %d)", e->pos().x(), e->pos().y());
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    const QRect sliderRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    const QPoint center = sliderRect.center() - sliderRect.topLeft();
    // to take half of the slider off for the setSliderPosition call we use the center - topLeft

    if (!sliderRect.contains(e->pos())) {

        e->accept();

        int v = pixelPosToRangeValue(pick(e->pos() - center));
        triggerAction(SliderMove);
        setRepeatAction(SliderNoAction);

        emit mouseHover(v);
        //emit sliderMoved(v);//TODO: ok?
        //emit sliderPressed(); //TODO: ok?
        QSlider::mouseMoveEvent(e);
    } else {
        QSlider::mouseMoveEvent(e);
    }
}
