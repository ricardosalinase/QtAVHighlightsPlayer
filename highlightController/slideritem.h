#ifndef SLIDERITEM_H
#define SLIDERITEM_H

#include <QGraphicsItem>
#include <QCursor>
#include <QPainter>
#include <QTimer>

#include "roundedpolygon.h"

/*!
 \brief
*/
enum sliderZone
{
    LEFT,
    CENTER,
    RIGHT,
    NONE
};


/*!
 \brief

*/
class sliderItem : public QGraphicsObject
{
    Q_OBJECT

public:
    /*!
     \brief

     \param parent
    */
    explicit sliderItem(QGraphicsItem *parent = 0);

    sliderItem(const sliderItem&item);

    sliderItem& operator=(const sliderItem&);


    /*!
     \brief

     \return QRectF
    */
    virtual QRectF boundingRect() const;

    /*!
     \brief

     \param painter
     \param option
     \param widget
    */
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /*!
     \brief

     \return QPainterPath
    */
    virtual QPainterPath shape() const;


    /*!
     \brief

     \param x
     \param y
     \param w
     \param h
     \param wi
     \param limitLeft
     \param limitRight
    */
    void setSliderGeometry(QRectF m_rect, qreal bRatio, qreal m_width = 3.0, qreal m_bwidth = 6.0);

    /*!
     \brief

     \param init
     \param end
    */
    void setSlider(qreal init, qreal end);

    void setLocalLimits();

    QColor selColor; /*!< TODO: describe */
    QColor commonColor; /*!< TODO: describe */

    sliderZone currZone; /*!< Current Slider zone */

    //bool clicked; /*!< Slider is currently clicked*/

    bool selected; /*!< Slider is currently selected*/

    bool leftContained;
    bool rightContained;

    QLineF line;

    bool intersected;

    QSize dragPixmapSize;

    /*!
     \brief

     \param z
     \return qreal
    */
    qreal getSliderValue(sliderZone z);


//    /*!
//     \brief

//     \param currentLeft
//     \param currentRight
//     \param posX
//     \param centerCorrection
//    */
//    void proceedSetSlider(qreal currentLeft, qreal currentRight, qreal posX, bool centerCorrection);

    /*!
     \brief

     \param p
     \return bool
    */
    bool onLeftSliderFromScene(QPointF e);
    /*!
     \brief

     \param p
     \return bool
    */
    bool onRightSliderFromScene(QPointF e);
    bool getTickOnInit() const;

    enum { Type = UserType + 2 };

   int type() const
   {
       // Enable the use of qgraphicsitem_cast with this item.
       return Type;
   }

   QString getHltTime() const;

   void setFontSize(int fontsize);

   void drawText(QPainter &painter, qreal x, qreal y, Qt::Alignment flags, const QString &text, QRectF *boundingRect = 0);

   void drawText(QPainter &painter, const QPointF &point, Qt::Alignment flags, const QString &text, QRectF *boundingRect = 0);


   qreal getSliderLenghtFactor() const;

   //void setSpeed(const qreal &value);

   void setTimes(qint32 init, qint32 end);

   void setPath(const QString &value);

public slots:
   /*!
    \brief Set current Time and Duration of Highlight

    \param value Current Highlight Time
   */
   void setHltTime(QString value);
private:
    qreal toLeftDistance; //Parámetro para movimiento global del slider /*!< TODO: describe */
    qreal toRightDistance; //Parámetro para movimiento global del slider /*!< TODO: describe */
    QFont font;
    //Atributos para restricción de tamaño
    qreal limitSlidersDistance; /*!< Límite de ancho entre sliders */
    qreal local_limitLeft; /*!< Límite Local hacía la izquierda */
    qreal local_limitRight; /*!< Límite Local hacía la derecha*/
    qreal local_limitWidth; /*!< Límite Local de ancho*/
    qreal sliderButtonRatio; /*!< Slider Button Ratio*/


    QRectF rect; /*!< Rectángulo que define el slider */
    qreal sliderLengthFactor; /*!< Ancho del Rectángulo inscrito en la figura del slider */
    qreal radius; /*!< Radio de las esquinas del slider */
    qreal sliderWidth; /*!< Ancho del Slider*/
    qreal sliderButtonWidth; /*!< Ancho de los Botones del Slider*/
    //Posición en de cada uno de los Slider en la Barra a la que pertenece
    QMap<sliderZone,qreal> posSlider; /*!< TODO: describe */
    QString hltTime; /*!< TODO: describe */

    bool tickOnInit;
    qreal prevLeftValue;
    qreal prevRightValue;

    //edit on limits:
    //QTimer seekTimer;
    qreal posX;
    qreal lastSeek;
    bool leftLimitFlag;
    bool rightLimitFlag;

    QPoint dragStartPosition;
    //qreal speed;
    QString times;
    QString path;

    int textFactor; /*!< Text Rect Pos and Size Factor*/

    /*!
     \brief

    */
    void setBarPos();

    /*!
     \brief

     \param diff
     \return QString
    */
    QString convertSliderTimeToMMSS(qint32 diff);
    /*!
     \brief

     \param p
     \return bool
    */
    bool onLeftSlider(QPointF p);
    /*!
     \brief

     \param p
     \return bool
    */
    bool onRightSlider(QPointF p);
    void changeSlider();

    void getSliderPointList(RoundedPolygon &rPol) const;

    void getButtonRect(QRectF &r1, QRectF &r2) const;

    void createArrowPath(QPainterPath &path, const QPointF &p0, const QPointF &p1, const QPointF &p2);

    QRectF getInnerRectangle() const;
    QPixmap toPixmap() const;

    void initItem();

    void setSlidersToBack(QList<QGraphicsItem *> itemList);

    bool onCenterSlider(QPointF p);

signals:

    void sliderPress(bool m_isSelected);

    /*!
     \brief

     \param qreal
     \param qreal
     \param qreal
    */
    void checkLimitsSignal(qreal,qreal,qreal);
//    /*!
//     \brief

//    */
//    void positionChange();

    void sliderChange(bool limitReached = false);
    /*!
     \brief

    */
    void movingSlider();


    /*!
     \brief Seek Player to pos.

    */
    void modifyTimeByPos(qreal pos);

    /*!
     \brief Seek Player to pos in time.

    */
    void modifyTime(qint64 timeDiff, qreal initialPos);

protected:

//    /*!
//     \brief

//     \param e
//    */
//    void mousePressEvent(QGraphicsSceneMouseEvent *e);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SLIDERITEM_H
