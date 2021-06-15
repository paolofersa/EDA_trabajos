#include <iostream>
#include <climits>		//INT_MAX, INT_MIN
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <vector>
#include <math.h>

using namespace std;
//ESTRUCTURA DE PUNTO
class Punto
{
public:
	Punto(double x, double y);
	~Punto();
	double coor_x, coor_y;
	int agrupado;
	double dist_min;

	double distancia_ptos(Punto pto);
private:

};

Punto::Punto(double x = 0, double y = 0)
{
	coor_x = x;
	coor_y = y;
	agrupado = -1;	//numero de centroide al cual pertenece
	dist_min = INT_MAX;
}

double Punto::distancia_ptos(Punto pto) {
	return sqrt(pow(pto.coor_x - coor_x, 2) + pow(pto.coor_y - coor_y, 2));
}

Punto::~Punto()
{
}

//ESCRIBIR EN UN ARCHIVO LOS DATOS AGRUPADOS
void escribirResultado(vector<Punto>* coordenadas) {
	ofstream myfile;
	myfile.open("coordenadas.csv");
	myfile << "longitud,latitud,agrupacion" << endl;

	for (vector<Punto>::iterator it = coordenadas->begin(); it != coordenadas->end();
		++it) {
		myfile << it->coor_x << "," << it->coor_y << "," << it->agrupado << endl;
	}
	myfile.close();
}

//ALGORITMO K-MEANS
void kMeans(vector<Punto>* coordenadas, int iteraciones, int cant_cluster) {
	vector<Punto> centroides;
	int n = coordenadas->size();
	srand(time(0));
	//ASIGNACION DE VALORES CENTROIDES
	for (int i = 0; i < cant_cluster; ++i) {
		centroides.push_back(coordenadas->at(rand() % n));
	}

	//BUCLE POR CADA ITERACION
	for (int i = 0; i < iteraciones; ++i) {
		//BUCLE SOBRE LA LISTA DE CENTROIDES
		for (vector<Punto>::iterator c = begin(centroides); c != end(centroides); ++c) {
			int identificador = c - begin(centroides);
			//BUCLE SOBRE LAS COORDENADAS
			for (vector<Punto>::iterator it = coordenadas->begin(); it != coordenadas->end(); ++it) {
				Punto p = *it;
				double dist = c->distancia_ptos(p);

				if (dist < p.dist_min) {
					p.dist_min = dist;
					p.agrupado = identificador;
				}
				*it = p;
			}
		}
	}

	vector<int> nuevosPuntos;
	vector<double> sumX, sumY;

	for (int j = 0; j < cant_cluster; ++j) {
		nuevosPuntos.push_back(0);
		sumX.push_back(0.0);
		sumY.push_back(0.0);
	}

	//CALCULAR VALORES PROMEDIOS PARA ACTUALIZACION DE CLUSTERS
	for (vector<Punto>::iterator it = coordenadas->begin(); it != coordenadas->end();
		++it) {
		int identificador = it->agrupado;
		nuevosPuntos[identificador] += 1;
		sumX[identificador] += it->coor_x;
		sumY[identificador] += it->coor_y;

		it->dist_min = INT_MAX;
	}
	//CALCULO DE NUEVOS VALORES PARA CENTROIDES
	for (vector<Punto>::iterator c = begin(centroides); c != end(centroides);
		++c) {
		int identificador = c - begin(centroides);
		c->coor_x = sumX[identificador] / nuevosPuntos[identificador];
		c->coor_y = sumY[identificador] / nuevosPuntos[identificador];
	}//fin de bucle segun iteraciones

	//ESCRIBIR RESULTADOS EN UN ARCHIVO PARA POSTERIOR DIAGRAMA
	escribirResultado(coordenadas);
}

//CODIGO PRINCIPAL
int main(){
	//OBTENER COORDENADAS DE ARCHIVO
	vector<Punto> coordenadas;
	string linea;
	ifstream file("datos_resumidos.csv");

	while (getline(file, linea)) {
		stringstream lineStream(linea);
		string bit;
		double x, y;
		getline(lineStream, bit, ',');
		x = stof(bit);
		getline(lineStream, bit, '\n');
		y = stof(bit);

		coordenadas.push_back(Punto(x, y));
	}

	//INSERTAR PUNTOS A ALGORITMO
	int it, cl;
	cout << "Insertar cantidad de ITERACIONES" << endl;
	cin >> it;
	cout << "Insertar cantidad de CLUSTERS" << endl;
	cin >> cl;
	kMeans(&coordenadas, it, cl);

    return 0;
}