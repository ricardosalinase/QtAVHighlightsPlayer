#ifndef timeLine_H
#define timeLine_H

#include <QObject>
#include <QCursor>
#include <QPainter>
#include <QGraphicsItem>

/*!
 \brief Clase que representa una línea de tiempo

*/
class timeLine : public QObject,public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    /*!
     \brief Constructor

     \param parent
    */
    explicit timeLine(QGraphicsItem * parent = 0);

    /*!
     \brief Obtiene el rectángulo que envuelve el item

     \return QRectF
    */
    QRectF boundingRect() const;
    QLineF line; /*!< Parámetrización de la línea de tiempo */
    //QMap<int,hltLine> hltLines; /*!< TODO: describe */
    int height; /*!< Parámetro que define la altura de la línea */
    QColor barColor; /*!< Color utilizado para dibujar la línea*/

    /*!
     \brief Método para setear los parámetros de la línea

     \param m_p1 Primer punto
     \param m_p2 Segundo punto
     \param m_height Altura
    */
    void setView(QPointF m_p1, QPointF m_p2, int m_height);


    /*!
     \brief

     \param painter
     \param option
     \param widget
    */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    enum { Type = UserType + 1 };

   int type() const
   {
       // Enable the use of qgraphicsitem_cast with this item.
       return Type;
   }


signals:

    void release(qreal pos);

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // timeLine_H
