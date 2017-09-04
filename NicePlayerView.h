#ifndef PIKDPLAYERCTL_H
#define PIKDPLAYERCTL_H

#include "PlayerView.h"
#include <QListWidgetItem>
#include <QObject>
#include <QPropertyAnimation>
#include "swtypes.h"

namespace QtAV {
    class AudioOutput;
    class AVError;
    class VideoRenderer;
    class VideoPreviewWidget;
    class AVWidgets;
    //class GraphicsItemRenderer;
}
class sliceFrame;
class SuggestionListWidget;
class PikdInfobox;
class PikdPlayerView : public PlayerView
{
    Q_OBJECT
    void setupUI();
public:
    explicit PikdPlayerView(QWidget *parent = 0);
    void initCustomView();
    /*!
     * \brief setParameters Setea los parámetros xml
     */
    void setParameters();
    ~PikdPlayerView();
    /**
     * @brief managePlayStatus Maneja el estado del player de acuerdo a su estado actual.
     * Estado:
     *  Stop o Pause -> emitir señal de play forzado.
     */
    void managePlayStatus();
    /**
     * @brief seekToEvent Actualiza los límites del slider de tiempo y avanza a la posición de inicio de este.
     */
    void seekToEvent();
    /**
     * @brief setMyPikdTree Setea el puntero para el modelo de árbol (Solo en modo edición por ahora).
     * @param value puntero al modelo de árbol.
     */
    void setEditorMode();
    /**
     * @brief setCurrentTicks Actualiza el mapa trick-tiempo actual
     * @param value mapa a actualizar.
     */
    void setCurrentTicks(const QMultiMap<QString, hltPar> &value);
    /**
     * @brief switchea la visualizacion de los componentes necesario para el playback de video standard
     * @param bool activar = true, desactivar = false
     */
    void toggleStdPlayer(bool active);
    /**
     * @brief switchea la visualizacion de los componentes necesario para el playback de video standard
     * @param bool activar = true, desactivar = false
     */
    void insertCustomSlider(QWidget *customSlider);
    /**
     * @brief toggleEditPlayer setea modo editor o modo normal de reproducción
     * @param active activar modo editor = true
     */
    void toggleEditPlayer(bool active);
    /**
     * @brief setEditEnabled muestra el botón de edición
     * @param visible mostrar = true
     */
    void setEditAndSaveEnabled(bool enabled);
    /**
     * @brief disableSearchMode Deshabilita el modo busqueda
     */
    void disableSearchView();

    void setSearchMode(bool value);

    bool getSearchMode() const;

    void blockPlaylistChange(bool isBlock);
    /**
     * @brief clearSuggestionListData Borra los datos de la lista de sugerencias de busqueda
     */
    void clearSuggestionListData();
    void setPromotion(QString prom);
    void setMediaMode(bool value);

    void initHltCtlUI();

    void insertHltCtl();
signals:
    /*!
     \brief Señal generada cuando se hace click en el botón para tomar una foto del punto actual de reproducción.

     \param qint64 es el punto actual de reproducción en [mseg].
    */
    void photoButtonClicked(qint64 );
    /*!
      * \brief Señal emitida cuando se hace click sobre el botón de editar.
    */
    void editButtonClicked();

    /*!
      * \brief Señal emitida cuando se hace click sobre el botón de compartir.
    */
    void shareButtonClicked();

    /*!
     * \brief backButtonClicked emitida cuando se hace click sobre el botón 'hacia atras'
     */
    void backButtonClicked();
    /*!
     * \brief applyButtonClicked emitida cuando se hace click sobre el botón 'aplicar'
     */
    void applyButtonClicked();
    /*!
     * \brief doSearch_dbInter Señal emitida para enviar una query al módulo de busqueda
     */
    void doSearch_dbInter(QString);
    /*!
     * \brief mediaButtonDropped Señal emitida cuando se dropea en el botón de media
     */
    void mediaButtonDropped(QString);
    /*!
     * \brief shareButtonDropped Señal emitida cuando se dropea en el botón de share
     */
    void shareButtonDropped(QString);

    /*!
     * \brief toggleSearchMode Señal emitida para mostrar o ocultar el modo busqueda
     */
    void toggleSearchMode(bool);
    void doSearch_promotion();
    void blockTimer();
public slots:
    /*!
     \brief Muestra todos los elementos de control de video.

    */
    void showAllControlItems();


    /*!
     * \brief setROItimes Permite definir el par de tiempos de interes de video
     * \param times tiempos en milisegundos
     */
    void setROItimes(hltPar times);
    void pauseForcedSlot();
    void playFromPauseForcedSlot();
    void initSuggestionList(const QStringList &sList);

protected:
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *e);
    void dropEvent(QDropEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);
    void dragLeaveEvent(QDragLeaveEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *event);
protected slots:
    void statusChanged(QtAV::MediaStatus status);
    /*!
     \brief Cambio los límites actuales de la línea de tiempo.

     \param init Límite inicial en [mseg].
            end Límite final en [mseg]
    */
    void sliderLimitsChangeSlot(qint64 init, qint64 end);
