#include "hltviewutils.h"
#include <QPainter>
#include <QTime>

QString trickToPixmapPath(QString trick )
{
    trick = trick.toUpper();
    if(trick == "HIGHLIGHT")
        return QString(":/resource/Icons/pin");

    if(trick == "WAVE")
        return QString(":/resource/Icons/wave");

    if(trick == "WIPEOUT")
        return QString(":/resource/Icons/wave");

    if(trick == "RUN")
        return QString(":/resource/Icons/run");

    if(trick == "HIGH")
        return QString(":/resource/Icons/pin");

    if(trick == "CRASH")
        return QString(":/resource/Icons/crash");

    if(trick == "RAIL")
        return QString(":/resource/Icons/rail");

    if(trick.contains("JUMP"))
        return QString(":/resource/Icons/rocket");

    if(trick.contains("FRONT") || trick.contains("BACK") || trick.contains("HORIZONTAL") || trick.contains(QRegExp("^[0-9]*$"))){
        return QString(":/resource/Icons/rotation 2");
    }

    if(trick == "TRICKS")
        return QString(":/resource/Icons/tricks");

    return QString(":/resource/Icons/pin");
}

//Convert variable uint32 in milliseconds to HH:MM:SS
QString milliseconds_to_HMS(quint32 duration)
{
    QString res;

    duration /= 1000; //To milisecons
    int seconds = (int) (duration % 60);
    duration /= 60;
    int minutes = (int) (duration % 60);
    duration /= 60;
    int hours = duration;

    return res.sprintf("%02d:%02d:%02d", hours, minutes, seconds);
}
QString milliseconds_to_HMSms(quint32 duration)
{
    QString res;
    int miliseconds = (int) duration % 1000;
    duration /= 1000; //To milisecons
    int seconds = (int) (duration % 60);
    duration /= 60;
    int minutes = (int) (duration % 60);
    duration /= 60;
    int hours = duration;

    return res.sprintf("%02d:%02d:%02d,%03d", hours, minutes, seconds,miliseconds);
}
QString milliseconds_to_MS(quint32 duration)
{
    QString res;

    duration /= 1000; //To seconds
    int seconds = (int) (duration % 60);
    duration /= 60;
    int minutes = (int) (duration % 60);
    duration /= 60;
    minutes += (int)(duration %60 );

    return res.sprintf("%02d:%02d", minutes, seconds);
}

//Convert variable QString HH:MM:SS to uint32 in milliseconds
quint32 HMS_to_milliseconds(QString duration)
{
    //Modificar en base a QTime
    QTime aux(0,0,0,0);

    aux = QTime::fromString(duration,"hh:mm:ss");

    //confugure the time for the player
    qint32 ret = aux.hour()*3600000+aux.minute()*60000+aux.second()*1000;

    return ret;
}
//Se asume que todos los pixmap tiene el mismo tamaño
QPixmap getMultiItemDragPixmap(const QList<QPixmap> &pxList)
{
    if(pxList.isEmpty())
        return QPixmap();

    int px_W = pxList.at(0).width();
    int px_H = pxList.at(0).height();

    px_W += pxList.size() > 1 ? (pxList.size() - 1)*(px_W/2) : 0;
    QPixmap px(px_W,px_H);
    QPainter *painter = new QPainter(&px);
    //painter->fillRect(px.rect(),Qt::transparent);
    painter->setBackground(Qt::transparent);
    QList< QPixmap >::const_iterator it = pxList.begin();
    int x = px_W;
    while(it != pxList.end())
    {
        x -= (*it).width();
        painter->drawPixmap(x,0,(*it).width(), (*it).height(),(*it));
        x += ((*it).width()/2);
        ++it;
    }
    delete painter;

    return px;
}

QString generateMimeDataString(const QList<QStringList> &strList)
{
    QList< QStringList >::const_iterator it = strList.begin();
    QString outputStr;

    while( it != strList.end())
    {
        QStringList::const_iterator tit = (*it).begin();
        while(tit != (*it).end())
        {

            outputStr += (*tit);
            ++tit;
            if( tit != (*it).end() )
                outputStr += ",";
        }

        ++it;

        if( it != strList.end() )
            outputStr += ";";
    }

    return outputStr;
}
