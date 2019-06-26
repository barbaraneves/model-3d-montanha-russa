#include "curva.h"
#include "gui.h"

Curva::Curva()
{
    P = new Vetor3D[4];
    this->setTipo(1);

}

void Curva::atualizarPontosDeControle(vector<Vetor3D> pontosDeControle){
    Pn.clear();
    for(Vetor3D v : pontosDeControle){
        Pn.push_back(v);
    }
}

Vetor3D Curva::PTu(float u){

    vector<float> us;
    us.resize(4);
    us[0] = pow(u, 3);
    us[1] = pow(u, 2);
    us[2] = u;
    us[3] = 1;

    float matriz[4];

    for(int i = 0; i<4; i++){
        matriz[i] = 0;
        for(int j = 0; j<4; j++){
            matriz[i] += us[j] * Modo[j][i];
        }
    }

    Vetor3D resultado;
    for(int i = 0; i<4; i++){
        resultado.x += matriz[i] * P[i].x;
        resultado.y += matriz[i] * P[i].y;
        resultado.z += matriz[i] * P[i].z;
    }

    return resultado;
}


void desenhaTrechoTrilho(){
    glPushMatrix();
       glTranslated(0.25, 0, 0);
       glRotated(90, 0, 1, 0);
       glRotated(180, 1, 0, 0);
       glPushMatrix();
         glColor3f(0.1f, 0.0f, 0.0f);
         glBegin(GL_QUADS);
           glVertex3f(  0, 0, -0.5);
           glVertex3f(-.3, 0,  -.5);
           glVertex3f(-.3, 0,   .5);
           glVertex3f(  0, 0,   .5);
           glVertex3f(  0, 0,   .5);
           glVertex3f(-.3, 0,   .5);
           glVertex3f(-.3, 0,  -.5);
           glVertex3f(  0, 0, -0.5);
         glEnd();
         glColor3f(0.1f, 0.1f, 0.1f);
       glPopMatrix();
    glPopMatrix();
}

Vetor3D Curva::d1(float u){

    vector<float> us;
    us.resize(4);
    us[0] = 3 * (pow(u, 2));
    us[1] = 2 * u;
    us[2] = 1;
    us[3] = 0;

    float matriz[4];

    for(int i = 0; i<4; i++){
        matriz[i] = 0;
        for(int j = 0; j<4; j++){
            matriz[i] += us[j] * Modo[j][i];
        }
    }
    Vetor3D resultado;
    for(int i = 0; i<4; i++){
        resultado.x += matriz[i] * P[i].x;
        resultado.y += matriz[i] * P[i].y;
        resultado.z += matriz[i] * P[i].z;
    }

    return resultado * -1;

}

Vetor3D Curva::d2(float u){

    vector<float> us;
    us.resize(4);
    us[0] = 6*u;
    us[1] = 2;
    us[2] = 0;
    us[3] = 0;

    float matriz[4];

    for(int i = 0; i<4; i++){
        matriz[i] = 0;
        for(int j = 0; j<4; j++){
            matriz[i] += us[j] * Modo[j][i];
        }
    }
    Vetor3D resultado;
    for(int i = 0; i<4; i++){
        resultado.x += matriz[i] * P[i].x;
        resultado.y += matriz[i] * P[i].y;
        resultado.z += matriz[i] * P[i].z;
    }

    return resultado;
}



void Curva::desenhaTrilho(){

    for(int i = 0; i < ((int) Pn.size()) - 3 ; i++){

        P[0] = Pn[i];
        P[1] = Pn[i + 1];
        P[2] = Pn[i + 2];
        P[3] = Pn[i + 3];


        for(float u = 0; u <= 1; u += delta){
            Vetor3D z_ = d1(u); // k'
            Vetor3D y_ = d2(u); // UP
            Vetor3D x_ = y_ ^ z_; // UP X k'  -- i'
                    y_ = z_ ^ x_; // k' X i' -- j'
            Vetor3D t_ = PTu(u); // O
            z_.normaliza();
            y_.normaliza();
            x_.normaliza();

            double Trans[16] = {
                x_.x, y_.x, z_.x, t_.x,
                x_.y, y_.y, z_.y, t_.y,
                x_.z, y_.z, z_.z, t_.z,
                0   , 0   , 0   , 1
            };

            glPushMatrix();
                glColor3f(0, 0, 0);
                glMultTransposeMatrixd(Trans);
                desenhaTrechoTrilho();
            glPopMatrix();


        }
    }
}

int Curva::getTipo(){
    return this->tipo;
}


void Curva::setTipo(int tipo){

    switch (tipo) {

        case 1:{ //Interpoladora
            this->tipo = tipo;
            float Mi[4][4] = {
                {-4.5,  13.5, -13.5,  4.5},
                {   9, -22.5,    18, -4.5},
                {-5.5,     9,  -4.5,    1},
                {   1,     0,     0,    0}
            };
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    Modo[i][j] = Mi[i][j];
                }
            }

        break;
        }

        case 2:{ //Bezier
            this->tipo = tipo;
            float Mb[4][4] = {
                {-1,  3, -3,  1},
                { 3, -6,  3,  0},
                {-3,  3,  0,  0},
                { 1,  0,  0,  0}
            };

            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    Modo[i][j] = Mb[i][j];
                }
            }

        break;
        }

        case 3:{ //Hermite
            this->tipo = tipo;
            float Mh[4][4] = {
                { 2, -2,  1,  1},
                {-3,  3, -2, -1},
                { 0,  0,  1,  0},
                { 1,  0,  0,  0}
            };
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    Modo[i][j] = Mh[i][j];
                }
            }
        break;
        }

        case 4: { //Catmull-Rom
            this->tipo = tipo;
            float Mcm[4][4] = {
                {-0.5,  1.5, -1.5,  0.5},
                {   1, -2.5,    2, -0.5},
                {-0.5,    0,  0.5,    0},
                {   0,    1,    0,    0}
            };

            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    Modo[i][j] = Mcm[i][j];
                }
            }

        break;
        }

        case 5:{ //B-Spline
            this->tipo = tipo;
            float Mbs[4][4] = {
                {-1/6.0,  3/6.0, -3/6.0, 1/6.0},
                { 3/6.0, -6/6.0,  3/6.0, 0/6.0},
                {-3/6.0,  0/6.0,  3/6.0, 0/6.0},
                { 1/6.0,  4/6.0,  1/6.0, 0/6.0}
            };
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    Modo[i][j] = Mbs[i][j];
                }
            }
        break;
        }
    }

}
