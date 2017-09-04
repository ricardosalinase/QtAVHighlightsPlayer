#ifndef HLTCTLVIEW_H
#define HLTCTLVIEW_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "nicexml.h"
#include "timeline.h"
#include "slideritem.h"
#include "trickitem.h"

/*!
 \brief Clase que modela el controlador de highlights para la aplicación de Escritorio.
 Permite hacer una vista zoomeada de alguna zona del reproductor de video, puede editar highlights,
 se tienen que ver de alguna forma los highlights (Lista de highlights), se tiene que ver la posición
 actual del reproductor.
*/
Q_DECLARE_METATYPE(sliderItem)
QDebug operator<<(QDebug dbg, const sliderItem &item);

class HltCtlView : public QGraphicsView
{
    Q_OBJECT

    QGraphicsScene *scene; /*!< Escena */
    QGraphicsLineItem *tickItem; /*!< Tick Item */
    timeLine *zoomedTimeLine; /*!< Línea de tiempo*/
    //QVector< sliderItem* > hltSliders; /*!< Lista de Highlights en el Zoom */
    sliderItem *myHltSlider;
    QVector< trickItem* > circleItems; /*!< Lista con los circulos que representan las marcas en la imágen*/
    QVector< QGraphicsPixmapItem* > thumbItems; /*!< Lista de Thumbnails */

public:
    HltCtlView(QWidget *parent = 0);
    ~HltCtlView();

    /*!
     \brief Inicialización del Controlador de Highlights
            Parámetros xml, dimensiones de los objetos de la escena y construcción de la escena.
    */
    void initHltController(niceXml *reader);//Genera los parámetros de la escena para poder ser dibujada

    /*!
     \brief Inicializa la escena

    */
    void setSceneView(); //Genera los parámetros de la escena para poder ser dibujada

//    /*!
//     \brief Retorna la posición del slider en unidades de tiempo de acuerdo a la zona que se asigne (Derecha, izquierda o centro)

//     \param z (LEFT, CENTER, RIGHT, NONE)
//     \return qint32 posición del slider en unidades de tiempo
//    */
//    qint32 getSliderTimePos(sliderZone z);

    /*!
     \brief Valor de zoom actual (Rango).

     \return int zoom actual en unidades de tiempo.
    */
    qint64 getScale();



//    /*!
//     \brief Bloquea la modificación del área zoomeada si el inicio o final de ésta, sobrepasa o es inferior, respectivamente, al inicio y final. del Slider.

//    */
//    void checkValidZoomModification();

//    /*!
//     \brief Recupera los índices de los highlights que cumplen las condiciones para ser mezclados

//     \return QPair<int, int> Par de índices de los highlights que deben ser unidos.
//    */
//    QPair<int,int> getIndexesforMerge();

    /*!
     \brief Emite señal para generar la separación de un highlight.

    */
    void splitByTick();

    /*!
     \brief Obtiene la velocidad actual de reproducción.

     \return qreal
    */
    qreal getVelRep() const;
    /*!
     \brief Asigna la velocidad actual de reproducción y actualiza la vista que lo muestra.

     \param value
    */
    void setVelRep(const qreal &value);

    /*!
     \brief Obtiene estado del flag de split.

     \return bool verdadero = se puede hacer seperar el highlight.
    */
    bool getSplitState() const;
    /*!
     \brief Setea el estado de split para el controlador.

     \param value estado de split.
    */
    void setSplitState(bool value);

    /*!
     \brief Obtiene el estado de merge de highlights.

     \return bool estado de merge.
    */
    bool getMergeState() const;
    /*!
     \brief Setea el estado de merge del highlight.

     \param value estado de merge.
    */
    void setMergeState(bool value);

    /*!
     \brief Obtiene la posición actual del reproductor.

     \return qint32 posición del tick en unidades de tiempo.
    */
    qint64 getCurrentTickPos() const;


    //when image is ready init the thumbnails for the timezoomedline

