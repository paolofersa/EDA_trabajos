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
	agrupado = -1;
	dist_min = INT_MAX;
}

double Punto::distancia_ptos(Punto pto) {
	return sqrt(pow(pto.coor_x - coor_x, 2) + pow(pto.coor_y - coor_y, 2));
}

Punto::~Punto()
{
}


//CODIGO PRINCIPAL
int main(){
	
    return 0;
}