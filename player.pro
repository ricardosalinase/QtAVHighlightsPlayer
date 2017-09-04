## Highlights Player By Ricardo Salinas and Cristobal Barrientos##

##Este modulo contiene una version simple del reproductor principal el cual contiene la subclase de este
##Aquí se puede probar la reproducción de videos y mejorar todo lo relacionado a ello (renderizacion, buffering, etc)
##El ejemplo carga una lista de highlights (struct highlightData) enviados como argumentos del programa y carga esa
##lista en el reproductor simple

##los argumentos se envian de la siguiente forma:
##"PATH VIDEO i" highlights i "PATH VIDEO J" highlights j
##donde highlights : startime-endtime
##Ejemplo:
##H:/GOPR0378ctm/GOPR0378.MP4  10000-20000 31324-39142 45413-54143 H:/Hero5Dirt/GOPR0106.MP4  12133-21423 33413-39413
##45123-51314

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hltplayer
TEMPLATE = app
CONFIG += c++11

include(player.pri)

SOURCES+= main.cpp \
    hltplayerctl.cpp

HEADERS += \
    hltplayerctl.h

# --------------------------------------------

# --------------- Common ---------------------

INCLUDEPATH += $$PWD/common

DEPENDPATH += $$PWD/common

SOURCES+= $$PWD/common/hltviewutils.cpp \
    $$PWD/common/nicexml.cpp \
    $$PWD/common/niceslider.cpp \
    $$PWD/common/roundedpolygon.cpp

HEADERS+= $$PWD/common/hltviewutils.h \
    $$PWD/common/hlttypes.h \
    $$PWD/common/nicexml.h \
    $$PWD/common/niceslider.h \
    $$PWD/common/roundedpolygon.h

FORMS += \
    highlightController/hltscontrollerwidget.ui

DISTFILES += \
    readme.md
