#pragma once
#include <iostream>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <Windows.h>
using namespace std;

const int NUM_ATRIBUTOS = 5;
const int NUM_DATOS = 14;
const string FICHERO_ATRIBUTOS = "AtributosJuego.txt";
const string FICHERO_DATOS = "Juego.txt";

typedef struct {
	int contador;
	string lista[NUM_ATRIBUTOS];
} tLista;

typedef struct {
	string atributos[NUM_ATRIBUTOS];
	string datos[NUM_DATOS][NUM_ATRIBUTOS];
	int contador;
} tDatos;

typedef struct {	//he creado este struct para calcular mas facil la entropia general
	int p;
	int n;
}tPyN;

typedef struct {
	string atributo;
	string nombreDato;
	tPyN pyn;
	float entropia;
} tValoresDato;

typedef struct {
	int contador;
	tValoresDato distintosDatos[NUM_DATOS];
	float AIE; //esto es el average information entropy del video
} tDistintosDatos;

typedef tDistintosDatos tValoresAtributos[NUM_ATRIBUTOS - 1];

/*
tValoresAtributos[0 - 4]					  |TiempoExterior|Temperatura|Humedad|Viento|
													 |
tValoresAtributos[0].distintosDatos				 |soleado|lluvioso|nublado|
													 |
tValoresAtributos[0].distintosDatos[0]			|atributo|nombreDato|positivos|negativos|entropia|
									|														 |
					[tValoresAtributos[0].contador]				     tValoresAtributos[0].distintosDatos[0].entropia
*/

int main();
int mostrarEntropia(float entropiaTotal, tDatos datos, tValoresAtributos valoresAtributos, int modo);
void resultado(float entropiaTotal, tDatos& datos, tValoresAtributos& valoresAtributos, string& resultadoString, int modo);
void iteraciones(float entropiaTotal, tDatos& datos, string opcion, tValoresAtributos& valoresAtributos, string& resultadoString, int modo);
bool cargarDatos(tDatos& datos);
void mostrarDatos(tDatos datos);
void mostrarCalculos(tValoresAtributos valoresAtributos);
void datosDistintos(int atributo, tDatos datos, tValoresAtributos& valoresAtributos); //comprueba los distintos nombre de los valore para una columna (atributo) y lo almacena en tValoresAtributos
bool valorEnLista(tLista& valores, string valor); //comprueba si el nombre del valor ya está en la lista o no
int sumaPN(int atributo, string valor, tDatos datos, string tipo); //devuelve los p o n de la columna pasada (atributo) teniendo en cuenta el nombre del valor (valor) y si es sí o no (tipo)
float entropia(float pos, float neg);
float entropiaAIAtributo(float pos, float neg, float total, float entropiaAtrib);
tPyN PyNTotal(tDatos& datos);
float gain(float ent, float atrib);