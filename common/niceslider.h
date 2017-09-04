#ifndef PIKDSLIDER_H
#define PIKDSLIDER_H

#include <QFrame>
#include <QObject>
#include <QSlider>

/*!
 \brief

*/
class NiceSlider : public QSlider
{
    Q_OBJECT

public:
    /*!
     \brief

     \param o
     \param parent
    */
    NiceSlider(Qt::Orientation o, QFrame *parent = 0);
    /*!
     \brief

     \param parent
    */
    NiceSlider(QWidget* parent = 0);

    /*!
     \brief

     \param e
    */
    void leaveEvent(QEvent *e);


    int lastValue; /*!< TODO: describe */

    void showZoom(int timeValue);

    /*!
     \brief

     \param value
     \param timeValue
    */
    void setZoom(int value, int timeValue);

protected:
    /*!
     \brief

     \param e
    */
    void mousePressEvent(QMouseEvent *e);

    void mouseMoveEvent(QMouseEvent *e);

    /*!
     \brief

     \param pt
     \return int
    */
    inline int pick(const QPoint &pt) const;
    /*!
     \brief

     \param pos
     \return int
    */
    int pixelPosToRangeValue(int pos) const;

    void mouseReleaseEvent(QMouseEvent *ev);
signals:

    /*!
     \brief

     \param int
    */
    void mouseHover(int);
    /*!
     \brief

    */
    void leave();

    void maxChanged();
    /*!
     \brief

    */
    void zoomSliderReleased();
};

#endif // PIKDSLIDER_H
