#ifndef TREM_H
#define TREM_H

#include "bib/model3ds.h"
//#include "curvas/spline.h"
#include "bib/CameraDistante.h"
#include "bib/objeto.h"



class Trem : public Objeto
{
public:

    Trem();
    Model3DS* malha;
    float u = 0.0;
    float delta = 0.01;

    void desenha();
};





#endif // TREM_H
