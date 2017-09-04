#ifndef SWTYPES_H
#define SWTYPES_H

#include <QDir>
#include "niceenum.h"
#include "tree.hh"
#include "hlttype.h"

typedef struct videoData_s{
    QString file;  /*!< nombre del archivo de video */
    QDir path;//Video Path /*!< ruta donde se encunentra el video */
    qint64 duration; /*!<  duracion total del video en [ms] */
    QDateTime date;
    sportType sport;
    QMultiMap<QString,hltPar> trickPool; /*!<  Pool de todos los tricks dentro del video */
    videoData_s( const videoData_s &vd):file(vd.file),path(vd.path),duration(vd.duration),date(vd.date),sport(vd.sport),trickPool(vd.trickPool){}
    videoData_s():file("0"),duration(-1){}

}videoData;
Q_DECLARE_METATYPE(videoData)

/*!
 \brief estructura principal de un video que fue procesado por el programa el cual

*/
typedef struct highlight_s{
    /*!
     \brief Tipo de dato que se define para la estructura highlight_s
     */
    videoData videoFile; /*!< Datos del video utilizado*/
//    QList<hltPar> times;  /*!< marcas de tiempo de los highlights encontrados */
    tree<highlightData> hf;
    bool isProcessed; // /*!< señala si el video ya fue procesado */
    sportType sport; /*!< tipo de deporte que pertenece el video */
    bool hasInternalData; /*!< Indica que tiene los datos dentro del video */
    highlight_s(){}
    highlight_s( videoData vd, tree<highlightData> tree):videoFile(vd),hf(tree),sport(vd.sport){}
}videoSportData;

typedef struct niceModelItem_s{
    highlightData hltData;
    Qt::CheckState state;
    bool isSelected;
    niceModelItem_s():state(Qt::Unchecked),isSelected(false){}
    niceModelItem_s(highlightData data):hltData(data),state(Qt::Unchecked),isSelected(false){}
    niceModelItem_s(int init, int end):hltData(init,end),state(Qt::Unchecked),isSelected(false){}
    bool operator()(niceModelItem_s i,niceModelItem_s j){return (i.hltData.realTime.first <j.hltData.realTime.first);}
}niceModelItem;

/*!
 \brief identifica los tipos de deporte disponibles para el sw, esto se usa para activar el clasificador determinado para cada deporte

*/


Q_DECLARE_METATYPE(sportType)

#endif // SWTYPES_H
