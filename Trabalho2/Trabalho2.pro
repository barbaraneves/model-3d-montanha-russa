TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += bib
INCLUDEPATH += gui_glut
INCLUDEPATH += pessoal

LIBS += -lGL -lGLU -lglut -l3ds -lSDL_image

SOURCES += main.cpp \
    bib/Camera.cpp \
    bib/CameraDistante.cpp \
    bib/CameraJogo.cpp \
    bib/Desenha.cpp \
    bib/model3ds.cpp \
    bib/Vetor3D.cpp \
    bib/objeto.cpp\
    curvas/carrinho.cpp \
    curvas/curva.cpp \
    curvas/ponto.cpp \
    gui_glut/extra.cpp \
    gui_glut/gui.cpp

HEADERS += \
    bib/Camera.h \
    bib/CameraDistante.h \
    bib/CameraJogo.h \
    bib/Desenha.h \
    bib/model3ds.h \
    bib/Vetor3D.h \
    bib/objeto.h\
    curvas/carrinho.h \
    curvas/curva.h \
    curvas/ponto.h \
    gui_glut/extra.h \
    gui_glut/gui.h
