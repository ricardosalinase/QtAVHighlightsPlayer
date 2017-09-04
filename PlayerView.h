
#ifndef NICEPLAYERVIEW_H
#define NICEPLAYERVIEW_H

#include <QGraphicsView>
#include <QTimer>
#include <QtAV/AVPlayer.h>
#include "highlightController/HltCtlView.h"

QT_BEGIN_NAMESPACE
class QtGui;
class QGraphicsScene;
class QGraphicsProxyWidget;
class QGraphicsTextItem;
class QLabel;
class QSpacerItem;
class QSlider;
class QPushButton;
class QListWidget;
QT_END_NAMESPACE
class niceXml;
class NiceTimeSlider;
class upWardComboBox;
class CustomPushButton;
class NiceSlider;
class niceDragItemRenderer;
class DropButton;
class sliceFrame;
class SearchFrame;
class BorderFrame;

namespace QtAV {
    class AudioOutput;
    class AVError;
    class VideoRenderer;
    class VideoPreviewWidget;
    class AVWidgets;
    //class GraphicsItemRenderer;
}

/*!
 \brief

*/
enum timeSliderPos{
    TOP_POS ,
    CENTER_POS ,
    BOTTOM_POS
};

/*!
 \brief Clase widget que permite generar un reproductor de video basado en QtAV y QObject.
        Posee señales y slots como interfaces para un controlador externo.
*/
class PlayerView : public QGraphicsView
{
    Q_OBJECT

public:
    /*!
     \brief

     \param parent es el widget padre de la clase.
    */
    PlayerView(QWidget *parent = 0);

    /*!
     \brief Método destructor.

    */
    ~PlayerView();

    /*!
     \brief Inicialización de la vista y los parámetros necesarios.
     \param xmlReader reader Lector de parámetros xml.
    */
    bool initView(niceXml *reader);
    void setupTopWidget(QWidget *w, bool useMoveButtons = true);
    void setCurrentIndex(int idx);

    /*!
     \brief Este método entrega la posición actual del reproductor.

     \return qint32 valor entero de 32 bits que devuelve la posición actual del reproductor.
    */
    qint64 currentPosition();

    //QMediaPlayer *player;
    QtAV::AVPlayer *player; /*!< Puntero que representa el reproductor basado en QtAV */
    sliceFrame *playlistFrame; /*!< Slice Frame que contiene el playlist actual */
    HltCtlView *hltCtl; /*!< Slider editor de highlights */

    qint64 startTime; /*!< Tiempo inicial de reproducción */
    qint64 endTime; /*!< Tiempo final de reproducción */
    /*!
        \brief Obtiene el último estado del reproductor de video. Los estados pueden ser: Detenido, Reproduciendo y Pausado.
    */
    QtAV::AVPlayer::State getPlayerState() const;

    /*!
    \brief La posición del reproductor avanza un "frame" hacia delante o hacia atras.
     \param isForward Para especificar si la posición del reproductor se mueve un frame hacia adelante (true) o hacoa atras (false).
    */
    void seekFrame(bool isForward);

    /**
     * @brief Para habilitar la línea de tiempo global en el reproductor.
     */
    void enableTimeSlider();

    /**
     * @brief Para deshabilitar la línea de tiempo global en el reproductor.
     */
    void disableTimeSlider();
    /**
     * @brief loadSpeed Interpreta el parámetro de entrada para setearlo (si existe) en el combobox de velocidad (rateBox)
     * @param speed velocidad que se va a cargar en el combobox
     */
    void loadSpeed(qreal speed);

    /**
     * @brief setXmlSettings Setea el puntero para el archivo de configuración.
     * @param value puntero al objecto xml.
     */
    void setXmlSettings(niceXml *value);

