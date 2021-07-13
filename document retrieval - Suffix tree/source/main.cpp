#include <iostream>
#include <vector>
#include <climits>		//INT_MAX, INT_MIN
#include <tuple>
#include <string>
#include <cstring>
#include <algorithm>    //max()
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <math.h>
#include <sstream>  //istringstream

using namespace std;


//SUFFIX ARRAY (SA)
vector<int> ordenarCaracteres(string S) {
    vector<int> orden(S.length());
    vector<int> contador(122, 0);   //cantidad igual al alfabeto
    char c;
    for (unsigned int i = 0; i <= S.length() - 1; i++)//solo menor
        contador[(int)S[i]] = contador[(int)S[i]] + 1;
    for (unsigned int j = 1; j <= contador.size() - 1; j++)
        contador[j] = contador[j] + contador[j - 1];
    for (unsigned int i = 0; i <= S.length() - 1; i++) {
        c = S[S.length() - 1 - i];
        contador[(int)c] = contador[(int)c] - 1;
        orden[contador[(int)c]] = S.length() - 1 - i;
    }
    return orden;
}

vector<int> calcularClasesDeCaracteres(string S, vector<int>& orden) {
    vector<int> clase(S.length());
    clase[orden[0]] = 0;
    for (unsigned int i = 1; i <= S.length() - 1; i++) {
        if (S[orden[i]] != S[orden[i - 1]])
            clase[orden[i]] = clase[orden[i - 1]] + 1;
        else
            clase[orden[i]] = clase[orden[i - 1]];
    }
    return clase;
}

vector<int> ordenarCicloDoble(string S, int L, vector<int>& orden, vector<int>& clase) {
    vector<int> contador(S.length());
    vector<int> nuevoOrden(S.length());
    int inicio, cDoble;
    for (unsigned int i = 0; i <= S.length() - 1; i++)
        contador[clase[i]] = contador[clase[i]] + 1;
    for (unsigned int j = 1; j <= S.length() - 1; j++)
        contador[j] = contador[j] + contador[j - 1];
    for (unsigned int i = 0; i <= S.length() - 1; i++) {
        inicio = (orden[S.length() - 1 - i] - L + S.length()) % S.length();
        cDoble = clase[inicio];
        contador[cDoble] = contador[cDoble] - 1;
        nuevoOrden[contador[cDoble]] = inicio;
    }
    return nuevoOrden;
}

vector<int> actualizarClasesDeCaracteres(vector<int>& nuevoOrden, vector<int>& clase, int L) {
    int n = nuevoOrden.size();
    int actual, anterior, medio, medioAnterior;
    vector<int> nuevaClase(n);
    nuevaClase[nuevoOrden[0]] = 0;
    for (unsigned int i = 1; i <= n - 1; i++) {
        actual = nuevoOrden[i];
        anterior = nuevoOrden[i - 1];
        medio = actual + L;
        medioAnterior = (anterior + L) % n;
        if (clase[actual] != clase[anterior] || clase[medio] != clase[medioAnterior])
            nuevaClase[actual] = nuevaClase[anterior] + 1;
        else
            nuevaClase[actual] = nuevaClase[anterior];
    }
    return nuevaClase;
}

vector<int> construirSuffixArray(string S) {
    vector<int> orden = ordenarCaracteres(S);
    vector<int> clase = calcularClasesDeCaracteres(S, orden);
    unsigned int L = 1;
    while (L < S.length()) {
        orden = ordenarCicloDoble(S, L, orden, clase);
        clase = actualizarClasesDeCaracteres(orden, clase, L);
        L = 2 * L;
    }
    return orden;
}

//PREFIJO COMUN MAS LARGO
int prefComunMasLargoDeSufijos(string S, int i, int j, int comparador) {
    unsigned int pcml = max(0, comparador);
    //comparacion de sufijos para cada caracter
    while (i + pcml < S.length() && j + pcml < S.length()) {
        //caracteres iguales?
        if (S[i + pcml] == S[j + pcml])
            pcml += 1;
        else
            break;
    }
    return pcml;
}

//funcion para retornar de cada posicion de S el lugar donde el sufijo inicia en la posicion y a cual corresponde en el SA
vector<int> suffixArrayInvertido(vector<int>& orden) {
    vector<int> pos(orden.size());
    for (unsigned int i = 0; i <= orden.size() - 1; i++)
        pos[orden[i]] = i;
    return pos;

}

