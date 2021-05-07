#include "Main.h";

int main() {
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	tDatos datos; tValoresAtributos valoresAtributos; float entropiaTotal = 0; int modo; string resultadoString = "";
	datos.contador = NUM_DATOS;
	if (cargarDatos(datos)) {
		float sumaAI = 0;
		for (int i = 0; i < NUM_ATRIBUTOS - 1; i++) {
			datosDistintos(i, datos, valoresAtributos);
		}
	}
	cout << "Practica 2: Algoritmo ID3. Christian Esteban y Fernando Bellot" << endl;
	mostrarDatos(datos);
	do {
		cout << endl;
		cout << "Seleccione como desea ver el algoritmo ID3" << endl;
		cout << "1. Paso a paso" << endl;
		cout << "2. Resultado final (Arbol de decision)" << endl;
		cin >> modo;
	} while (modo < 1 || modo > 2);

	//ver1
	if (modo == 1) {
		mostrarCalculos(valoresAtributos);
	}
	//ver2
	resultado(entropiaTotal, datos, valoresAtributos, resultadoString, modo);
	cout << endl;
	cout << "Arbol de decision:" << endl << endl;
	cout << resultadoString;
	cout << endl << endl;
	system("pause");
	return 0;
}
int mostrarEntropia(float entropiaTotal, tDatos datos, tValoresAtributos valoresAtributos, int modo) {
	tPyN pynTot = PyNTotal(datos); //calculamos los positivos y negativos totales
	entropiaTotal = entropia(pynTot.p, pynTot.n); //calculamos la entropia total
	//ver1
	if (modo == 1) {
		cout << endl << setw(15) << "Atributo" << setw(12) << "Gain" << endl;
		for (int a = 0; a < 15 + 12; a++) cout << "-";
		cout << endl;
	}

	//ver2
	float maxGain = -1; int atributoMax = -1;
	for (int i = 0; i < NUM_ATRIBUTOS - 1; i++) {
		float gain = entropiaTotal - valoresAtributos[i].AIE;
		if (gain > maxGain) {
			maxGain = gain;
			atributoMax = i;
		}
		//ver1
		if (modo == 1) {
			cout << setw(15) << datos.atributos[i] << setw(12) << gain << endl;
		}
		//ver2
	}
	//ver1
	if (modo == 1) {
		cout << endl << "MAYOR GAIN -> " << datos.atributos[atributoMax] << " -> " << maxGain << endl << endl;
		system("pause");
	}
	//ver2
	return atributoMax;
}
void resultado(float entropiaTotal, tDatos& datos, tValoresAtributos& valoresAtributos, string& resultadoString, int modo) {
	int atributoMax = mostrarEntropia(entropiaTotal, datos, valoresAtributos, modo);
	resultadoString.insert(resultadoString.size(), valoresAtributos[atributoMax].distintosDatos[0].atributo);
	resultadoString.insert(resultadoString.size(), " {");
	for (int i = 0; i < valoresAtributos[atributoMax].contador; i++) {
		resultadoString.insert(resultadoString.size(), valoresAtributos[atributoMax].distintosDatos[i].nombreDato);
		resultadoString.insert(resultadoString.size(), ": ");
		if (valoresAtributos[atributoMax].distintosDatos[i].entropia == 0) {
			if (valoresAtributos[atributoMax].distintosDatos[i].pyn.p - valoresAtributos[atributoMax].distintosDatos[i].pyn.n > 0)
				resultadoString.insert(resultadoString.size(), "si ");
			else resultadoString.insert(resultadoString.size(), "no ");
		}
		else {
			iteraciones(entropiaTotal, datos, valoresAtributos[atributoMax].distintosDatos[i].nombreDato, valoresAtributos, resultadoString, modo);
		}

	}
	resultadoString.insert(resultadoString.size(), "} ");

}
void iteraciones(float entropiaTotal, tDatos& datos, string opcion, tValoresAtributos& valoresAtributos, string& resultadoString, int modo) {
	tDatos datosIteracion; int contador = 0;
	for (int i = 0; i < NUM_ATRIBUTOS; i++) {
		datosIteracion.atributos[i] = datos.atributos[i];
	}
	tValoresAtributos valoresAtributosIteracion;
	for (int i = 0; i < NUM_DATOS; i++) {
		if (datos.datos[i][0] == opcion) {
			for (int j = 0; j < NUM_ATRIBUTOS; j++) {
				datosIteracion.datos[contador][j] = datos.datos[i][j];
			}
			contador++;
		}
	}
	//ver1
	if (modo == 1) {

		transform(opcion.begin(), opcion.end(), opcion.begin(),
			[](unsigned char c) { return toupper(c); });

		cout << endl << opcion << endl << endl;

		for (int i = 0; i < NUM_ATRIBUTOS; i++) {
			cout << setw(14) << datos.atributos[i];
		}
		cout << endl;
		for (int i = 0; i < 14 * NUM_ATRIBUTOS; i++) cout << "-";
		cout << endl;
		for (int i = 0; i < contador; i++) {
			for (int j = 0; j < NUM_ATRIBUTOS; j++) {
				cout << setw(14) << datosIteracion.datos[i][j];
			}
			cout << endl;
		}
	}
	//ver2

	datosIteracion.contador = contador;
	for (int i = 0; i < NUM_ATRIBUTOS - 1; i++) {
		datosDistintos(i, datosIteracion, valoresAtributosIteracion);
	}
	//ver1
	if (modo == 1) {
		mostrarCalculos(valoresAtributosIteracion);
	}
	//ver2
	resultado(entropiaTotal, datosIteracion, valoresAtributosIteracion, resultadoString, modo);
}
bool cargarDatos(tDatos& datos) {
	ifstream archivo; string linea; bool cargado = true;
	//Leemos el fichero con los atributos
	archivo.open(FICHERO_ATRIBUTOS);
	if (archivo.is_open()) {
		getline(archivo, linea);
		stringstream atributos(linea);
		for (int i = 0; i < NUM_ATRIBUTOS; i++) {
			getline(atributos, datos.atributos[i], ',');
		}
		archivo.close();
	}
	else cargado = false;
	//Leemos el fichero con los datos
	archivo.open(FICHERO_DATOS);
	if (archivo.is_open()) {
		for (int i = 0; i < NUM_DATOS; i++) {
			getline(archivo, linea);
			stringstream atributos(linea);
			for (int j = 0; j < NUM_ATRIBUTOS; j++) {
				getline(atributos, datos.datos[i][j], ',');
			}
		}
		archivo.close();
	}
	else cargado = false;
	return cargado;
}
void mostrarDatos(tDatos datos) {
	cout << endl;
	for (int i = 0; i < NUM_ATRIBUTOS; i++) {
		cout << setw(14) << datos.atributos[i];
	}
	cout << endl;
	for (int i = 0; i < 14 * NUM_ATRIBUTOS; i++) cout << "-";
	cout << endl;
	for (int i = 0; i < NUM_DATOS; i++) {
		for (int j = 0; j < NUM_ATRIBUTOS; j++) {
			cout << setw(14) << datos.datos[i][j];
		}
		cout << endl;
	}
}
void mostrarCalculos(tValoresAtributos valoresAtributos) {
	cout << endl;
	for (int i = 0; i < NUM_ATRIBUTOS - 1; i++) {
		cout << endl << setw(15) << "Atributo" << setw(12) << "Valor" << setw(4) << "P" << setw(4) << "N" << setw(10) << "Entropia" << endl;
		for (int a = 0; a < 15 + 12 + 4 + 4 + 10; a++) cout << "-";
		cout << endl;
		for (int j = 0; j < valoresAtributos[i].contador; j++) {
			cout << setw(15) << valoresAtributos[i].distintosDatos[j].atributo <<
				setw(12) << valoresAtributos[i].distintosDatos[j].nombreDato <<
				setw(4) << valoresAtributos[i].distintosDatos[j].pyn.p <<
				setw(4) << valoresAtributos[i].distintosDatos[j].pyn.n <<
				setw(10) << valoresAtributos[i].distintosDatos[j].entropia << endl;
		}
		cout << endl << "Average Information Entropy (" << valoresAtributos[i].distintosDatos[0].atributo << "): " << valoresAtributos[i].AIE << endl;
	}
}
void datosDistintos(int atributo, tDatos datos, tValoresAtributos& valoresAtributos) {
	tLista valores; string valor;
	valores.contador = 0;
	//Guardamos en tLista valores el número de valores distintos para un atributo y el nombre de estos
	for (int i = 0; i < datos.contador; i++) {
		valor = datos.datos[i][atributo];
		if (!valorEnLista(valores, valor)) {
			valores.lista[valores.contador] = datos.datos[i][atributo];
			valores.contador++;
		}
	}
	//Guardamos en valoresAtributos todos los datos de cada uno
	valoresAtributos[atributo].contador = valores.contador;
	valoresAtributos[atributo].AIE = 0;
	float valorAverageInfEntropia;
	for (int i = 0; i < valores.contador; i++) {
		valoresAtributos[atributo].distintosDatos[i].atributo = datos.atributos[atributo];
		valoresAtributos[atributo].distintosDatos[i].nombreDato = valores.lista[i];
		valoresAtributos[atributo].distintosDatos[i].pyn.p = sumaPN(atributo, valores.lista[i], datos, "si");
		valoresAtributos[atributo].distintosDatos[i].pyn.n = sumaPN(atributo, valores.lista[i], datos, "no");
		valoresAtributos[atributo].distintosDatos[i].entropia = entropia(valoresAtributos[atributo].distintosDatos[i].pyn.p, valoresAtributos[atributo].distintosDatos[i].pyn.n);
		valorAverageInfEntropia = entropiaAIAtributo(valoresAtributos[atributo].distintosDatos[i].pyn.p, valoresAtributos[atributo].distintosDatos[i].pyn.n, datos.contador, valoresAtributos[atributo].distintosDatos[i].entropia);
		//aqui calculamos el average information entropy de cada valor del atributos y lo vamos sumando en cada vuelta, 
		//para tener al final del bucle el average information entropy total de ese atributo
		valoresAtributos[atributo].AIE += valorAverageInfEntropia;
	}
}
bool valorEnLista(tLista& valores, string valor) {
	bool estaEnLista = false;
	for (int i = 0; i < valores.contador; i++) {
		if (valores.lista[i] == valor) estaEnLista = true;
	}
	return estaEnLista;
}
int sumaPN(int atributo, string valor, tDatos datos, string tipo) {
	int contador = 0;
	for (int i = 0; i < NUM_DATOS; i++) {
		if (datos.datos[i][atributo] == valor && datos.datos[i][NUM_ATRIBUTOS - 1] == tipo) {
			contador++;
		}
	}
	return contador;
}
float entropia(float pos, float neg) {
	float ret = ((-pos / (pos + neg)) * (log2((pos / (pos + neg))))) - ((neg / (pos + neg)) * (log2(neg / (pos + neg))));
	float i = (log2(neg / (pos + neg)));

	if (isnan(ret)) {
		ret = 0;
	}
	return ret;
}
float entropiaAIAtributo(float pos, float neg, float total, float entropiaAtrib) {
	float ret = ((pos + neg) / (total)) * entropiaAtrib;

	if (isnan(ret)) {
		ret = 0;
	}
	return ret;
}
tPyN PyNTotal(tDatos& datos) {
	int p = 0, n = 0;
	for (int i = 0; i < datos.contador; i++) {
		if (datos.datos[i][NUM_ATRIBUTOS - 1] == "si") p++;
		else n++;
	}
	return { p,n };
}
float gain(float ent, float atrib) {
	return ent - atrib;
}