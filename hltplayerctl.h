#ifndef HLTPLAYERCTL_H
#define HLTPLAYERCTL_H

#include <QColor>
#include <QObject>
#include <QRect>
#include "hlttype.h"

class QListWidget;
class PlayerView;
/*!
 \brief clase controlador del ejemplo de modulo de player que carga una vista y un playlist para su posterior manejo de ambos

*/
class HltPlayerCtl : public QObject
{
    Q_OBJECT
public:
    /*!
     \brief

    */
    explicit HltPlayerCtl();
    /*!
     \brief

    */
    ~HltPlayerCtl();

    /*!
     \brief carga una lista de highlights a la playlist del reproductor

     \param clips
    */
    void setVideoList2(const QList<highlightData> &clips);
private slots:
    /*!
     \brief cierra la aplicacion

    */
    void closeLibraryWindow();
    /*!
     \brief carga la informacion del item seleccionado al reproductor y lo reproduce

     \param index indice del item seleccionado actual ya sea por lista o por que termino el anterior
    */
    void handleSelectionChanged(int index);
    /*!
     \brief maneja que highlight se va a reproducir cuando se cambia mediante una accion realizada por el reproductor ( termino el track o si se presiona flechas)

     \param isFordward indica si se tiene que avanzar o retroceder en la lista
     \param isAutorepeat indica si el usuario mantiene apretada la flecha para avanzar evitando que se haga una carga continua de highlights
    */
    void changeItemByPlayer(bool isFordward, bool isAutorepeat = false);
private:
    QListWidget *playlist; /*!< lista de clips a reproducir */
    PlayerView *pv; /*!< TODO: vista en pantalla completa donde se realiza la reproduccion de los clips */

    QList<highlightData> cd; /*!< lista de la informacion de los highlights que maneja el controlador */
    int thumbHeight, thumbWidth; /*!< thumbnail size */
    QColor hoverColor;
    QColor textColor;
    QString currentPath; /*!< ultimo video cargado. Se utiliza para chequear si es necesario cargar otro video o solo hacer seek a nuevos tiempos del currentpath!*>

    /*!
     \brief inicializa el reproductor

    */
    void setupPlayer();
    /*!
     \brief inicializa la vista playlist

    */
    void setupListView();
    /*!
     \brief agrega la informacion basica del highlight como item a la lista

     \param fData el highlight a agregar
    */
    void addFilteredChild(highlightData &fData);
    /*!
     \brief agrega la imagen generada en addfilteredchild al item que se agregara a la lista (en el modulo se genera solo una imagen negra ya que no carga el modulo videomanager)

     \param frame  imagen generada
     \param itemData el highlight a agregar
    */
    void addToPlaylist(QImage frame, const highlightData &itemData);
    /*!
     \brief genera el icono con la imagen y informacion adicional

     \param image imagen del icono
     \param textAtt tiempos del video
     \param pathAtt localizacion del video
     \return QIcon el icono resultado
    */
    QIcon generateHighlightIcon(QPixmap image, const QString &textAtt, const QString &pathAtt);
};

#endif // HLTPLAYERCTL_H
