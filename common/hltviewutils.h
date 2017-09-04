#ifndef HLTVIEWUTILS_H
#define HLTVIEWUTILS_H

#include <QPixmap>
class QStringList;

/*!
 \brief

 \param trick
 \return QString
*/
QString trickToPixmapPath(QString trick = "highlight");
/*!
 \brief

 \param pxList
 \return QPixmap
*/
QPixmap getMultiItemDragPixmap(const QList<QPixmap> &pxList);
/*!
 \brief

 \param strList
 \return QString
*/
QString generateMimeDataString(const QList< QStringList > &strList);
/*!
 \brief

 \param duration
 \return QString
*/
QString milliseconds_to_MS(quint32 duration);
/*!
 \brief

 \param duration
 \return QString
*/
QString milliseconds_to_HMSms(quint32 duration);
/*!
 \brief convierte milisegundos a qstring formato HH:MM:SS

 \param duration tiempo en milisegundos
 \return QString tiempo formateado
*/
QString milliseconds_to_HMS(quint32 duration);
/*!
 \brief convierte tiempo formateado en HH:MM:SS a milisegundos

 \param duration String tiempo formateado
 \return quint32 duration tiempo en milisegundos
*/
quint32 HMS_to_milliseconds(QString duration);


#endif // HLTVIEWUTILS_H