    /**
     * @brief setParameters Configura los parámetros de la clase que provienen de un archivo externo.
     */
    void setParameters();
    /**
     * @brief setPath Setea la ruta del video que se esta reproduciendo actualmente.
     * @param value ruta absoluta actual del video.
     */
    void setPath(const QString &value);
    /**
     * @brief setRepeat Maneja el estado de repetición para el video que se esta reproduciendo actualmente.
     * @param value repetición.
     */
    void setRepeat(bool isRepeated);
    /**
     * @brief setCloseButtonEnabled Habilita el botón de cerrado en la UI.
     * @param value verdadero: mostrar botón, falso: ocultar botón.
     */
    void setCloseButtonEnabled(bool value);
    /**
     * @brief setTrackPosition Setea los límites de la sección actual de video que se esta reproduciendo.
     * @param times Tiempos en ms.
     */
    void setTrackPosition(qint32 start, qint32 end);
    /**
     * @brief seekToEvent Actualiza los límites del slider de tiempo y avanza a la posición de inicio de este.
     */

    virtual void seekToEvent();
    void setOpenGlViewport();

    /*!
     \brief

    */
    void showBasicControlItems();

    /*!
     \brief

    */
    void showAdvancedControlItems();

    void initHltCtlUI();

protected:
    /*!
     \brief Evento que maneja el comportamiento cuando el widget cambia de tamaño.
            Actualiza el tamaño de la escena, el item de video y el frame donde se encuentran los controles de video.
     \param event es una instancia de QResizeEvent.
    */
    void resizeEvent(QResizeEvent *event);

    /*!
     \brief Evento que maneja el comportamiento cuando el mouse se mueve sobre el widget.
            Esconde el frame donde se encuentra los controles de video.
     \param e es una instancia de evento proveniente del mouse.
    */
    void mouseMoveEvent(QMouseEvent *e);

    /*!
     \brief Evento que maneja el comportamiento cuando se presiona una tecla del teclado.
            Permite salir del modo pantalla completa si se encuentra en ese estado y se presiona la tecla ESCAPE.
     \param e es una instancia de evento proveniente del teclado.
    */
    void keyPressEvent(QKeyEvent *e);
    /*!
     \brief Evento que maneja el comportamiento cuando se hace release del botón del mouse.
            Maneja los flag de cerrado y edición.
     \param e es una instacia de evento.
    */
    void mouseReleaseEvent(QMouseEvent *e);

    void initButton(QPushButton **btn, QSize size, const QString &style, QWidget *parent = 0, bool checkeable = false);
    /*!
     \brief

     \param foo
    */
    void freeWidget(QWidget *foo);
    /*!
     \brief Verifica que una posición de tiempo se encuentre dentro de los límites del slider de tiempo actual.

     \param pos Posición de tiempo a verificar

     \return bool Verdadero si la posición de tiempo 'pos' se encuentre entre 'startTime' y 'endTime' del slider de tiempo.
    */
    bool setSliderLimitTimes(qint32 m_startTime, qint32 m_endTime);
    QRectF visibleRect();
    /*!
     \brief Muestra mensaje de error.

     \param n_error número del error.
    */
    void displayErrorMessage(QtAV::AVError n_error);

    /*!
     * \brief toggleCloseButtonStyleSheet Change stylesheet of close button between large and 'X' desing
     * \param largeStyle largeStyle = true -> large design
     */
    void toggleCloseButtonStyleSheet(bool largeStyle = false);

    /*!
     \brief Transforma la posición actual del reproductor en formato "HH:mm:ss" y lo ingresa como texto al label de visualización.
    */
    void setCurrTimeView(qint64 value);

    niceDragItemRenderer *videoItem; /*!< Renderizador de video, en este caso es un QGraphicsItem que es insertado en la escena.*/
    QGraphicsScene *scene; /*!< Escena donde se cargarán los diferentes elementos gráficos que componen el reproductor.*/
    /**
     * @brief visibleRect entrega el rectángulo visible de la vista.
     * @return rectángulo que representa el rectángulo visible de la vista.
     */
    //Time Slider Item
    NiceTimeSlider *timeSlider; /*!< Puntero que representa la barra de tiempo del reproductor */
    BorderFrame *frame; /*!< Aquí se insertan los diferentes widgets que componen y manejan el control de video. */
    QGraphicsProxyWidget *proxyFrame; /*!< Proxy que toma el widget (frame) donde se insertan los diferentes widgets que componen y manejan el control de video.*/
    QGraphicsProxyWidget *proxyCloseButton; /*!< Proxy Widget para el botón de cerrado*/
    QGraphicsProxyWidget *proxyPlaylist; /*!< Proxy Widget para el frame que contiene el playlist actual */

