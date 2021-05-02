#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <sstream>

using namespace std;

//COORDENADA
struct Punto
{
    long double x, y;
    Punto(long double _x = 0, long double _y = 0) {
        x = _x;
        y = _y;
    }
};

//CUADRADO QUE CONTENDRA LOS PUNTOS
class Marco
{
public:
    Marco(long double x, long double y, long double _ancho, long double _alto);
    long double pmX, pmY, ancho, alto;
    bool contienePto(Punto pto);
};

Marco::Marco(long double x = 0, long double y = 0, long double _ancho = 0, long double _alto = 0)
{
    pmX = x;
    pmY = y;
    ancho = _ancho;
    alto = _alto;
}

bool Marco::contienePto(Punto pto) {
    return(pto.x >= pmX - ancho && pto.x < pmX + ancho && pto.y >= pmY - alto && pto.y < pmY + alto);
}


//QUADTREE
class Quadtree
{
public:
    Quadtree(Marco limite, int n);
    ~Quadtree();
    bool insertarPto(Punto pto);
    Marco limiteMarco;
    int profundidad;
    vector<Punto> Puntos;
    bool dividido;
    
    Quadtree* derechaArriba;
    Quadtree* derechaAbajo;
    Quadtree* izquierdaAbajo;
    Quadtree* izquierdaArriba;

    static int cantidadMaxPtos;

private:
    void eliminarSubDIvision(Quadtree* destructor);
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
        if (Puntos.size() > cantidadMaxPtos)
            cantidadMaxPtos = Puntos.size();
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
    Marco DAb(limiteMarco.pmX + limiteMarco.ancho/2, limiteMarco.pmY - limiteMarco.alto / 2, limiteMarco.ancho/2, limiteMarco.alto / 2);
    Marco DAr(limiteMarco.pmX + limiteMarco.ancho / 2, limiteMarco.pmY + limiteMarco.alto / 2, limiteMarco.ancho / 2, limiteMarco.alto / 2);
    Marco IAb(limiteMarco.pmX - limiteMarco.ancho / 2, limiteMarco.pmY - limiteMarco.alto / 2, limiteMarco.ancho / 2, limiteMarco.alto / 2);
    Marco IAr(limiteMarco.pmX - limiteMarco.ancho / 2, limiteMarco.pmY + limiteMarco.alto / 2, limiteMarco.ancho / 2, limiteMarco.alto / 2);
    
    derechaAbajo = new Quadtree(DAb, profundidad-1);
    derechaArriba = new Quadtree(DAr, profundidad - 1);
    izquierdaAbajo = new Quadtree(IAb, profundidad - 1);
    izquierdaArriba = new Quadtree(IAr, profundidad - 1);

    dividido = true;
}

void escribirCoordenadas(Quadtree* ptro){
    if (!ptro->dividido) {
        if (ptro->Puntos.size() > 0)
        {
            ofstream ofs;
            ofs.open("provisional.csv", ios::app);

            double num = ptro->Puntos.size();
            double den = Quadtree::cantidadMaxPtos;
            double valor = num / den;
            string color = valor > 0.9 ? "#800026" :
                valor > 0.8 ? "#BD0026" :
                valor > 0.7 ? "#E31A1C" :
                valor > 0.6 ? "#FC4E2A" :
                valor > 0.5 ? "#FD8D3C" :
                valor > 0.4 ? "#FEB24C" :
                valor > 0.2 ? "#FED976" :
                "#FFEDA0";

            ofs << ptro->limiteMarco.pmX - ptro->limiteMarco.ancho << "," <<
                ptro->limiteMarco.pmY + ptro->limiteMarco.alto << "," <<
                ptro->limiteMarco.pmX + ptro->limiteMarco.ancho << "," <<
                ptro->limiteMarco.pmY - ptro->limiteMarco.alto << "," <<
                color << endl;

            ofs.close();
        }
        return;
    }
    else
    {
        escribirCoordenadas(ptro->derechaArriba);
        escribirCoordenadas(ptro->derechaAbajo);
        escribirCoordenadas(ptro->izquierdaAbajo);
        escribirCoordenadas(ptro->izquierdaArriba);
    }
}

//ESTRUCTURA PARA REGISTRO
struct registro
{
    long double x;
    long double y;
};


//CODIGO PRINCIPAL
int Quadtree::cantidadMaxPtos = 0;

int main(){
    
    int profundidad;
    cout << "Ingresar profundidad deseada:\n";
    cin >> profundidad;

    Marco marcoExterior(0,0,180,90);      // (-180,90) y (180,-90)
    Quadtree QT(marcoExterior, profundidad);
    Quadtree* ptroQT = &QT;
    Punto pto;
    
    //ABRIR ARCHIVO Y LEER
    ifstream file("loc-brightkite_totalCheckins.tsv");
    vector <registro> v;

    string line;

    while (getline(file, line))
    {
        istringstream is(line);
        registro r;
        string s;
        //primer valor
        getline(is, s, '\t');
        r.x = stold(s);
        //segundo valor
        getline(is, s, '\n');
        r.y = stold(s);

        v.push_back(r);
    }
    //INSERTAR PUNTOS A ARBOL
    for (auto it = begin(v); it != end(v); ++it)
    {
        pto.x = it->x;
        pto.y = it->y;
        QT.insertarPto(pto);
    }
    //GUARDAR COORDENADAS EN TXT
    ofstream ofs;
    ofs.open("provisional.csv", ofstream::out | ofstream::trunc);
    ofs.close();

    escribirCoordenadas(ptroQT);

    cout << "\n Los datos necesarios para el MAPA DE CALOR se han almacenado satisfactoriamente\n";
    
    /*
    Punto p1(24,60);
    Punto p2(50, 90);
    Punto p3(70, 30);
    Punto p4(20, 20);
    Punto p5(60, 40);
    Punto p6(10, 60);
    Punto p7(55, 40);

    QT.insertarPto(p1);
    QT.insertarPto(p2);
    QT.insertarPto(p3);
    QT.insertarPto(p4);
    QT.insertarPto(p5);
    QT.insertarPto(p6);
    QT.insertarPto(p7);

    cout << "Cantidad maxima de ptos por cuadrante es:\t" << Quadtree::cantidadMaxPtos << endl;
    
    ofstream ofs;
    ofs.open("provisional.txt", ofstream::out | ofstream::trunc);
    ofs.close();

    escribirCoordenadas(ptroQT);
    */
    return 0;
}