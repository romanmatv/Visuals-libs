#-------------------------------------------------
#
# Project created by QtCreator 2018-10-15T12:32:16
#
#-------------------------------------------------

QT       += gui widgets

TARGET = Nanko
TEMPLATE = lib

DEFINES += NANKO_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG(debug, debug|release) {
    # Подключаем debug-версии библиотек для разных платформ
    win32: LIBS += -Llib -lAudioThreadd1
    unix: LIBS += -Llib -L. -lAudioThreadd -Wl,-rpath,lib -Wl,-rpath,.
} else {
    # Подключаем release-версии библиотек для разных платформ
    win32: LIBS += -Llib -lAudioThread1
    unix: LIBS += -Llib -L. -lAudioThread -Wl,-rpath,lib -Wl,-rpath,.
}

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        nanko.cpp

HEADERS += \
        nanko.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
