#include <iostream>

using namespace std;

#include <gui.h>
#include <vector>

#include "CameraDistante.h"
#include "Desenha.h"

#include <curvas/curva.h>
#include <curvas/ponto.h>
#include <curvas/trem.h>

vector<ponto *> pontos;
vector<ponto *> pronto;

Trem * trem = new Trem;

float delta = 0.01;
Curva *  spline = new Curva;

Camera * cam = new CameraDistante();
Camera * goPro = new CameraDistante();
Camera * terceiraPessoa = new CameraDistante();

static bool multiple_viewports = false;
int width = 800;
int height = 600;


//-------------------picking------------------

int pontoSelecionado = 0;

void desenhaPontosDeControle()
{
    //desenhando pontos de controle
    for (int i=0; i<pontos.size(); i++) {
        //definindo cor da selecao
        if (i == pontoSelecionado-1) {
            GUI::setColor(1,1,1,1,true);
        } else {
            GUI::setColor(0,0,1,1,true);
        }
        //desenhando (definindo nomes para o picking)
        Vetor3D p = pontos[i]->getCoordenadas();
        glPushName(i+1); //não se deve definir name = 0!
            GUI::drawSphere(p.x,p.y,p.z,0.2);
        glPopName();
    }

}

//picking
int picking( GLint cursorX, GLint cursorY, int w, int h ) {
    int BUFSIZE = 512;
    GLuint selectBuf[512];

    GUI::pickingInit(cursorX,cursorY,w,h,selectBuf,BUFSIZE);

//de acordo com a implementacao original da funcao display
    //lembrar de nao inicializar a matriz de projecao, para nao ignorar a gluPickMatrix
    GUI::displayInit();
    //só precisa desenhar o que for selecionavel
    desenhaPontosDeControle();
//fim-de acordo com a implementacao original da funcao display

    //retornando o name do objeto (ponto de controle) mais proximo da camera (z minimo! *[matrizes de normalizacao da projecao])
    return GUI::pickingClosestName(selectBuf,BUFSIZE);
}
//-------------------picking------------------

void inserePontoControle(){
    ponto * p = new ponto();
    p->tx = 1; p->ty = 1; p->tz = 1;
    pontos.push_back(p);
}

void removePontoControle(){
    if(pontos.size() > 0){
        pontos.pop_back();
    }

}

void removePontoControleSelecionado(){
    if(pontos.size() > 0 && pontoSelecionado >= 0){
        pontos.erase(pontos.begin()+pontoSelecionado-1);
        pontoSelecionado = -1;
    }

}

//-------------------viewPorts------------------
bool viewports = false;
bool scissored = false;

void cenario();

//visao de duas cameras (duas viewports), viewport auxiliar sobrepondo a principal
void viewPorts() {
    float width = glutGUI::width;
    float height = glutGUI::height;

    //viewport principal
    glViewport(0, 0, width, height);
        glLoadIdentity();
        gluLookAt(glutGUI::cam->e.x,glutGUI::cam->e.y,glutGUI::cam->e.z, glutGUI::cam->c.x,glutGUI::cam->c.y,glutGUI::cam->c.z, glutGUI::cam->u.x,glutGUI::cam->u.y,glutGUI::cam->u.z);
            cenario();

    //viewport auxiliar sobrepondo a principal
    if (!scissored) {
        //misturando com a principal
        glViewport(0, 3*height/4, width/4, height/4);
    } else {
        //recortando/substituindo o pedaço
        GUI::glScissoredViewport(0, 3*height/4, width/4, height/4);
    }
        glLoadIdentity();
        Vetor3D eye = pontos[4]->getCoordenadas();
        Vetor3D center = pontos[2]->getCoordenadas();
        gluLookAt(eye.x,eye.y,eye.z, center.x,center.y,center.z, 0.0,1.0,0.0);
            cenario();

    // GoPro
    if (!scissored) {
         //misturando com a principal
         glViewport(width - (width/4.0), height - (height / 4.0), width/4.0, height/4.0);
     } else {
         //recortando/substituindo o pedaço
         GUI::glScissoredViewport(width - (width/4.0), height - (height / 4.0), width/4.0, height/4.0);
     }
        glViewport(width - (width/4.0), height - (height / 4.0), width/4.0, height/4.0);
        glLoadIdentity();
        gluLookAt(goPro->e.x,goPro->e.y,goPro->e.z,goPro->c.x,goPro->c.y,goPro->c.z, goPro->u.x,goPro->u.y,goPro->u.z);
        cenario();


}
//-------------------viewPorts------------------

