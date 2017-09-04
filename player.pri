INCLUDEPATH += $$PWD
CONFIG += avwidgets

SOURCES += \
    $$PWD/CustomPushButton.cpp \
    $$PWD/niceTimeSlider.cpp \
    $$PWD/UpWardComboBox.cpp \
    $$PWD/niceDragItemRenderer.cpp \
    $$PWD/searchframe.cpp \
    $$PWD/sliceframe.cpp \
    $$PWD/PlayerView.cpp \
    $$PWD/BorderFrame.cpp \
    $$PWD/highlightController/HltCtlView.cpp \
    $$PWD/highlightController/slideritem.cpp \
    $$PWD/highlightController/timeline.cpp \
    $$PWD/highlightController/trickitem.cpp


HEADERS  += \
    $$PWD/CustomPushButton.h \
    $$PWD/niceTimeSlider.h \
    $$PWD/UpWardComboBox.h \
    $$PWD/niceDragItemRenderer.h \
    $$PWD/searchframe.h \
    $$PWD/sliceframe.h \
    $$PWD/PlayerView.h \
    $$PWD/BorderFrame.h \
    $$PWD/highlightController/HltCtlView.h \
    $$PWD/highlightController/slideritem.h \
    $$PWD/highlightController/timeline.h \
    $$PWD/highlightController/trickitem.h

RESOURCES += $$PWD/qtavplayer.qrc
