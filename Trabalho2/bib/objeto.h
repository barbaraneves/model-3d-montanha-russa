#ifndef OBJETO_H
#define OBJETO_H

#include <bib/model3ds.h>
#include <bib/Desenha.h>
#include <fstream>

using namespace std;

class Objeto
{
public:

//locais
    float tx = 0.0;
    float ty = 0.0; // coordenadas Translação
    float tz = 0.0;

    float ax = 0.0;
    float ay = 0.0;//rotation eu acho
    float az = 0.0;

    float sx = 1.0;
    float sy = 1.0;//scala
    float sz = 1.0;

    bool selecionado = false;
    bool DrEx = true;

    Objeto(){};
    virtual ~Objeto() = 0;
    virtual void desenha() = 0;
};

#endif // OBJETO_H