    /*!
     \brief Actualiza el tiempo de los thumbnails y emite señal para obtenerlos.
    */
    void updateThumbs();
    /*!
     \brief Entrega las dimensiones de un thumbnail.

     \return QSize Dimensiones del Thumbnail.
    */
    QSize getSliderThumbSize();
    /*!
     \brief Inicializa los thumbnails en la escena.

    */
    void setThumbScene(qreal m_thumbHeight);
//    /*!
//     \brief Inicializa la lista de highlights.

//     \param l Lista de highlights.
//    */
//    void setHltsList(videoSportData *l);


    void updateSceneGeometry();

    int getThumbNum() const;

    qreal getZoomFactor() const;


    int getThumbInHlt() const;

    void setXmlSettings(niceXml *value);

    /*!
     \brief Asignar valores a los parámetros de la clase mediante el documento xml.

    */
    void setParameters();

    void setPlayerTimeStampData(const qint64 &init, const qint64 &end);

    qint64 getDuration() const;

    void setUpdateTickFlag(bool value);

    qint64 getMinimalZoom() const;

    void setHltCtlSize(int neighWidgetHeight, qreal ratioToNeighWidget);
    void setPath(const QString &value);

    hltPar getEditedHlt() const;

    void setTrickPool(const QMultiMap<QString, hltPar> &tp);

    bool getUseThumbList() const;

public slots:


    /*!
     \brief Verify sliders for merge.
    */
    void verifyMerge();

//    /*!
//     \brief

//     \param z
//     \return bool
//    */
//    bool getBlockFlag(sliderZone z);

    /*!
     \brief

     \param idx
     \param s
     \return bool
    */
    bool addHltToZoomedTimeLine();//Agrega Highlight al aárea de Vista
    /*!
     \brief

    */
    void updateZoomedViewOfHighlights();//Actualiza la lista de Highlights en la vista zoomeada
//    /*!
//     \brief

//     \param step
//     \param changeInit
//    */
//    void changeLimitOfZoomedTimeLine(qint32 step, bool changeInit);//Actualiza los Limites de la Vista Zoomeada

    /*!
     \brief

     \param value
     \return qint32
    */
    qint64 zoomedTimeLineTotimeValue(qreal value);

    //Update Tick
    /*!
     \brief Actualiza la posición del tick de reproducción en el Highlight Controller View.
            Si es necesario actualiza el zoom actual.

     \param pos Posición en tiempo donde se actualizará el tick de reproducción.
     \param first índica si solo hay que actualizar el zoom.
    */
    bool updateTick(qint64 pos);

    //update Thumbnails
    /*!
     \brief

     \param thumbsFromHlt
    */
    void setOneThumbnail(const QPixmap &pix, int idx);

//    /*!
//     \brief

//    */
//    void setHltTimeOnSlider();


signals:
    /*!
     \brief

     \param qint32
    */
    void changeTickPos(qint64);
    /*!
     \brief

     \param qint32 posición del highlight
    */
    void splitHltTree(qint32);
    /*!
     \brief

    */
    void mergeHlt();

    /*!
     \brief Señal que es emitida cuando se requiere actualizar la lista de thumbnails que son dibujados en el área zoomeada

     \param QVector<int> Vector con enteros que representan los tiempos de los thumbnails que se necesitan para
            actualizar la vista zoomeada.
    */
    void updateThumbSignal(QVector<int>);

    /*!
     \brief

    */
    void leave();
    /*!
     \brief

    */
    void enter();
    /*!
     \brief

     \param value
    */
    void onTimeLine(int value);

    void zoomChanged(int );
    //void zoomTimesChanged(int,int);

    void sliderModified(hltPar );
    void highlightModified(hltPar);
    void sliderSelectedFinished(hltPar);
    void pauseSignal_player(bool);

    void seekTemporalEdiToPlayer(qint64);

    void selectTickFinished(qint64 );

    void sliderSelected();

    void showSignal();
    void hideSignal();

private:

