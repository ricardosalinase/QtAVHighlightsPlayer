#ifndef PIKDTIMESLIDER_H
#define PIKDTIMESLIDER_H

#include <QFrame>
#include <QObject>
#include <QSlider>

/*!
 \brief Estructura que define un highlight o tick en el slider.
*/
struct tick{
    int id; /*!< Identificador del tick */
    QString typeName; /*!< Identifica el tipo de trick al que pertenece*/
    qint32 pos; /*!< Posición Central del tick*/
    qint32 lpos; /*!< Posición izquierda del tick*/
    qint32 rpos; /*!< Posición derecha del tick*/
    QRectF boundingRect; /*!< Región rectangular que describe el tick */
    bool selected; /*!< Indica si el elemento esta seleccionado*/
    bool over; /*!< Indica si el mouse esta sobre el elemento*/
    bool isRect; /*!< Indica si el tick debe ser dibujado como un rectángulo o como un tick */
    QList<qreal> marks; /*!< Marcas visuales en el tick */
};

/*!
 \brief Clase que implenta un slider con la capacidad de "marcar" distinas posiciones a lo largo de éste.
        Posee un vector con ticks con métodos para insertar y extraer elementos. Además de los procedimientos de dibujado.
*/
class NiceTimeSlider : public QSlider
{
    Q_OBJECT

public:
    /*!
     \brief Constructor.

     \param o Orientación del slider.
     \param parent Frame padre.
    */
    NiceTimeSlider(Qt::Orientation o, QFrame *parent);
    /*!
     \brief Constructor.

     \param parent Widget padre.
    */
    NiceTimeSlider(QWidget* parent);

    /*!
     \brief Inserta un tick al vector de ticks (pushback)

     \param p tick a insertar.
    */
    void insertTick(tick p, QVector<tick> &tickVector);
    /*!
     \brief Borra un tick de una posición específica.

     \param id Identificador del tick a borrar.
    */
    void deleteTick(int id, QVector<tick> &tickVector);
    /*!
     \brief Método de dibujado de los ticks.
            Recorre el vector y los va pintando uno por uno.
     \param p elemento de pintado de bajo nivel que contiene la región donde se va a dibujar.
    */
    void updateTick(tick p, int idx, QVector<tick> &tickVector);
    void drawTick(QPainter &p);
    /*!
     \brief Método para dejar un tick en modo seleccionado.
            Si hay otro seleccionado, lo deselecciona.
     \param id Identificador del tick.
    */
    void setTickSelected(int id);
    /*!
     \brief Método para señalar que un tick tiene el mouse encima.
     \param id Identificador del tick.
    */
    void setTickHovered(int id);

    /*!
     \brief Método para dejar un tick en modo seleccionado.
            Si hay otro seleccionado, lo mantiene seleccionado.
     \param id Identificar del tick
    */
    void setTickSelectedRelax(int id);
    /*!
     \brief Método para seleccionar varios ticks.
     \param idxs vector con los índices de los tick seleccionados.
    */
    void setSeveralTickSelected(QVector<int> idxs);

    /*!
     \brief Parámetro que permite transformar una posición del slider en unidades de la variable externa que mide el slider.

     \return qint64 rango.
    */
    int getExtUnit() const;
    /*!
     \brief Permite trasladar una posición del slider a la unidad de medición externa.

     \param value Valor del rango externo.
    */
    void setExtUnit(const int &value);

    qint32 getExtInitValue() const;
    /*!
     \brief

     \param extInitValue
    */
    void setExtInitValue(const qint64 &extInitValue);
    /*!
     \brief Define si se insertan y dibujan ticks en el slider.

     \param state estado verdadero o falso.
    */
    void setUseTicks(bool state);

    /*!
     \brief Borrar todos los ticks registrados en el vector de ticks.
    */
    void deleteAllTicks();


    /*!
     \brief

     \return qreal
    */
    qreal getTickWidth() const;
    /*!
     \brief

     \param value
    */
    void setTickWidth(const qreal &value);

    /*!
     \brief

     \return qreal
    */
    qreal getTickHeight() const;
    /*!
     \brief

     \param value
    */
    void setTickHeight(const qreal &value);

    /*!
     \brief

     \param widthRatio
     \param heightRatio
    */
    void updateTickGeometry(qreal widthRatio, qreal heightRatio);

    /*!
     \brief

     \param value
     \return int
    */
    int timeToSliderPos(const qreal &value);

    /*!
     \brief

     \param value
    */
    void setFirstDragParam(const QString &value);

    int lastValue; /*!< TODO: describe */

    QVector<tick> highlights; /*!< Ticks */


    //void setSpeed(const qreal &value);

    /*!
     \brief

     \param value
    */
    void setSrcDuration(const qint32 &value);

private:

    int m_extUnit; /*!< TODO: describe */
    qint64 m_extInitValue;
    qint32 srcDuration;
    QColor selectedColor; /*!< TODO: describe */
    QColor unselectedColor; /*!< TODO: describe */
    QColor aboveColor; /*!< TODO: describe */
    bool useTicks; /*!< TODO: describe */
    qreal tickWidth;
    qreal tickHeight;
    bool drawLargerTick;
    QPoint dragStartPosition; /*!< TODO: describe */
    QString dragTypeName; /*!< TODO: describe */
    QString firstDragParam; /*!< TODO: describe */
    QString dragTimes; /*!< TODO: describe */
    int dragIndexParam; /*!< TODO: describe */
    //qreal currentSpeed;

protected:

    /*!
     \brief

     \param e
    */
    void leaveEvent(QEvent *e) Q_DECL_OVERRIDE;
    /*!
     \brief

     \param e
    */
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    /*!
     \brief

     \param e
    */
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    /*!
     \brief

     \param e
    */
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;

    /*!
     \brief

     \param ev
    */
    void paintEvent(QPaintEvent *ev) Q_DECL_OVERRIDE;
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
signals:

    /*!
     \brief

    */
    void leave();
    /*!
     \brief

     \param id
    */
    void overTick(int id);
    /*!
     \brief

     \param id índice del tick
    */
    void tickClicked(int id);

    /*!
     \brief

     \param pos
     \param value
    */
    void onHover(int pos, int value);


};

#endif // PIKDTIMESLIDER_H
