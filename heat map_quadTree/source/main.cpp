#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>

using namespace std;

//COORDENADA
struct Punto
{
    double x, y;
    Punto(double _x = 0, double _y = 0) {
        x = _x;
        y = _y;
    }
};

//CUADRADO QUE CONTENDRA LOS PUNTOS
class Marco
{
public:
    Marco(double x, double y, double a);
    double pmX, pmY, apotema;
    bool contienePto(Punto pto);
};

Marco::Marco(double x = 0, double y = 0, double a = 0)
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
    ~Quadtree();
    bool insertarPto(Punto pto);

private:
    Marco limiteMarco;
    int profundidad;
    vector<Punto> Puntos;
    bool dividido;
    void eliminarSubDIvision(Quadtree* destructor);

    Quadtree* derechaArriba;
    Quadtree* derechaAbajo;
    Quadtree* izquierdaAbajo;
    Quadtree* izquierdaArriba;

    void dividir();
};

Quadtree::Quadtree(Marco limite, int n)
{
    limiteMarco = limite;
    profundidad = n;
    dividido = false;
    derechaArriba = derechaAbajo = izquierdaAbajo = izquierdaArriba = 0;
}

Quadtree::~Quadtree()
{
    if (dividido) {
        eliminarSubDIvision(derechaArriba);
        eliminarSubDIvision(derechaAbajo);
        eliminarSubDIvision(izquierdaAbajo);
        eliminarSubDIvision(izquierdaArriba);
    }
}

void Quadtree::eliminarSubDIvision(Quadtree* destructor) {
    if (!destructor->dividido)
        return;
    else
    {
        eliminarSubDIvision(destructor->derechaArriba);
        eliminarSubDIvision(destructor->derechaAbajo);
        eliminarSubDIvision(destructor->izquierdaAbajo);
        eliminarSubDIvision(destructor->izquierdaArriba);
        delete destructor->derechaArriba;
        delete destructor->derechaAbajo;
        delete destructor->izquierdaAbajo;
        delete destructor->izquierdaArriba;
    }
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
    
    ofstream f;
    ifstream in;

    Marco marcoExterior(50,50,50);
    Quadtree QT(marcoExterior, 2);

    //abrir archivo
    in.open("provisional.txt", ios::app);
    if (in.fail())
    {
        cout << "Error al abrir" << endl;
        system("pause");
        exit(1);
    }

    Punto p1(24,60);
    Punto p2(50, 90);
    Punto p3(70, 30);
    Punto p4(20, 20);

    QT.insertarPto(p1);
    cout << "-------------------------------------------------------------------------------\n";
    QT.insertarPto(p2);
    cout << "-------------------------------------------------------------------------------\n";
    QT.insertarPto(p3);
    cout << "-------------------------------------------------------------------------------\n";
    QT.insertarPto(p4);
    cout << "-------------------------------------------------------------------------------\n";

    return 0;
}