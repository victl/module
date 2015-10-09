#-------------------------------------------------
#
# Project created by QtCreator 2015-10-02T09:24:08
#
#-------------------------------------------------

QT       -= core gui

TARGET = module
TEMPLATE = lib
VERSION = 1.0.1

CONFIG += debug

DEFINES += MODULE_LIBRARY

SOURCES += \
    charpoint.cpp \
    connection.cpp \
    lane.cpp \
    log.cpp \
    mem_map.cpp \
    module.cpp \
    perimeter.cpp \
    radar_map.cpp \
    rndf.cpp \
    rndfobj.cpp \
    segment.cpp \
    shm.cpp \
    spot.cpp \
    utils.cpp \
    waypoint.cpp \
    zone.cpp \
    impl/factory.cpp \
    impl/shm_.cpp

HEADERS += \
    charpoint.h \
    connection.h \
    lane.h \
    log.h \
    mem_map.h \
    module.h \
    perimeter.h \
    radar_map.h \
    rndf.h \
    rndfobj.h \
    segment.h \
    shm.h \
    spot.h \
    types.h \
    utils.h \
    version.h \
    waypoint.h \
    zone.h \
    impl/factory.h \
    impl/shm_.h

unix {
    CONFIG += staticlib
    target.path = /home/victor/workspace/car/UGV/libs/
    INSTALLS += target
}

DISTFILES += \
    module.txt
