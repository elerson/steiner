#-------------------------------------------------
#
# Project created by QtCreator 2014-01-23T09:53:43
#
#-------------------------------------------------

QT       += core gui

TARGET = Visualizer
TEMPLATE = app


SOURCES += main.cpp\
        visualiserwindow.cpp \
        image.cpp \
        visibilitygraph.cpp \
        ../../VisibilityAlgorithm/visilibity.cpp \
        ../../GraphSteinerTree/SteinerGenetic.cpp \
        ../../GraphSteinerTree/SteinerApproximation.cpp \
        ../../GraphSteinerTree/Prim.cpp \
        ../../GraphSteinerTree/Heap.cpp \
        ../../GraphSteinerTree/Dijkstra.cpp \
    euclideansteiner.cpp

HEADERS  += visualiserwindow.h \
        image.h \
        visibilitygraph.h \
        ../../VisibilityAlgorithm/visilibity.hpp \
        ../../GraphSteinerTree/SteinerGenetic.h \
        ../../GraphSteinerTree/SteinerApproximation.h \
        ../../GraphSteinerTree/Prim.h \
        ../../GraphSteinerTree/Heap.h \
        ../../GraphSteinerTree/Graph.h \
        ../../GraphSteinerTree/Dijkstra.h \
        ../../EuclideanSteinerTree/efst.h \
        ../../EuclideanSteinerTree/bbobstacle.h \
    euclideansteiner.h

INCLUDEPATH += ../../VisibilityAlgorithm ../../GraphSteinerTree ../../EuclideanSteinerTree
INCLUDEPATH += $(shell pkg-config opencv4 --cflags )

FORMS    += visualiserwindow.ui
LIBS += -lCGAL -lgmp -frounding-math ../../EuclideanSteinerTree/libefst.a
CONFIG += link_pkgconfig
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

OTHER_FILES += \
    ../../GraphSteinerTree/Makefile

PKGCONFIG += opencv4