    QPushButton *maxButton;
    //Controls
    QPushButton *playButton; /*!< Botón de "play" y "pause".*/
    QPushButton *volumeButton; /*!< Botón de "volumen". */
    NiceSlider *volumeSlider; /*!< Barra deslizante para controlar el volumen del reproductor. */
    QLabel *currentPosLabel; /*!< Label para especificar la posición actual del reproductor y la duración en forma <tiempo_actual>/<duración>*/
    upWardComboBox *rateBox; /*!< Caja que muestra la lista con las diferentes velocidades de reproducción posibles. */
    CustomPushButton *closeButton; /*!< Botón para emitir señal de salida*/

    QLayout *l; /*!< Layout que maneja la separación y alineación de los elementos de control de video*/

    niceXml *xmlSettings; /*!< Archivo de configuración externo */

    bool temporalSeekFlag;
    bool closeButtonEnabled; /*!< Flag que indica que esta habilitado el botón de close*/

    //Player
    QtAV::AVPlayer::State playerState; /*!< Almacena el estado actual del reproductor (Pausado, Reproduciendo o Detenido). */
    QString path; /*!< Ruta al video actual*/
    int m_unit; /*!< Unidad que representa el intervalo de notificación del reproductor. */
    qint64 startTrackPos; /*!< Posición inicial del slider */
    qint64 endTrackPos; /*!< Posición final del slider */
    qint64 startTimeROI; /*!< Tiempo inicial segmento de interes del video */
    qint64 endTimeROI; /*!< Tiempo final segmento de interes del video */

//    void keyReleaseEvent(QKeyEvent *e);
private:

    QGraphicsProxyWidget *proxyPreviewWidget; /*!< Proxy widget que toma el widget que almacena el thumbnail (imagen pequeña) del frame actual donde se encuetra el cursor.*/

//    SearchFrame *searchFrame;
    QSpacerItem *sp1; /*!< Separador para los elementos de control de video. */
    QSpacerItem *sp2; /*!< Separador para los elementos de control de video. */

    QLayout *optL; /*!< Layout opcional en caso de que se use el time slider sobre los elementos de control de video*/
    QGraphicsTextItem *toolTipItem; /*!< Muestra el tiempo actual desde donde se obtuvo el thumbnail (imagen pequeña).*/


    QPushButton *editButton; /*!< Botón para editar item */
    QPushButton *shareButton; /*!< Botón para compartir item*/
    QPushButton *mediaButton; /*!< Botón de media */
    QLabel *infoBoxLabel; /*! Usado para mostrar información al usuario en forma de label de texto */

    qreal prevVolumenPos; /*!< Posición previa de la barra deslizante de video. */

    QtAV::VideoPreviewWidget *m_preview; /*!< Widget que representa una vista anticipada de un frame del video.*/
    QTimer previewTimer; /*!< Timer para esconder preview luego de un tiempo definido*/


    //Adjustable parameters
    bool m_previewActivated; /*!< Flag que indica si esta activada la visualización del frame de muestra anticidada. */
    QSize m_previewSize; /*!< Almacena el tamaño de la vista anticipada de un frame. */
    int m_previewShowTime; /*!< Tiempo durante el cual la imagen previa se puede ver. (Mientras el mouse esta estático o fuera del slider de tiempo) */
    int m_initalVolume;/*!< Volumen inicial*/
    bool m_currPosViewActivated; /*!< Flag que indica si esta activada la visualización de la posición actual del reproductor.*/
    qreal m_tickWidthRatio;/*!< Razón entre el tamaño del tick de highlight y el ancho del slider*/
    qreal m_tickHeightRatio;/*!< Razón entre el tamaño del tick de highlight y el alto del slider*/
    timeSliderPos m_timeSliderPos;/*!< Posición del slider de tiempo con respecto a los demás elementos del control de video*/
    qreal m_frameHeightRatio;/*!< Razón entre el alto del frame de control de video y el player*/
    bool m_defaultPlayBackRate;/*!< Uso de vector de velocidades por defecto o nuevo*/
    bool m_enableHideControlFrame;/*!< Habilitar esconder el frame de control de video*/
    QVector<qreal> playBackRatesVector;/*!< Vector de velocidades usado*/

