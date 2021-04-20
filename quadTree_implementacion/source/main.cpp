#include <iostream>
#include <vector>

using namespace std;

//COORDENADA
struct Punto
{
    int x, y;
    Punto(int _x = 0, int _y = 0) {
        x = _x;
        y = _y;
    }
};

//CUADRADO QUE CONTENDRA LOS PUNTOS
class Marco
{
public:
    Marco(int x, int y, int a);
    int pmX, pmY, apotema;   
    bool contienePto(Punto pto);
};

Marco::Marco(int x = 0, int y = 0, int a = 0)
{
    pmX = x;
    pmY = y;
    apotema = a;
}

bool Marco::contienePto(Punto pto) {
    return(pto.x >= pmX - apotema && pto.x < pmX + apotema && pto.y >= pmY - apotema && pto.y < pmY + apotema);
}


//QUADTREE
class Quadtree
{
public:
    Quadtree(Marco limite, int n);
    bool insertarPto(Punto pto);

private:
    Marco limiteMarco;
    int profundidad;
    vector<Punto> Puntos;
    bool dividido;

    Quadtree* derechaArriba = 0;
    Quadtree* derechaAbajo = 0;
    Quadtree* izquierdaAbajo = 0;
    Quadtree* izquierdaArriba = 0;
    vector<Punto> Puntos;


    void dividir();
};

Quadtree::Quadtree(Marco limite, int n)
{
    limiteMarco = limite;
    profundidad = n;
    dividido = false;
}

bool Quadtree::insertarPto(Punto pto) {
    if (!limiteMarco.contienePto(pto))
        return false;

    if (profundidad == 0) {
        Puntos.push_back(pto);
        return true;
    }
    else {
        if (!dividido) {
            dividir();
        }
        if (derechaArriba->insertarPto(pto))
            return true;
        if (derechaAbajo->insertarPto(pto))
            return true;
        if (izquierdaAbajo->insertarPto(pto))
            return true;
        if (izquierdaArriba->insertarPto(pto))
            return true;
    }
}

void Quadtree::dividir() {
    Marco DAb(limiteMarco.pmX + limiteMarco.apotema/2, limiteMarco.pmY - limiteMarco.apotema / 2, limiteMarco.apotema/2);
    Marco DAr(limiteMarco.pmX + limiteMarco.apotema / 2, limiteMarco.pmY + limiteMarco.apotema / 2, limiteMarco.apotema / 2);
    Marco IAb(limiteMarco.pmX - limiteMarco.apotema / 2, limiteMarco.pmY - limiteMarco.apotema / 2, limiteMarco.apotema / 2);
    Marco IAr(limiteMarco.pmX - limiteMarco.apotema / 2, limiteMarco.pmY + limiteMarco.apotema / 2, limiteMarco.apotema / 2);
    
    derechaAbajo = new Quadtree(DAb, profundidad-1);
    derechaArriba = new Quadtree(DAr, profundidad - 1);
    izquierdaAbajo = new Quadtree(IAb, profundidad - 1);
    izquierdaArriba = new Quadtree(IAr, profundidad - 1);

    dividido = true;
}

//CODIGO PRINCIPAL
int main(){
    
    Marco marcoExterior(50,50,50);
    Quadtree QT(marcoExterior, 2);
    Punto p1(24,60);
    Punto p2(50, 90);


    return 0;
}