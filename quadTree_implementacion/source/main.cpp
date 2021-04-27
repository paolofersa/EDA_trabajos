#include <iostream>
#include <vector>

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
        cout << "Se ELIMINO el cuadrante de coordenada media:\t" << derechaArriba->limiteMarco.pmX << ", " << derechaArriba->limiteMarco.pmY << endl;
        //delete derechaArriba;
        cout << "Se ELIMINO el cuadrante de coordenada media:\t" << derechaAbajo->limiteMarco.pmX << ", " << derechaAbajo->limiteMarco.pmY << endl;
        //delete derechaAbajo;
        cout << "Se ELIMINO el cuadrante de coordenada media:\t" << izquierdaAbajo->limiteMarco.pmX << ", " << izquierdaAbajo->limiteMarco.pmY << endl;
        //delete izquierdaAbajo;
        cout << "Se ELIMINO el cuadrante de coordenada media:\t" << izquierdaArriba->limiteMarco.pmX << ", " << izquierdaArriba->limiteMarco.pmY << endl;
        //delete izquierdaArriba;
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
        cout << "Se ELIMINO el cuadrante de coordenada media:\t" << destructor->derechaArriba->limiteMarco.pmX << ", " << destructor->derechaArriba->limiteMarco.pmY << endl;
        delete destructor->derechaArriba;
        cout << "Se ELIMINO el cuadrante de coordenada media:\t" << destructor->derechaAbajo->limiteMarco.pmX << ", " << destructor->derechaAbajo->limiteMarco.pmY << endl;
        delete destructor->derechaAbajo;
        cout << "Se ELIMINO el cuadrante de coordenada media:\t" << destructor->izquierdaAbajo->limiteMarco.pmX << ", " << destructor->izquierdaAbajo->limiteMarco.pmY << endl;
        delete destructor->izquierdaAbajo;
        cout << "Se ELIMINO el cuadrante de coordenada media:\t" << destructor->izquierdaArriba->limiteMarco.pmX << ", " << destructor->izquierdaArriba->limiteMarco.pmY << endl;
        delete destructor->izquierdaArriba;
    }
}

bool Quadtree::insertarPto(Punto pto) {
    if (!limiteMarco.contienePto(pto))
        return false;

    if (profundidad == 0) {
        Puntos.push_back(pto);

        cout << "Se inserto el pto:\t" << pto.x << ", " << pto.y << endl;
        cout << "en el cuadrante de coordenada media:\t" << limiteMarco.pmX << ", " << limiteMarco.pmY << endl;
        cout << endl;

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

    cout << "Se DIVIDIO el cuadrante de coordenada media:\t" << limiteMarco.pmX << ", " << limiteMarco.pmY << endl;
    cout << endl;

    dividido = true;
}

//CODIGO PRINCIPAL
int main(){
    
    Marco marcoExterior(50,50,50);
    Quadtree QT(marcoExterior, 2);

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