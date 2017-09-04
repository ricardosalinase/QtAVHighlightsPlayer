#ifndef PIKDENUM_H
#define PIKDENUM_H
#include <qnamespace.h>
#include <QString>

enum sportType{
    DownHill = 0,
    Surf = 1,
    Ski = 2,
    Snowboard = 3,
    MotoCross,
    Other,
    UnknownSport,
    None,
    BMX ,
    Bike //General Sport

};
enum pikdRoles{
    tagIDRole = Qt::UserRole,
    favoriteRole = Qt::UserRole+1,
    dataGIndexRole = Qt::UserRole+2,
    flagRole = Qt::UserRole + 3,
    iconReadyRole = Qt::UserRole + 4,
    clipIDRole = Qt::UserRole + 5
};
enum sortType{
    dates,
    places,
    tricks,
    favorites,
};

enum pikdContext{
    device,
    media,
    unknown
};

enum dbSearchtype{
    highlights,
    fullclip
};

QString sportTypeToString(sportType type);

sportType stringToSportType(const QString typestr);

#endif // PIKDENUM_H