    bool repeat; /*!< Flag para indicar si al final de un video se debe reiniciar el video o avanzar al siguiente*/


    bool closeFlag; /*!< Flag para indicar que se va a cerrar el player*/

    bool useOpenGL; /*!< Flag para indicar si se usa OpenGl para el player */

    /*!
     \brief Inicializa la UI del reproductor.

    */
    void setupDefaultUI();
    /*!
     \brief Inicializa la configuración inicial del reproductor.
            Conecta las señales con los slots e incializa los parámetros necesarios.
    */
    void initDefaultPlayer();
    /*!
     \brief Inicializa la escena.
            Inserta el frame de control de video en el reproductor.
    */
    void initDefaultScene();
    /*!
     \brief Entrega la velocidad de reproducción actual para video.

     \return qreal es la velocidad de reproducción.
    */
    qreal playbackRate() const;

    /**
     * @brief deleteProxyWidgets Elimina los widgets insertados en la escena.
     */
    void deleteProxyWidgets();

protected slots:
    /*!
    \brief Slot que maneja cuando cambia el estado multimedia del reproductor.
     \param status Estado del reproducor.
    */
    virtual void statusChanged(QtAV::MediaStatus status);
    /*!
     \brief Cambio los límites actuales de la línea de tiempo.

     \param init Límite inicial en [mseg].
            end Límite final en [mseg]
    */
    virtual void sliderLimitsChangeSlot(qint64 init, qint64 end);
public slots:
    void changeCloseButtonPos();

    /*simpleplayer example qtav*/
    /*!
    \brief Carga y reproduce el archivo multimedia indicado en el parámetro "path".
     \param path Ruta del archivo multimedia.
     \return bool retorna si el archivo multimedia se cargo corrrectmanete.
    */
    bool openMedia(QString path);
    /*!
    \brief Mover la posición del reproductor a una posición específica.
     \param value Posición que se definirá de acuerdo.
    */
    void seekBySlider(int value);
    /*!
    \brief Mover la posición del reproductor de acuerdo a la posición actual del slider de tiempo.
    */
    void seekBySlider();
    /*!
    \brief Si hay un archivo multimedia cargado, este se reproduce (si esta pausado o detendio) o se pausa (si esta reproduciendo).
    */
    void playPause();
    /*simpleplayer example qtav*/
    /*!
     \brief Maneja el modo de reproducción en pantalla completa.

     \param st es un booleano que indica si el modo de reproducción en pantalla completa esta activado o no.
    */
    void setFullScreen(bool st = true);

    /*!
     \brief Esconde los elementos de control de video.

    */
    void hideControls();
    /*!
     \brief Muestra los elementos de control de video.

    */
    void showControlFrame();
//    /*!
//     \brief Muestra en la vista del reproductor únicamente la barra de tiempo.

//    */
//    void showTimeSlider();


    /*!
     \brief Maneja la selección de un tick o highlight.

     \param idx corresponde al índice del highlight que se va a seleccionar.
    */
    void selectTick(int idx);

    /*!
     \brief Maneja el ingreso a un tick o highlight en forma externa.

     \param idx corresponde al índice del highlight que se va ha entrar.
    */
    void hoveredTick(int idx);

    /*!
     \brief Detiene la reproducción actual.

     \return bool retorna verdadero si el video pudo ser detenido.
    */
    void stopPlayer();
    /**
     * @brief managePlayPauseStatus Maneja el estado del player de acuerdo a su estado actual.
     * Estado:
     *  Stop o Pause -> emitir señal de play.
     *  Reproducción -> emitir señal de pause.
     */
    void managePlayPauseStatus();
    void playForcedSlot();

//    void changeCloseButtonPos();

signals:

    /*!
     \brief

    */
    void listKeyReleased();
    /*!
     \brief Señal enviada cuando el cursor del mouse ingresa sobre un tick.
     \param idx Indice del tick ingresado.
    */
    void tickEntered(int idx);
    void hideSearchBar();

    /*!
     \brief Señal de "play" forzada.

    */
    void playForced();

    /*!
     \brief Señal de "play".

    */
    void play();

    /*!
     \brief Señal de "pause".

    */
    void pause();