void cenario() {
    //GUI::setLight(1,1,3,5,true,false);
    //GUI::setLight(2,-1.5,0.5,-1,true,false);
    GUI::setLight(3,-5,3,5,true,false);

    GUI::drawOrigin(0.5);

    //GUI::setColor(1,0,0);
    //GUI::drawFloor();
    GUI::setColor(0,0,0);
    Desenha::drawGrid( 5, 0, 1, 1 );
    desenhaPontosDeControle();
}

void desenhaObjetoNoTrilho(){
    Vetor3D ponto;

    int i = (int)delta;
    if (spline) {
        if (spline->P && !spline->Pn.empty()) {
            spline->P[0] = spline->Pn[i];
            spline->P[1] = spline->Pn[i + 1];
            spline->P[2] = spline->Pn[i + 2];
            spline->P[3] = spline->Pn[i + 3];
        }
    }

    float u = delta-i;
    Vetor3D z_ = spline->d1(u);
    Vetor3D y_ = spline->d2(u);
    Vetor3D x_ = y_ ^ z_;
            y_ = z_ ^ x_;
    Vetor3D t_ = spline->PTu(u);
    z_.normaliza();
    y_.normaliza();
    x_.normaliza();

    double Tranz[] = {
        x_.x, y_.x, z_.x, t_.x,
        x_.y, y_.y, z_.y, t_.y,
        x_.z, y_.z, z_.z, t_.z,
        0   , 0   , 0   , 1
    };

    glPushMatrix();
        glMultTransposeMatrixd(Tranz);
        glRotated(90, 0, 1, 0);
        trem->desenha();
    glPopMatrix();


    goPro->e = spline->PTu(delta - 20  * 0.01) ;
    goPro->c = spline->PTu(delta) + spline->d1(delta) * -1;
    goPro->u = (spline->d2(delta - 20  * 0.01)).getUnit();
    goPro->e = goPro->u + goPro->e;


}


void desenha() {
    GUI::displayInit();

    if (!viewports) {
        cenario();
    } else {
        viewPorts();
    }


    //transladando ponto selecionado atraves do picking
    //if (pontoSelecionado > 0 and pontoSelecionado <= objetos.size()) {
    if (pontoSelecionado!=0) {
        pontos[pontoSelecionado-1]->tx += glutGUI::dtx;
        pontos[pontoSelecionado-1]->ty += glutGUI::dty;
        pontos[pontoSelecionado-1]->tz += glutGUI::dtz;
    }

    vector<Vetor3D> pontosControle;
        for(int i = 0; i < (int) pontos.size(); i++){
            pontosControle.push_back(pontos.at(i)->getCoordenadas());
        }
        spline->atualizarPontosDeControle(pontosControle);
        spline->desenhaTrilho();
        desenhaObjetoNoTrilho();

    GUI::displayEnd();
}