vector<int> calcularArregloPCML(string S, vector<int>& orden) {
    vector<int> arregloPCML(S.length() - 1);   //arreglo resultante del calculo
    int indDelOrden, sigSufijo;
    int pcml = 0;               //inicialmente el prefijo mas largo es 0
    vector<int> posDelOrden = suffixArrayInvertido(orden);  //relacionar la posicion en la cadena S y la posicion en el SA
    int sufijo = orden[0];      //posicion que corresponde al sufijo mas corto
    //iterar sobre la cadena S
    for (unsigned int i = 0; i <= S.length() - 1; i++) {
        indDelOrden = posDelOrden[sufijo];      //posicion del sufijo en el SA 
        //el sufijo es el mas largo del SA?
        if (indDelOrden == S.length() - 1) {
            pcml = 0;
            sufijo = (sufijo + 1) % S.length();   //pasar al sgt sufijo de la cadena S
            continue;
        }
        sigSufijo = orden[indDelOrden + 1];
        pcml = prefComunMasLargoDeSufijos(S, sufijo, sigSufijo, pcml - 1);
        arregloPCML[indDelOrden] = pcml;
        sufijo = (sufijo + 1) % S.length();
    }
    return arregloPCML;
}

//SUFFIX TREE
//nodo
struct SuffixTreeNode
{
    SuffixTreeNode* padre;
    map<char, SuffixTreeNode*> hijos;
    int profundidadCad;
    int aristaInicial;
    int aristaFinal;
    SuffixTreeNode(SuffixTreeNode* p, int _sd, int _es, int _ee) {
        padre = p;
        profundidadCad = _sd;
        aristaInicial = _es;
        aristaFinal = _ee;
    }
};

SuffixTreeNode* nuevaHoja(SuffixTreeNode* nodo, string S, int sufijo) {
    SuffixTreeNode* hoja = new SuffixTreeNode(nodo, S.length() - sufijo, sufijo + nodo->profundidadCad, S.length() - 1);
    nodo->hijos[S[hoja->aristaInicial]] = hoja;
    return hoja;
}

SuffixTreeNode* dividirArista(SuffixTreeNode* nodo, string S, int inicio, int desplazamiento) {
    char caracterInicial = S[inicio];
    char caracterMedio = S[inicio + desplazamiento];
    SuffixTreeNode* nodoMedio = new SuffixTreeNode(nodo, nodo->profundidadCad + desplazamiento, inicio, inicio + desplazamiento - 1);
    nodoMedio->hijos[caracterMedio] = nodo->hijos[caracterInicial];
    nodo->hijos[caracterInicial]->padre = nodoMedio;
    nodo->hijos[caracterInicial]->aristaInicial += desplazamiento;
    nodo->hijos[caracterInicial] = nodoMedio;
    return nodoMedio;
}

SuffixTreeNode* suffixTreeDesdeSuffixArray(string S, vector<int>& orden, vector<int>& arregloPCML) {
    SuffixTreeNode* raiz = new SuffixTreeNode(nullptr, 0, -1, -1);
    int sufijo, aristaInicial, desplazamiento;
    int PCMLanterior = 0;
    SuffixTreeNode* nodoActual = raiz;
    SuffixTreeNode* nodoMedio;
    for (unsigned int i = 0; i <= S.length() - 1; i++) {
        sufijo = orden[i];
        while (nodoActual->profundidadCad > PCMLanterior)
            nodoActual = nodoActual->padre;
        if (nodoActual->profundidadCad == PCMLanterior)
            nodoActual = nuevaHoja(nodoActual, S, sufijo);
        else {
            aristaInicial = orden[i - 1] + nodoActual->profundidadCad;
            desplazamiento = PCMLanterior - nodoActual->profundidadCad;
            nodoMedio = dividirArista(nodoActual, S, aristaInicial, desplazamiento);
            nodoActual = nuevaHoja(nodoMedio, S, sufijo);
        }
        if (i < S.length() - 1)
            PCMLanterior = arregloPCML[i];
    }
    return raiz;
}

int conteoRecursivo(SuffixTreeNode* nodoActual) {
    if (nodoActual->hijos.size() == 0)
        return 1;
    else {
        int contNodo = 0;
        for (map<char, SuffixTreeNode*>::iterator it = nodoActual->hijos.begin(); it != nodoActual->hijos.end(); it++)
            contNodo += conteoRecursivo(it->second);
        return contNodo;
    }
}