private slots:
    /*!
     \brief Maneja el click del botón "hacia atras"
    */
    void backwardButtonClickedSlot();

    /*!
     \brief Maneja el click del botón "hacia adelante"
    */
    void fordwardButtonClickedSlot();

    /*!
     \brief Maneja el click del botón "Editar"
    */
    void editButtonClickedSlot();
    void mediaButtonClickedSlot();
    void shareButtonClickedSlot();
    void backButtonClickedSlot();
    void applyButtonClickedSlot();
    /*!
     \brief Maneja el comportamiento cuando se selecciona un tick o highlight del slider de tiempo.

     \param idx índice del tick o highlight seleccionado.
    */
    void tickClickedSlot(int idx);
    void stopProgressBar();
    /**
     * @brief temporalSeek Avanza o retrocede a la posición especificada.
     * @param newTime posición objetivo.
     */
    void temporalSeek(qint64 newTime);
    void seekAndPause(bool flag);

    void seekAndPlay(qint64 pos);

    /*!
     \brief Maneja el comportamiento del widget cuando se espera obtener una foto del archivo multimeda que se esta reproduciendo.
    */
    void takePhotoSlot();
    /**
     * @brief updateTicksOnViewFromMap Actualiza la lista de ticks que se van a dibujar en la barra de tiempo de acuerdo a un mapa de trick-tiempo
     * @param times MultiMapa del par trick-tiempo
     */
    void updateTicksOnViewFromMap(const QMultiMap<QString, hltPar> &times);
    /*!
     * \brief Set time for video item (drag option)
     * \param time init - end times of current track
     */
    void updateVideoItemTimes(hltPar time);
    void updateSlot(hltPar time, bool seekToInit);

    void setLimitsEditor();
    void setFrameGeometry();
    void doSearch_dbInterSlot( const QString &str );
    void setAndSendText(QListWidgetItem *item);
    void enableButtons();
private:
    //coming soon ( no borrar)
    //void fadingOutWidget(QLineEdit **w);
//    void fadingInWidget(QLineEdit **w);
    QPropertyAnimation *animation;
    SearchFrame *searchFrame;
    QLabel *gif_anim; /*!< Label para mostrar animación de progreso */
    PikdInfobox *infoBoxLabel; /*! Usado para mostrar información al usuario en forma de label de texto */
    QPushButton *shareButton; /*!< Botón para compartir item*/
    QPushButton *mediaButton;
    QPushButton *backwardButton;/*!< Botón para retroceder un item en el playlist */
    QPushButton *fordwardButton;/*!< Botón para avanzar un item en el playlist */
    QPushButton *photoButton; /*!< Botón para extraer una foto desde el video. */
    QPushButton *editButton; /*!< Botón para editar item */
    QPushButton *backButton; /*!< Botòn para volver atras */
    QPushButton *applyButton;  /*!< Botòn para aplicar cambios de edicion */
    SuggestionListWidget *sugListWidget; /*!< List de Sugerencias en la busqueda */
    QGraphicsProxyWidget *proxyEditButton;  /*!< Proxy Widget para el botón de editar*/
    QGraphicsProxyWidget *proxyShareButton; /*!< Proxy Widget para el botón de compartir*/
    QGraphicsProxyWidget *proxyFordwardButton; /*!< Proxy Widget para la flecha hacia adelante*/
    QGraphicsProxyWidget *proxyBackwardButton; /*!< Proxy Widget para la flecha hacia atras*/
    QGraphicsProxyWidget *proxyMediaButton; /*!< Proxy Widget para el botón de media */
    QGraphicsProxyWidget *proxyShowListButton; /*!< Proxy Widget para el botón para mostrar la lista */
    QGraphicsProxyWidget *proxySearchFrame; /*!< Proxy Widegt para el el frame de busqueda */
    QGraphicsProxyWidget *proxyInfoLabel; /*!< Proxy Widget para el label de información */
    QGraphicsProxyWidget *proxySuggestionListWidget; /*!< Proxy widget para la lista de sugerencias de busqueda */
    QGraphicsProxyWidget *proxygif_anim;
    QGraphicsEllipseItem *elliShare;
    QGraphicsEllipseItem *elliMedia;

    bool editorMode; /*!< Flag que indica si esta habilitado modo editor*/
    bool searchMode; /*!< Flag que indica si se encuentra el player en modo busqueda*/
    bool mediaMode;  /*!< Flag que indica si se encuentra el player en reproduccion de clips de media*/
    bool dragging; /*!< Flag que indica si actualmente se esta llevando a cabo una operación de drag*/
    bool backFlag; /*!< Flag que indica cuando se hizo click en el botón 'hacia atras' */
    bool applyFlag; /*!< Flag que indica que se aplicara cambio en edicion de item */
    bool editFlag; /*!< Flag para indicar que se va a editar el item*/
    //Parameters
    bool m_editorDefByUser; /*!< Geometría del editor definida por el usuario */
    int m_editorBottomMargin; /*!< Margen entre la parte inferior del player y la posición del editor */
    qreal m_editorHeightFactor; /*!< Factor entre el alto de la escena y el editor */
    qreal m_editorWidthFactor; /*!< Factor entre el ancho de la escena y el editor */

    QMultiMap< QString, hltPar > currentTicks; /*!< Lista de Ticks actuales que se dibujan en el player*/

    void initProxyButton(QPushButton **btn,QGraphicsProxyWidget **proxy, QSize size, const QString &style,QWidget *parent = 0, bool checkeable = false);
    void setupPikdScene();
    void setupPikdPlayer();
    void deleteProxyWidgets();

    void hideDragWidgets();
    /**
     * @brief setArrowVisible Ocultar/Mostrar flechas de cambio de video dentro del player.
     * @param isVisible
     */
    void setArrowVisible(bool isVisible);
    /**
     * @brief updateCurrentTickTree Actualiza el tick indicado (Modo Edición ).
     * @param time nuevo tiempo
     * @param idx índice
     */
    void updateCurrentTickTree(const hltPar &time, int idx);
    /*!
     \brief Verifica que una posición de tiempo se encuentre dentro de los límites del slider de tiempo actual.

     \param pos Posición de tiempo a verificar

     \return bool Verdadero si la posición de tiempo 'pos' se encuentre entre 'startTime' y 'endTime' del slider de tiempo.
    */
    bool checkTickInTimeLine(qreal pos);

};

#endif // PIKDPLAYERCTL_H
