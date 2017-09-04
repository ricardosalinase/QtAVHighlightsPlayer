##Highlights Player QtAV By Ricardo Salinas and Cristobal Barrientos

# Introducción
Este modulo contiene una version simple del reproductor principal el cual contiene la subclase de este
Aquí se puede probar la reproducción de videos y mejorar todo lo relacionado a ello (renderizacion, buffering, etc)
El ejemplo carga una lista de highlights (struct highlightData) enviados como argumentos del programa y carga esa
lista en el reproductor simple


# Prerequisites
* Qt SDK 5.4 or higher (Open source or commercial)
* Windows
* Build: MinGW 4.9 or higher version
* QtAV: (QtAV-Qt5.4-MinGW32-release-a30ffaf used)

# Qt Creator args:
los argumentos se envian de la siguiente forma:
"PATH VIDEO i" highlights i "PATH VIDEO J" highlights j
donde highlights : startime-endtime
Ejemplo:
H:/GOPR0378ctm/GOPR0378.MP4  10000-20000 31324-39142 45413-54143 H:/Hero5Dirt/GOPR0106.MP4  12133-21423 33413-39413
45123-51314

## Contact:

rsalinase@outlook.cl
