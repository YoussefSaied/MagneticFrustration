QT += opengl
QT += charts
QT += gui
QT       += core gui

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simulation1
LIBS += -L../general -lgeneral
LIBS += -llapack -lblas -larmadillo
win32:LIBS += -lopengl32
win64:LIBS += -lopengl32

PRE_TARGETDEPS += ../general/libgeneral.a

INCLUDEPATH += ../general
CONFIG += qt debug
SOURCES += \
    main_qt_gl.cc \
    glwidget.cc \
    Window.cc \
    glsphere.cc \
    glcylinder.cc \
    vue_opengl.cc \
    ../general/SupportADessinTexte.cc \
    ../general/Systeme.cc \
    ../general/Aleatoire.cc \
    ../general/Plan.cc \
    ../general/Dalle.cc \
    ../general/Brique.cc \
    ../general/Dodec.cc \
    ../general/Cube.cc \
    ../general/Sphere.cc \
    ../general/Vecteur3D.cc


HEADERS += \
    glwidget.h \
    Window.h \
    vertex_shader.h \
    vue_opengl.h \
    glsphere.h \
    glcylinder.h \
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
    ../general/Cube.h \
    ../general/Sphere.h \
    ../general/Vecteur3D.h

RESOURCES += \
    resource.qrc
