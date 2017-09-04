#ifndef PIKDDRAGITEMRENDERER_H
#define PIKDDRAGITEMRENDERER_H

#include <QtAVWidgets>
#include <QObject>

/*!
 \brief

*/
class niceDragItemRenderer : public QtAV::GraphicsItemRenderer
{
    Q_OBJECT

public:
    /*!
     \brief

     \param item
    */
    niceDragItemRenderer(QGraphicsItem *item = 0);

    /*!
     \brief

     \param value
    */
    void setModelIndex(int value);

    /*!
     \brief

     \param value
    */
    void setPath(const QString &value);

    /*!
     \brief

     \param value
    */
    void setPixmapSize(const QSize &value);

    /*!
     \brief

     \param value
    */
    void setSpeed(const qreal &value);

    /*!
     \brief

     \param init
     \param end
    */
    void setTimes(qint32 init, qint32 end);
    /*!
     * \brief getMimeDataText get object data to external use
     * \return QString mime data of "path,times"
     */
    QString getMimeDataText();

signals:
    /*!
     \brief

    */
    void videoRendererClicked();
protected:
    /*!
     \brief

     \param event
    */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     \brief

     \param event
    */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     \brief

     \param event
    */
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
private:

    //qreal speed; /*!< Speed */
    int modelIndex; /*!< Model index */
    QString path;   /*!< Video Path */
    QString times; /*!< Time Parameters */
    QSize dragPixmapSize;
    QPointF dragStartPosition;
    QPixmap toPixmap() const;
};

#endif // PIKDDRAGITEMRENDERER_H
