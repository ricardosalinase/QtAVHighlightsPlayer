#include "pikdenum.h"

QString sportTypeToString(sportType type)
{
    switch (type)
    {
    case Surf:
        return "surf";
    case DownHill:
        return "downhill";
    case BMX:
        return "bmx";
    case MotoCross:
        return "motocross";
    case Snowboard:
        return "snowboard";
    case Ski:
        return "ski";
    case UnknownSport:
        return "unknown";
    case Other:
        return "other";
    default:
        return "none";
    }
}

sportType stringToSportType(const QString typestr)
{
    if(typestr.contains("surf",Qt::CaseInsensitive))
        return Surf;
    if(typestr.contains("downhill",Qt::CaseInsensitive))
        return DownHill;
    if(typestr.contains("bmx",Qt::CaseInsensitive))
        return BMX;
    if(typestr.contains("motocross",Qt::CaseInsensitive))
        return MotoCross;
    if(typestr.contains("snowboard",Qt::CaseInsensitive))
        return Snowboard;
    if(typestr.contains("ski",Qt::CaseInsensitive))
        return Ski;
    if(typestr.contains("other",Qt::CaseInsensitive))
        return Other;
    return UnknownSport;
}
