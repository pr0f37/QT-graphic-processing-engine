#-------------------------------------------------
#
# Project created by QtCreator 2010-08-02T12:52:16
#
#-------------------------------------------------

QT       += core gui sql xml widgets

TARGET = SQL
TEMPLATE = app


SOURCES += main.cpp\
       mainwindow.cpp \
    infowindow.cpp \
    view.cpp \
    imageitem.cpp \
    analyser.cpp \
    logger.cpp \
    siftanalyser.cpp \
    SIFT/xform.c \
    SIFT/utils.c \
    SIFT/sift.c \
    SIFT/minpq.c \
    SIFT/kdtree.c \
    SIFT/imgfeatures.c \
    siftdescriptor.cpp \
    siftimage.cpp \
    siftdictionarysections.cpp \
    siftclasscontainer.cpp \
    lssanalyser.cpp \
    lssimage.cpp \
    lssdescriptor.cpp \
    lssclasscontainer.cpp

HEADERS  += mainwindow.h \
    connection.h \
    infowindow.h \
    view.h \
    imageitem.h \
    analyser.h \
    logger.h \
    siftanalyser.h \
    SIFT/xform.h \
    SIFT/utils.h \
    SIFT/sift.h \
    SIFT/minpq.h \
    SIFT/kdtree.h \
    SIFT/imgfeatures.h \
    siftdescriptor.h \
    siftimage.h \
    siftdictionarysections.h \
    siftclasscontainer.h \
    lssanalyser.h \
    lssimage.h \
    lssdescriptor.h \
    lssclasscontainer.h

FORMS    += mainwindow.ui

RESOURCES += \
    SQL.qrc

INCLUDEPATH += SIFT/ \
    /usr/include/opencv \
    /home/prorok/OpenCV-2.4.3/include/opencv


LIBS += `pkg-config opencv --cflags --libs`