    //Párametros configurables
    int m_thumbNum; /*!< Número de Thumbnails en la barra zoomeada */
    qreal m_zoomFactor; /*!< Factor de zoom cuando se muestra un Highlight centrado*/
    qreal m_widthHltFact; /*! Factor para asignar el ancho del Highlight en el área zoomeada (para obtener el ancho se multipla por el alto del highlight)*/
    bool m_onInitTickOutsidePos;
    int m_sliderFontSize; /*!< Tamaño de la letra del tiempo en los sliders*/
    qreal m_barWidth; /*!< Ancho del Pincel con el que se pinta la Barra de Tiempo */
    qreal m_circlesVerticalPos; /*!< Posición vertical de los circulos en la barra de tiempo */
    QSize m_trickIconSize; /*!< Tamaño de los iconos que representan los tricks */
    qreal m_sliderHeightFactor; /*!< Porcentaje del alto de la escena que ocupa slider junto a los thumbnails */

    niceXml *xmlSettings; /*!< XML Reader para parámetros externos */

    //Run Slider Aproach
    //FatherSlider *editStR;
    //FatherSlider *editEtR;
    highlightData editedHlt; /*!< Información del highlight que se esta editando actualmente */

//    qreal stretchWidth; /*!< Factor de tamaño para las posiciones iniciales del rectangulo que define la barra de tiempo */
    qreal stretchHeight;  /*!< Factor de tamaño para el LARGO de la Barra de Tiempo */
    qreal stretchBar; /*!< Factor de tamaño para el ANCHO de la Barra de Tiempo */
    qreal widthZHlt; /*!< Ancho de la Barra de Tiempo Aumentada */

    qint64 initZoomedTimeLine; /*!< Inicio de la linea de tiempo aumentada en medidas de tiempo */
    qint64 endZoomedTimeLine; /*!< Fin de la linea de tiempo aumentada en medidas de tiempo */
    qint64 prevInitZoomedTimeLine; /*!< Valor previo al último cambio del inicio de la linea de tiempo aumentada */
    qint64 prevEndZoomedTimeLine; /*!< Valor previo al último cambio del fin de la linea de tiempo aumentada */
    qint64 initPlayerTimePos; /*!< Inicio de la línea de tiempo global (Línea de tiempo a la que se le esta haciendo Zoom) */
    qint64 endPlayerTimePos; /*!< Fin de la línea de tiempo global (Línea de tiempo a la que se le esta haciendo Zoom)*/
    qint64 minimalZoom; /*!< Mínimo zoom que se puede hace a la vista zoomeada.*/
//    qint64 initFatherTimePos;
//    qint64 endFatherTimePos;

    QPointF initPosZoomedTimeLine; /*!< Posición inicial en la Barra de Tiempo Global */
    QPointF endPosZoomedTimeLine; /*!< Posición Final en la Barra de Tiempo Global */

    QColor backColor;/*!< Color del Fondo */
    QColor barColor; /*!< Color del item de la barra de tiempo */
    QColor hltColor; /*!< Color de los Highlights*/

    qreal sliderHeight; /*!< TODO: describe */

    //Thumb Parameters
    bool useThumbList; /*!< flag for set thumbnail highlightcontroller */
    qreal thumbWidth; /*!< Ancho de los Thumbnails */
    qreal thumbHeight; /*!< Alto de los Thumbnails */

    //init zoom Parameters

    qreal initSliderPosInTimeLine;
    qreal endSliderPosInTimeLine;


    //Highlights View Parameters
    qreal m_sliderWidth; /*!< Ancho del slider*/
    qreal m_sliderButtonWidth; /*!< Ancho del botón del slider*/

    bool firstTick; /*!< Flag para determinar si es la primera vez que se actualiza el tick de tiempo */
    qint64 currentTickPos; /*!< Posición actual del Tick de tiempo */
    qreal pixelPosition; /*!< Posición en pixeles del tick  */
    bool updateTickFlag; /*!< Flag de actualización del tick*/

    bool tickOnInit; /*!< Flag que indica si el tick de tiempo debe estar al inicio del highlight */

    bool splitState;  /*!< Señala si se esta en modo normal o modo plit highlihgts */
    bool mergeState; /*!< Especifíca si los highlight que se solapan deben unirse  */

    bool editingSlider; /*!< Editando slider flag*/

    qreal zoomFactor; /*!< Factor de Zoom del highlight en relación al área zoomeada */

