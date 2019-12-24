QMAKE_CXXFLAGS += -std=c++11
CONFIG+=configA

configT{
TARGET = simulationt
SOURCES += \
    main_textmatlabf.cc
}

configA{
SOURCES += \
    Analysis.cc

HEADERS += \
    Analysisclass.h

TARGET = Analysis
}

LIBS += -L../general -lgeneral
win32:LIBS += -lopengl32
win64:LIBS += -lopengl32

PRE_TARGETDEPS += ../general/libgeneral.a

INCLUDEPATH += ../general
CONFIG += qt debug
SOURCES += \
    ../general/SupportADessinTexte.cc \
    ../general/Systeme.cc \
    ../general/Aleatoire.cc \
    ../general/Magnet.cc \
    ../general/Plan.cc \
    ../general/Dalle.cc \
    ../general/Brique.cc \
    ../general/Dodec.cc \
    ../general/Sphere.cc \
    ../general/Vecteur3D.cc


HEADERS += \
    ConfigFile.tpp \
    ../general/Dessinable.h \
    ../general/SupportADessin.h \
    ../general/SupportADessinTexte.h \
    ../general/Systeme.h \
    ../general/Aleatoire.h \
    ../general/Magnet.h \
    ../general/Obstacle.h \
    ../general/Plan.h \
    ../general/Dalle.h \
    ../general/Brique.h \
    ../general/Dodec.h \
    ../general/Sphere.h \
    ../general/Vecteur3D.h
