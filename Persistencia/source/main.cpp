#include <iostream>
#include <climits>		//INT_MAX, INT_MIN
#include <ctime>
#include <vector>
#include <math.h>
#include <string>
#include <tuple>

using namespace std;
//NODO PARA ED
template <class T>
struct CNode
{
	T value;
	CNode* next;
	int nOfVersion;
	vector <tuple<int, T, CNode*>> record;	//#version, value, next
	CNode(T _v) {
		value = _v;
		next = 0;
		nOfVersion = 0;
	}
};

//ESTRUCTURA DE DATOS
template <class T>
class CPersStruct
{
public:
	CPersStruct();
	~CPersStruct();
	void print();
	void insertar(int nodePos, string field, T newValue);
	void buscar(int nodePos, int version, string field);

	//static int globalVersion;

private:
	CNode<T>* head;
};

template <class T>
CPersStruct<T>::CPersStruct()
{
	head = 0;
}

template <class T>
CPersStruct<T>::~CPersStruct()
{
	CNode<T>* a = head, *b;
	while (a)
	{
		b = a->next;
		//recorrer historial de versiones buscando punteros
		for (int i = 0; i < a->record.size(); i++) {
			if (get<2>(a->record[i]) != a->next) {
				delete get<2>(a->record[i]);
			}
		}
		delete a;
		a = b;
	}
}

template <class T>
void CPersStruct<T>::print() {
	cout << "..." << endl;
	for (CNode<T>* a = head;
		a;
		a = a->next) {
		cout << "Valor inicial: " << a->value;
		//recorrer historial de versiones
		for (int i = 0; i < a->record.size(); i++) {
			cout << "\tv" << i+1 << ". ";
			//si se modifico un puntero next
			if (get<2>(a->record[i]) != a->next)
				cout << "PunteroNext con valor " << (get<2>(a->record[i]))->value;
			else
				cout << "Valor " << get<1>(a->record[i]);
		}
		cout << endl;
	}
	cout << "..." << endl;
}

template <class T>
void CPersStruct<T>::insertar(int nodePos, string field, T newValue) {
	//determinar posicion de cambio
	CNode<T>** p = &head;
	for (int i = 0;
		p && i < nodePos;
		p = &((*p)->next), i++);
	//determinar campo
	if (field == "puntero") {
		//determinar si es el primer cambio
		if (!(*p)) {
			CNode<T>* n = new CNode<T>(newValue);
			n->next = *p;
			*p = n;
		}
		else {
			(*p)->nOfVersion++;
			CNode<T>* n = new CNode<T>(newValue);
			n->next = *p;
			(*p)->record.push_back(
				make_tuple((*p)->nOfVersion,
					(*p)->value,
					n));
		}
	}
	else {
		(*p)->nOfVersion++;
		(*p)->record.push_back(
			make_tuple((*p)->nOfVersion,
				newValue,
				(*p)->next));
	}
}
template <class T>
void CPersStruct<T>::buscar(int nodePos, int version, string field) {
	//determinar posicion de cambio
	CNode<T>** p = &head;
	for (int i = 0;
		p && i < nodePos;
		p = &((*p)->next), i++);
	//consultar si es la version 0
	if (version == 0) {
		//determinar campo
		if (field == "puntero") {
			cout << "El punteroNext en la version " << version 
				<< " corresponde al nodo de valor: " << ((*p)->next)->value << endl;
		}
		else {
			cout << "El valor en la version " << version
				<< " es de: " << (*p)->value << endl;
		}
	}
	else {
		//determinar campo
		if (field == "puntero") {
			cout << "El punteroNext en la version " << version
				<< " corresponde al nodo de valor: " << (get<2>((*p)->record[version - 1]))->value << endl;
		}
		else {
			cout << "El valor en la version " << version
				<< " es de: " << get<1>((*p)->record[version - 1]) << endl;
		}
	}
}


//CODIGO PRINCIPAL
int main(){
	//CREACION DE ESTRUCTURA
	CPersStruct<int> estructure;
	/*
	* CPersStruct.insertar(nodePosition<int>,
							field<string>("puntero" or "valor"),
							value<int>(field=="puntero"->value for new next node, field=="valor"->new value for the node))
	* CPersStruct.buscar(nodePosition<int>,
							version<int>,
							field<string>("puntero" or "valor"))
	*/
	estructure.insertar(0, "puntero", 5);	//position's node == 0 with initial value 5
	estructure.insertar(1, "puntero", 10);	//position's node == 1 with initial value 10
	estructure.insertar(2, "puntero", 8);	//position's node == 2 with initial value 8

	estructure.print();
	//MODIFICACION DE VALORES
	estructure.insertar(1, "valor", 20);	//version 1 - node with initial value 10
	estructure.insertar(1, "valor", 40);	//version 2 - node with initial value 10
	estructure.insertar(2, "valor", 16);	//version 1 - node with initial value 8
	//CONSULTA DE VERSIONES
	estructure.buscar(1, 2, "valor");
	estructure.buscar(2, 1, "valor");
	//MODIFICACION DE PUNTEROS NEXT
	estructure.insertar(0, "puntero", 100);	//version 1 - node with initial value 5
	estructure.insertar(1, "puntero", 200);	//version 3 - node with initial value 10
	estructure.insertar(1, "puntero", 400);	//version 4 - node with initial value 10
	estructure.insertar(2, "puntero", 500);	//version 2 - node with initial value 8
	//CONSULTA DE VERSIONES
	estructure.buscar(1, 3, "puntero");
	estructure.buscar(2, 2, "puntero");
	estructure.buscar(0, 1, "puntero");
	estructure.buscar(1, 3, "puntero");

	estructure.print();
    return 0;
}