    /*!
     \brief Señal emitida cuando cambia la duración actual del video.

     \param qint64 es la nueva duración del video en [mseg].
    */
    void durationChanged(qint64 , qint64);

    /*!
     \brief Señal emitida cuando se hace click en un highlight (Tree Aproach).

     \param int es el indice del highlight en el que se hizo click.
    */
    void tickClickedTree(int);

    void tickClickedSignal();

    /*!
     \brief Señal enviada cuando el reproductor pasa a modo de pantalla completa.

    */
    void fullscreen();
    /*!
     \brief Selección del tick para el hltctlview (vista zoomeada).
     \param int idx Indice.
    */
    void tickSelected_hltView(int idx);

    /*!
     \brief Cambio de posición luego de una actualización del slider de tiempo, dirigida al  hltctlview (vista zoomeada).
     \param qint64 value nueva posición.
    */
    void positionChanged_hltView(qint64 value);

    /*!
     \brief Señal enviada cuando un video termina de cargarse en el reproductor.
    */
    void videoLoaded();
    /*!
     * \brief Señal emitida cuando el player comienza a reproducir un video por primera vez.
     */
    void initPlayerSignal(qint64);

    /*!
      * \brief Señal emitida cuando se hace click sobre una de las flechas para avanzar o retroceder un elemento del playlist.
      *
      * \param isFordward índica si fue presionada la flecha para avanzar, en caso contrario corresponde a la flecha para retroceder un item.
      *
    */
    void arrowClicked(bool isFordward,bool isAutoRepeat);

    /*!
      * \brief Señal emitida cuando se llega al final del item de video actual que se esta reproduciendo.
    */
    void endCurrentMediaItem(bool isFordward);
    /*!
      * \brief Señal emitida cuando se hace click sobre el botón de cerrar
    */
    void closeButtonClicked();

private slots:
    /*simpleplayer example qtav*/
    /*!
     \brief Slot que maneja la actualización de la posición del slider de tiempo

     \param value es la nueva posición del slider.
    */
    void updateSlider(qint64 value);

    /*!
     \brief Actualiza la unidad de hace la transformación entre posición en el slider de tiempo y posición en el reproductor, de acuerdo al intervalo de notificación.
    */
    void updateSliderUnit();
    /*simpleplayer example qtav*/
    /*!
     \brief Asigna el estado actual al reproductor.

     \param state Estado del reproductor.
    */
    void setState(QtAV::AVPlayer::State state);
    /*!
     \brief Actualiza la velocidad de reproducción de acuerdo al combobox de velocidad.

    */
    void updateRate();
    /*!
     \brief Cambia el volumen de acuerdo al slider de volumen.

     \param position posición del slider de volumen.
    */
    void volumeSliderMoved(int position);
    /*!
     \brief Maneja el comportamiento del frame de control de video cuando se hace click() en el boton de volumen.

     \param st estado actual del boton de volumen, si es verdadero entonces se asigna un valor de cero a la posición del slider de volumen.
    */
    void volumeButtonClicked(bool st);
    /*!
     \brief Maneja el comportamiento del widget cuando el mouse se encuentra sobre el slider de tiempo.
            Muestra un preview del frame actual y la posición actual del reproductor en formato "HH:mm:ss".

     \param pos Posición en el slider de tiempo.
     \param value Valor de tiempo.
    */
    void onTimeSliderHover(int pos, int value);
    /*!
     \brief Maneja el comportamiento del widget cuando el mouse sale del slider de tiempo.

    */
    void onTimeSliderLeave();
    /*!
     \brief Muestra el tiempo actual de reproducción desde una fuente externa.

     \param value valor de tiempo.
    */
    void showToolTipExt(int value);
    /*!
     \brief Oculta la ventana de vista previa de un frame.
    */
    void hidePreview();

    void initPlayerWidget(const QString &path);
    /*!
     \brief

     \param index
    */
    void selectSlot(int index);
    /*!
     \brief Maneja el ingreso a un tick por parte del mouse

     \param idx corresponde al índice del highlight al que se ingreso.
    */
    void enteredTickSlot(int idx);


    /*!
     \brief Maneja el click del botón "Cerrar"
    */
    void closeButtonClickedSlot();

    void videoRendererClickedSlot();
};

#endif // NICEPLAYERVIEW_H