void teclado(unsigned char key, int x, int y) {
    GUI::keyInit(key,x,y);

    switch (key) {
    case 't':
        glutGUI::trans_obj = !glutGUI::trans_obj;
        //transPontos = glutGUI::trans_obj;
        break;
    case 'l':
        glutGUI::trans_luz = !glutGUI::trans_luz;
        break;

    case 'f'://Progressão com o objeto
        if(spline->getTipo() == 5){
            if(delta > 8.85){
                delta = 0.01;
            }else{
               delta += 0.01;
            }
        }else{
            if(delta >= 1){
                delta = 0.01;
            }else{
               delta += 0.01;
            }
        }
        break;

    case 'd'://Regressão com o objeto
        if(spline->getTipo() == 5){
            if(delta <= 0){
                delta = 8.85;
            }else{
               delta -= 0.01;
            }
        }else{
            if(delta <= 0){
                delta = 0.99;
            }else{
               delta -= 0.01;
            }
        }
        break;
    case 'u':
        inserePontoControle();
        break;
    case 'i':
        removePontoControle();
        break;
    case 'k':
        removePontoControleSelecionado();
        break;

    case 'v':
        viewports = !viewports;
        break;
    case 's':
        scissored = !scissored;
        break;
    case 'N': //move o ponto selecionado no eixo x
        pontos.at(pontoSelecionado)->tx = pontos.at(pontoSelecionado)->tx + .1;
        break;
    case 'n': //move o ponto selecionado no eixo x
        pontos.at(pontoSelecionado)->tx = pontos.at(pontoSelecionado)->tx - .1;
        break;

    case 'Q': //move o ponto selecionado no eixo y
        pontos.at(pontoSelecionado)->ty = pontos.at(pontoSelecionado)->ty + .1;
        break;
    case 'q': //move o ponto selecionado no eixo y
        pontos.at(pontoSelecionado)->ty = pontos.at(pontoSelecionado)->ty - .1;
        break;

    case 'A': //move o ponto selecionado no eixo z
        pontos.at(pontoSelecionado)->tz = pontos.at(pontoSelecionado)->tz + .1;
        break;
    case 'a': //move o ponto selecionado no eixo z
        pontos.at(pontoSelecionado)->tz = pontos.at(pontoSelecionado)->tz - .1;
        break;
    case 'I':{
                ponto * p = new ponto;
                p = new ponto;
                p->tx = -10; p->ty = 10; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = -5; p->ty = 1; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 0; p->ty = 5; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 5; p->ty = 10; p->tz = 0;
                pronto.push_back(p);


                pontos.clear();



                for(ponto * p : pronto){
                   pontos.push_back(p);
                }

                pronto.clear();

                spline->setTipo(1);
                delta = 0.01;
                glutSetWindowTitle("Interpoladora");
                break;
                }

            case 'B':{
                ponto * p = new ponto;
                p = new ponto;
                p->tx = -10; p->ty = 10; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = -5; p->ty = 1; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 0; p->ty = 5; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 5; p->ty = 10; p->tz = 0;
                pronto.push_back(p);

                pontos.clear();

                for(ponto * p : pronto){
                   pontos.push_back(p);
                }

                pronto.clear();

                spline->setTipo(2);
                delta = 0.01;
                glutSetWindowTitle("Bezier");
                break;
                }

            case 'H':{
                ponto * p = new ponto;
                p = new ponto;
                p->tx = -10; p->ty = 10; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = -5; p->ty = 1; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 0; p->ty = 1; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 5; p->ty = 10; p->tz = 0;
                pronto.push_back(p);

                pontos.clear();

                for(ponto * p : pronto){
                   pontos.push_back(p);
                }

                pronto.clear();

                spline->setTipo(3);
                delta = 0.01;
                glutSetWindowTitle("Hermite");
                break;
                }

            case 'C':{
                ponto * p = new ponto;
                p->tx = -25; p->ty = -3; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = -25; p->ty = 3; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = -15; p->ty = 5; p->tz = -3;
                pronto.push_back(p);
                p = new ponto;
                p->tx = -10; p->ty = 5; p->tz = -3;
                pronto.push_back(p);
                p = new ponto;
                p->tx = -5; p->ty = 8; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 0; p->ty = 10; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 5; p->ty = 1; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 10; p->ty = 5; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 15; p->ty = 10; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 20; p->ty = 10; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 25; p->ty = 5; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 30; p->ty = 1; p->tz = 0;
                pronto.push_back(p);

                pontos.clear();

                for(ponto * p : pronto){
                   pontos.push_back(p);
                }

                pronto.clear();

                spline->setTipo(4);
                delta = 0.01;
                glutSetWindowTitle("Catmull-Rom");
                break;
                }

            case 'S':{
                ponto * p = new ponto;
                p->tx = -25; p->ty = -3; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = -25; p->ty = 3; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = -15; p->ty = 5; p->tz = -3;
                pronto.push_back(p);
                p = new ponto;
                p->tx = -10; p->ty = 5; p->tz = -3;
                pronto.push_back(p);
                p = new ponto;
                p->tx = -5; p->ty = 8; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 0; p->ty = 10; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 5; p->ty = 1; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 10; p->ty = 5; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 15; p->ty = 10; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 20; p->ty = 10; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 25; p->ty = 5; p->tz = 0;
                pronto.push_back(p);
                p = new ponto;
                p->tx = 30; p->ty = 1; p->tz = 0;
                pronto.push_back(p);

                pontos.clear();

                for(ponto * p : pronto){
                    pontos.push_back(p);
                }

                pronto.clear();

                spline->setTipo(5);
                delta = 0.01;
                glutSetWindowTitle("B-Spline");
                break;
                }
    default:
        break;
    }
}

void mouse(int button, int state, int x, int y) {
    GUI::mouseButtonInit(button,state,x,y);

    // if the left button is pressed
    if (button == GLUT_LEFT_BUTTON) {
        // when the button is pressed
        if (state == GLUT_DOWN) {
            //picking
            int pick = picking( x, y, 5, 5 );
            if (pick != 0) {
                pontoSelecionado = pick;
                glutGUI::lbpressed = false;
            }
        }
    }
}

int main()
{
    cout << "Hello World!" << endl;

    GUI gui = GUI(800,600,desenha,teclado,mouse);
}
