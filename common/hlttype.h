#ifndef HLTTYPE_H
#define HLTTYPE_H
#include <QPair>
#include <QVariantHash>
#include <QDateTime>

typedef QPair<qint32,qint32> hltPar;

typedef struct highlightData_s{
    unsigned int tagID = 0; /*!< tagID = 0 -> fullvideo */
    unsigned int clipID;
    qreal speed;  /*!< Speed of highlight */
    QString path; /*!< ruta del video asociado a highlightData (solo libreria)*/
    QString trick; /*!< Nombre de la maniobra*/
    hltPar realTime;/*!< Tiempos del highlight arrojados por classifier*/
    QVariantHash properties;
    QMultiMap<QString,hltPar> trickMap; /*!< Tiempos del highlight hijos arrojados por classifier*/
    highlightData_s():speed(1.0),trick("highlight") {}
    highlightData_s(int st,int et) : speed(1.0),trick("highlight"),realTime(st,et){}
    highlightData_s(QString p, QString t, hltPar hp) : path(p),trick(t),realTime(hp){
    }
    bool operator()(highlightData_s i,highlightData_s j){return (i.realTime.first<j.realTime.first);}
    QString getValueWithFormat(QString value){
        return value == "date" ? properties.value(value).toDateTime().toString("yyyy MM dd"):properties.value(value).toString();
    }
}highlightData;

#endif // HLTTYPE_H
