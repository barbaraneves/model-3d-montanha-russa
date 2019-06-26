#ifndef PONTO_H
#define PONTO_H
#include <bib/Vetor3D.h>
#include <GL/glut.h>
#include <bib/objeto.h>

class ponto: public Objeto
{
public:
    float tx = 0.0;
    float ty = 0.0; // coordenadas Translação
    float tz = 0.0;

    ponto();

    void desenha();
    Vetor3D getCoordenadas();
};

#endif // PONTO_H