    QPoint dragStartPosition;
    QList< trickItem* > tricksToDrag;
    QMultiMap<QString,hltPar> trickPool;
    QList< QPixmap > pixToDrag;
    QString path;

    //Métodos
    /*!
     \brief

     \return bool
    */
    bool equalZoomedLimits();
    /*!
     \brief

     \param timeValue
     \return bool
    */
    bool valueInZoomedZone(qint64 timeValue);

    /*!
     \brief

    */
    void drawBackground();
    /*!
     \brief

     \param init
     \param end
    */
    void setZoomedLimitValues(qint64 init, qint64 end);//Set Zoomed Limit Values

    /*!
     \brief

    */
    void initThumbItems();

    /*!
     \brief

     \param timeValue
     \return qreal
    */
    qreal timeToZoomedTimeLine(qint64 timeValue);

    /*!
     \brief

     \param value
     \return qint64
    */
    qint64 zoomedAreaToTime(qreal value);

    /*!
     \brief

     \param diff
     \return QString
    */
    QString convertSliderTimeToMMSS(qint64 diff);
//    /*!
//     \brief

//    */
//    void debugCurrentZoomList();

    bool valueInPlayerTimeZone(qint64 timeValue);

    bool valueInCurrentHighlight(qint64 timeValue);

    void paintTimeTick(qreal pos);

    /*!
     \brief Add Line Item like a Highlight

     \param hlt Line parameters
     \param idx Highlight Index
     \param selected True if is Selected
     \param contained True if is contained in current Zoom
    */
    void updateSlider(QLineF hlt, bool lcc = true, bool rcc = true);

    /*!
     \brief Delete all sliders from scene

    */
    void deleteSlider();

    /*!
     \brief Delete all Circles From Scene

    */
    void deleteCircles();

    QRectF getHighlightRectFromLine(QLineF m_line);

    sliderItem *getSliderByPos(QPoint pos);

    QList<sliderItem *> getSlidersFromList(QList<QGraphicsItem *> itemList);

    qreal getMaxZValueFromSliders(QList<sliderItem *> sliderList);

    void setSlidersToBack(QList<QGraphicsItem *> itemList);

    bool pixelTransitionOnRightSlider(qreal pos);
    bool pixelPosOnRightSlider(qreal pos);
    bool pixelPosOnLeftSlider(qreal pos);
    bool pixelPosOnSlider(qreal pos);

    bool hltParInZoommedArea(hltPar t);

    hltPar getZoomLimitsFromCurrentSelect(hltPar time);

    void insertCircle(qreal x, qreal y, QString trick, bool isActive, int idx);

    trickItem *getTrickByPos(QPoint pos);

    trickItem *getTrickByKey(int p_key);

    QList<QPixmap> trickDragListToPixmapList();

    void initSlider();

    void insertItemCircles();
    void updateView();
    void setNewZoom(hltPar plTime);
private slots:

    /*!
     \brief Set Initial Pos for Zoomed Area

    */
    void setInitialZoomedAreaPos(qint64 pos);
    void updateEditedHltData(highlightData m_editedHlt);
    void selectCurrentHlt();

    void updateCurrentChildTime(hltPar time);
    void onTimeLineSlot(qreal pos);

    void sliderChangeSlot(bool limitReached = false);

    void sliderMoveSlot();

    void temporalSeekToPlayer(qreal pos);
    void temporalSeekToPlayerByDiff(qint64 step, qreal initialPos);

    void durationChanged(qint64 init, qint64 end);

    void sliderPressEvent(bool isSelected);
protected:
//    /*!
//     \brief

//     \param object
//     \param event
//     \return bool
//    */
//    bool eventFilter(QObject *object, QEvent *event) Q_DECL_OVERRIDE;


    /*!
     \brief

     \param event
    */
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    /*!
     \brief

     \param event
    */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     \brief

     \param event
    */
    void enterEvent(QEvent * event) Q_DECL_OVERRIDE;

    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    void hideEvent(QHideEvent *e) Q_DECL_OVERRIDE;

    //void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

};




#endif // HLTCTLVIEW_H