int cantidadCoincidencias(SuffixTreeNode* nodo, string S, string P) {
    int contador = 0, posComparada;
    SuffixTreeNode* nodoActual = nodo;
    bool coincidencia = true;

    //DETERMINAR EL NODO BASE PARA LAS COINCIDENCIAS
    for (posComparada = 0; coincidencia && posComparada < P.length() - 1;) {
        //iterar sobre hijos del nodo actual para encontrar coincidencia
        for (map<char, SuffixTreeNode*>::iterator it = nodoActual->hijos.begin(); it != nodoActual->hijos.end(); it++) {
            if (it->first == P[posComparada]) {
                nodoActual = it->second;
                coincidencia = true;
                break;
            }
            else
                coincidencia = false;
        }
        //incrementar posicion comparada
        for (int i = nodoActual->aristaInicial; posComparada < P.length() - 1 && i <= nodoActual->aristaFinal; i++) {
            if (S[i] == P[posComparada]) {
                posComparada++;
                coincidencia = true;
            }
            else {
                coincidencia = false;
                break;
            }
        }
    }
    //en caso no hubo coincidencia completa
    if (!coincidencia)
        return 0;

    //CONTAR NODOS HOJA A PARTIR DEL NODO ACTUAL
    return conteoRecursivo(nodoActual);
}

//CODIGO PRINCIPAL
int main()
{
    //LEER ARCHIVO CSV
    vector<string> ids;
    vector<string> titulos;
    vector<string> abstracts;
    string linea;
    ifstream file("../preprocesamiento/datosBase_prueba.csv");

    while (getline(file, linea)) {
        stringstream lineStream(linea);
        string bit;
        getline(lineStream, bit, ',');
        ids.push_back(bit);
        getline(lineStream, bit, ',');
        titulos.push_back(bit);
        getline(lineStream, bit, '\n');
        abstracts.push_back(bit);
    }

    //CREACION DE ESTRUCTURA PARA ALMACENAMIENTO DE ABSTRACTS
    vector<SuffixTreeNode*> nodosRaiz;
    string S;
    for (int i = 0; i < ids.size(); i++) {
        S = abstracts[i];
        vector<int> orden = construirSuffixArray(S);
        vector<int> arregloPCML = calcularArregloPCML(S, orden);
        SuffixTreeNode* raiz = suffixTreeDesdeSuffixArray(S, orden, arregloPCML);
        nodosRaiz.push_back(raiz);
    }

    //INSERTAR PATRON DE BUSQUEDA
    string query;
    vector<string> P;
    cout << "Insertar consulta:\n";
    getline(cin, query);
    istringstream iss(query);
    for (query; iss >> query; )
        P.push_back(query);


    //CALCULAR COINCIDENCIAS
    int cantPalabrasQuery = P.size();
    
    int** matrCantidades = new int* [ids.size()];   //filas
    for (int i = 0; i < ids.size(); i++)
        matrCantidades[i] = new int[cantPalabrasQuery];    //[cantidadDocumentos][palabrasQuery]

    vector<int> docContienenPalabra;
    for (int j = 0; j < cantPalabrasQuery; j++) {
        int docContienenPalabrasPorConsulta = 0;
        for (int i = 0; i < ids.size(); i++) {
            int valor = cantidadCoincidencias(nodosRaiz[i], abstracts[i], P[j]);
            *(*(matrCantidades + i) + j) = valor;
            if (*(*(matrCantidades + i) + j) > 0)
                docContienenPalabrasPorConsulta++;
        }
        docContienenPalabra.push_back(docContienenPalabrasPorConsulta);
    }

    //CALCULAR CALIFICACION POR DOCUMENTO Y POR PALABRA DE QUERY
    cout << "\t\t";
    for (int i = 0; i < cantPalabrasQuery; i++)
        cout << P[0] << '\t';
    cout << endl;

    for (int i = 0; i < ids.size(); i++) {
        cout << ids[i] << ' :\t';
        for (int j = 0; j < cantPalabrasQuery; j++) {
            double calificacion = matrCantidades[i][j] * log(ids.size() / docContienenPalabra[j]);
            cout << calificacion << '\t';            
        }
        cout << endl;
    }
    cout << endl;


    //liberar memoria
    for (int i = 0; i < ids.size(); i++)
        delete[] matrCantidades[i];
    delete[] matrCantidades;

    /*
    string S = "ababaa$";
    vector<int> orden = construirSuffixArray(S);
    for (int i = 0; i < S.length(); i++)
        cout << orden[i] << '\t';
    cout << endl;

    vector<int> arregloPCML = calcularArregloPCML(S, orden);
    for (int i = 0; i < S.length() - 1; i++)
        cout << arregloPCML[i] << '\t';
    cout << endl;

    SuffixTreeNode* raiz = suffixTreeDesdeSuffixArray(S, orden, arregloPCML);

    string P = "a$";
    int coincidencias = cantidadCoincidencias(raiz, S, P);
    cout << "Cantidad de coincidencias es: " << coincidencias << endl;
    */
    return 0;
}