QT       += core gui
QMAKE_CXXFLAGS += -std=c++11 -fPIC

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -llapack -lblas -larmadillo
TEMPLATE = lib

CONFIG = staticlib
CONFIG += qt


SOURCES = Systeme.cc\
Aleatoire.cc \
Plan.cc \
Dalle.cc \
Brique.cc \
MagnetA.cc \
MagnetH.cc \
Vecteur3D.cc \
Cylinder.cc \
Sphere.cc \
SupportADessinTexte.cc \

HEADERS += \
    Systeme.h \
    Aleatoire.h \
    Obstacle.h \
    Plan.h \
    Dalle.h \
	Brique.h \
    MagnetA.h \
    MagnetH.h \
    Magnet.h\
    Vecteur3D.h \
    Dessinable.h \
    SupportADessin.h \
	Cylinder.h \
    Sphere.h \
    SupportADessinTexte.h \
    Magnet.h
