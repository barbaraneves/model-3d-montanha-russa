#ifndef CURVA_H
#define CURVA_H

#include "bib/Vetor3D.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <GL/gl.h>
#include <GL/glut.h>
#include <Desenha.h>

using namespace std;

class Curva
{
public:
public:

    float Modo[4][4];
    vector<Vetor3D> Pn;
    Vetor3D * P;
    float delta = .01;
    int tipo;

    Curva();

    void atualizarPontosDeControle(vector<Vetor3D> pontosDeControle);
    Vetor3D PTu(float u);
    Vetor3D d1(float u);
    Vetor3D d2(float u);
    void desenhaTrilho();
    void setTipo(int tipo);
    int getTipo();
};

#endif // CURVA_H
