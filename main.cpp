#include <iostream>
#include <string.h>
#include <vector>
#include <queue>

using namespace std;

const int nNodos = 7; //Es el numero de nodos del grafo
const double pesoInf = 99; //Peso muy grande que no pueden tomar las aristas y con el que se inicializan los nodos para recorrido de dijkstra

struct Nodo { //Define una estructuraque representará a cada nodo del grafo, en ella guardaremos el nombre del nodo, el tipo de nodo, el peso o distancia total y si ha sido visitado
	int id;
	char nombre; // nombre del nodo
	char tipo[15]; // tipo: fuente, pozo o transición
	double peso_total; //peso desde el origen hasta el nodo actual
	bool visitado; //indica si el nodo ya fue visitado en dijkstra
};

struct Arco{ // Define una estructura que representará las transiciones/arcos/caminos/enlaces del grafo, en ella guardaremos el peso del arco, si este existe y los nodos de origen y destino
	Nodo origen; //nodo de origen
	Nodo adyacente; //nodo de destino o adyacente
	double peso; //peso del arco en caso de existir, en este caso es la probailidad de operatividad del enlace, se escoje double y no float para la precision de 2 decimales
	bool existe; //esta variable nos permitirá saber si existe un enlace o camino entre dos nodos
};

struct comparadorMinimo { //Comparador que indica si el peso del primer enlace en menor al segundo, necesario para la cola de prioridad que se implementará para dijkstra
    bool operator() ( const Nodo a , const Nodo b ) {
        return a.peso_total > b.peso_total;
    }
};

void definirNodos(Nodo nodos[nNodos]); // Llena el vector de nodos del grafo
void imprimirNodos(Nodo nodos[nNodos]); // imprime el vector de nodos
void creaMatriz(Nodo nodos[nNodos], Arco matriz[nNodos][nNodos]); // llena la matriz de transiciones/arcos/enlaces, matriz asociada del grafo
void obtenerMatriz(Arco matriz[nNodos][nNodos]); // Obtiene matríz con pesos correctos para dijkstra, mejor explicación en la función
void imprimirMatriz(Nodo nodos[nNodos], Arco matriz[nNodos][nNodos], char *cadena); //imprime la matriz asociada al grafo
void imprimirAdyacentes(Arco matriz[nNodos][nNodos]); //imprime la matriz asociada al grafo
Nodo actualizarVertice(Nodo actual , Nodo adyacente , double peso);
void imprimirCamino(Arco matrizAsoc[nNodos][nNodos], Nodo nodos[nNodos]);
void dijkstra(Nodo nodos[nNodos], Arco matrizAsoc[nNodos][nNodos]);

int main(int argc, char** argv) {
	Nodo nodos[nNodos]; //define el vector de nodos del grafo (vector de nNodos [7] posiciones) de tipo Nodo, es decir, en cada posición del vector tendremos un Nodo
	Arco matrizAsociada[nNodos][nNodos];  //define la matriz asociada al grafo (matriz de dimensión nNodos*nNodos [7*7]) de tipo Arco, es decir, en cada posición de la matriz tendremos un Arco
	char titulo[50]; //titulo a imprimir en el metodo imprimir de la matrizAsociada
	
	definirNodos(nodos); //llena los nodos
	imprimirNodos(nodos); //imprime el vector de nodos (nombre -> tipo)	
	creaMatriz(nodos, matrizAsociada); // llena la matriz asociada tomando en cuenta las transiciones definidas
	strcpy(titulo, " ** Matriz Asociada al Grafo ** ");
	imprimirMatriz(nodos, matrizAsociada, titulo); // imprime la matriz asociada
	obtenerMatriz(matrizAsociada); // llena la matriz asociada tomando en cuenta las transiciones definidas
	strcpy(titulo, " ** Matriz Asociada al Grafo - Pesos invertidos para dijkstra** ");
	imprimirMatriz(nodos, matrizAsociada, titulo); // imprime la matriz asociada
	imprimirAdyacentes(matrizAsociada);
	
	dijkstra(nodos, matrizAsociada);
	cout << endl;
	system("pause");
}

void definirNodos(Nodo nodos[nNodos]){
	nodos[0].nombre = 'T';
	nodos[1].nombre = 'A';
	nodos[2].nombre = 'B';
	nodos[3].nombre = 'C';
	nodos[4].nombre = 'D';
	nodos[5].nombre = 'E';
	nodos[6].nombre = 'R';	
	
	for (int i = 0; i < nNodos; i ++){ //recorre el vector de nodos, y asigna el tipo que corresponda, viendo el grafo podemos notar que:
		nodos[i].id = i;
		nodos[i].peso_total = pesoInf; //inicializamos todos los nodos con el peso infinito definido anteriormente
		nodos[i].visitado = false; //inicializamos todos los nodos como no visitados
		
		if(nodos[i].nombre == 'T'){
			nodos[i].peso_total = 0; //inicializamos el peso inicial del origen o fuente en 0
			strcpy(nodos[i].tipo, "fuente"); // T es el nodo fuente, de él salen arcos/enlaces pero no llega ninguno a él
		}else if(nodos[i].nombre == 'R'){
			strcpy(nodos[i].tipo, "pozo"); // R es el nodo pozo, a él llegan arcos/enlaces pero de él no sale ninguno
		}else{
			strcpy(nodos[i].tipo, "transicion"); // A, B, C, D, E son nodos normales que permiten las transiciones del grafo
		}
	}
}//define los nodos del grafo

void imprimirNodos(Nodo nodos[nNodos]){
	cout << " ** Nodos ** " << endl;
	for (int i = 0; i < nNodos; i ++){ //recorre el vector de nodos e imprime el nombre y tipo de cada uno
		cout << nodos[i].id << " -> " << nodos[i].nombre << " -> " << nodos[i].tipo << endl;
	}
}//define los nodos del grafo

void creaMatriz(Nodo nodos[nNodos], Arco matriz[nNodos][nNodos]){
	// Como se va a llenar una matriz, es necesario recorrerla en sus dos dimensiones: filas (i) y columnas (j), ambas definidas por el vector de nodos del grafo
	for( int i = 0; i < nNodos; i++){ 
		for( int j = 0; j < nNodos; j++){
			if(nodos[i].nombre == 'A'){ // Revisa las transiciones que salen del nodo A
				matriz[i][j].origen = nodos[i]; //asigna como nodo de origen el nodo actual A
				if(nodos[j].nombre == 'C'){ // Asigna el peso o probabilidad de operatividad del enlace o arco A -> C
					matriz[i][j].adyacente = nodos[j]; // asigna el nodo adyancente
					matriz[i][j].peso = 0.95;
					matriz[i][j].existe = true;
				}else if(nodos[j].nombre == 'B'){ // Asigna el peso o probabilidad de operatividad del enlace o arco A -> B
					matriz[i][j].adyacente = nodos[j]; // asigna el nodo adyancente
					matriz[i][j].peso = 0.99;
					matriz[i][j].existe = true;
				}else{ // al resto de los posibles arcos le especifica que ellos no existen
					matriz[i][j].existe = false;
				}
			}// nodos de A: A -> B, A -> C
			if(nodos[i].nombre == 'B'){ // Revisa las transiciones que salen del nodo 
				matriz[i][j].origen = nodos[i]; //asigna como nodo de origen el nodo actual B
				if(nodos[j].nombre == 'A'){ // Asigna el peso o probabilidad de operatividad del enlace o arco B -> A
					matriz[i][j].adyacente = nodos[j]; // asigna el nodo adyancente
					matriz[i][j].peso = 0.99;
					matriz[i][j].existe = true;
				}else if(nodos[j].nombre == 'C'){ // Asigna el peso o probabilidad de operatividad del enlace o arco B -> C
					matriz[i][j].adyacente = nodos[j]; // asigna el nodo adyancente
					matriz[i][j].peso = 0.98;
					matriz[i][j].existe = true;
				}else if(nodos[j].nombre == 'D'){ // Asigna el peso o probabilidad de operatividad del enlace o arco B -> D
					matriz[i][j].adyacente = nodos[j]; // asigna el nodo adyancente
					matriz[i][j].peso = 0.90;
					matriz[i][j].existe = true;
				}else{ // al resto de los posibles arcos le especifica que ellos no existen
					matriz[i][j].existe = false;
				}
			}// nodos de B: B -> A, B -> C, B -> D
			if(nodos[i].nombre == 'C'){ // Revisa las transiciones que salen del nodo C
				matriz[i][j].origen = nodos[i]; //asigna como nodo de origen el nodo actual C
				if(nodos[j].nombre == 'D'){ // Asigna el peso o probabilidad de operatividad del enlace o arco C -> D
					matriz[i][j].adyacente = nodos[j]; // asigna el nodo adyancente
					matriz[i][j].peso = 0.99;
					matriz[i][j].existe = true;
				}else if(nodos[j].nombre == 'E'){ // Asigna el peso o probabilidad de operatividad del enlace o arco C -> E
					matriz[i][j].adyacente = nodos[j]; // asigna el nodo adyancente
					matriz[i][j].peso = 0.98;
					matriz[i][j].existe = true;
				}else if(nodos[j].nombre == 'R'){ // Asigna el peso o probabilidad de operatividad del enlace o arco C -> R
					matriz[i][j].adyacente = nodos[j]; // asigna el nodo adyancente
					matriz[i][j].peso = 0.94;
					matriz[i][j].existe = true;
				}else{ // al resto de los posibles arcos le especifica que ellos no existen
					matriz[i][j].existe = false;
				}
			}// nodos de C: C -> D, C -> E, C -> R
			if(nodos[i].nombre == 'D'){ // Revisa las transiciones que salen del nodo D
				matriz[i][j].origen = nodos[i]; //asigna como nodo de origen el nodo actual D
				if(nodos[j].nombre == 'C'){ // Asigna el peso o probabilidad de operatividad del enlace o arco D -> C
					matriz[i][j].adyacente = nodos[j]; // asigna el nodo adyancente
					matriz[i][j].peso = 0.99;
					matriz[i][j].existe = true;
				}else if(nodos[j].nombre == 'E'){ // Asigna el peso o probabilidad de operatividad del enlace o arco D -> E
					matriz[i][j].adyacente = nodos[j]; // asigna el nodo adyancente
					matriz[i][j].peso = 0.97;
					matriz[i][j].existe = true;
				}else{ // al resto de los posibles arcos le especifica que ellos no existen
					matriz[i][j].existe = false;
				}
			}// nodos de D: D -> C, D -> E
			if(nodos[i].nombre == 'E'){ // Revisa las transiciones que salen del nodo E
				matriz[i][j].origen = nodos[i]; //asigna como nodo de origen el nodo actual E
				if(nodos[j].nombre == 'R'){ // Asigna el peso o probabilidad de operatividad del enlace o arco E -> R
					matriz[i][j].adyacente = nodos[j]; // asigna el nodo adyancente
					matriz[i][j].peso = 0.97;
					matriz[i][j].existe = true;
				}else{ // al resto de los posibles arcos le especifica que ellos no existen
					matriz[i][j].existe = false;
				}
			}// nodos de E: E -> R 
			if(nodos[i].nombre == 'T'){ // Revisa las transiciones que salen del nodo T
				matriz[i][j].origen = nodos[i]; //asigna como nodo de origen el nodo actual T
				if(nodos[j].nombre == 'A'){ // Asigna el peso o probabilidad de operatividad del enlace o arco T -> A
					matriz[i][j].adyacente = nodos[j]; // asigna el nodo adyancente
					matriz[i][j].peso = 0.99;
					matriz[i][j].existe = true;
				}else if(nodos[j].nombre == 'B'){ // Asigna el peso o probabilidad de operatividad del enlace o arco T -> B
					matriz[i][j].adyacente = nodos[j]; // asigna el nodo adyancente
					matriz[i][j].peso = 0.97;
					matriz[i][j].existe = true;
				}else{ // al resto de los posibles arcos le especifica que ellos no existen
					matriz[i][j].existe = false;
				}
			}// nodos de T: T -> A, T -> B 
			if(nodos[i].nombre == 'R'){ // Revisa las transiciones que salen del nodo B 
				matriz[i][j].existe = false; // Desde R no nace ningún arco o enlace, por lo tanto se especifica que ninguno existe
			}// nodos de R: Ninguno, R es un pozo 
		}//for j
	}//for i
}// metodo crear matriz

void obtenerMatriz(Arco matriz[nNodos][nNodos]){ 
/*
Dijkstra es un algoritmo que permite hallar el camino o ruta minima entre dos nodos, sin embargo, en el enunciado se nos pide hallar la ruta de maxima fiabilidad,
dado que los pesos de los enlaces es la probabilidad de que el enlace esté operativo, no sería posible aplicar dijkstra de esta manera, porque estaríamos hallando
una ruta máxima o de mayor peso, es por esto que se invertirán los pesos de los enlaces, es decir, conociendo que la probabilidad total es 1, y teniendo la probabilidad de
que el enlace esté operativo, podemos obtener la probabilidad de que el enlace falle: 1 - peso_enlace, lo que nos permite aplicar dijsktra para hallar el camino mínimo, 
es decir, el camino con menos probabilidad de falla, que es lo mismo, al camino de mayor fiabilidad
*/
	for(int i = 0; i < nNodos; i++){
		for(int j = 0; j < nNodos; j++){
			if(matriz[i][j].existe == true){
				matriz[i][j].peso = 1 - matriz[i][j].peso;
			}
		}
	}
}

void imprimirMatriz(Nodo nodos[nNodos], Arco matriz[nNodos][nNodos], char *cadena){
	cout << endl << cadena << endl << endl;
	cout << "      T    A    B    C    D    E    R" << endl;
	for( int i = 0; i < nNodos; i++){ //Recorre la matriz asociada al grafo e imprime el peso de cada enlace/arista o arco siempre y cuando el mismo exista
		cout << nodos[i].nombre << " | ";
		for( int j = 0; j < nNodos; j++){
			if(matriz[i][j].existe == true){
				cout << matriz[i][j].peso << " ";
			}else{
				cout << "     ";
			}
		}//for j
		cout << endl;
	}//for i
}

void imprimirAdyacentes(Arco matriz[nNodos][nNodos]){	
	cout << endl << " ** Adyacentes **" << endl << endl; //Imprime el listado de nodos o vertices adyacentes y el peso del enlace/arista entre los mismos
	for( int i = 0; i < nNodos; i++){ 
		for( int j = 0; j < nNodos; j++){
			if(matriz[i][j].existe == true){
				cout << matriz[i][j].origen.nombre << " " << matriz[i][j].adyacente.nombre << " " << matriz[i][j].peso << endl;
			}
		}//for j
		cout << endl;
	}//for i
}

Nodo actualizarVertice(Nodo actual , Nodo adyacente , double peso){ //actualiza el peso minimo del vertice y retorna la información del mismo
    if( actual.peso_total + peso < adyacente.peso_total ){ //Si el peso del vertice actual + peso del enlace o arista es menor al peso del nodo adyacente, actualizamos el peso del nodo adyacente como nuevo peso minimo
        adyacente.peso_total = actual.peso_total + peso;  //actualizamos el peso del vertice adyacente con el peso minimo nuevo
    }
    return adyacente;
}

void imprimirCamino(Arco matrizAsoc[nNodos][nNodos], Nodo nodos[nNodos]){
	Nodo final = nodos[nNodos-1]; //guarda el nodo final R
	Nodo anterior = final; // auxiliar para guardar el nodo anterior en la ruta final
	Nodo ruta[nNodos]; //vector de nodos que indicaran ruta final
	double pesoMin = final.peso_total; // auxiliar del peso minimo total para el calculo de la ruta minima
	double resta; 
	int totalNodos = 0; //total de nodos en la ruta final
	
	cout << endl << "1.- Peso minimo (Probabilidad minima de falla en el sistema): " << pesoMin ;
	for(int i = nNodos - 1; i > -1; i--){ 
		for(int j = nNodos - 1; j > -1; j--){ //Recorro la matriz de adyacencia en orden inverso (Para recorrer los nodos desde el final al origen)
			if(matrizAsoc[i][j].existe == true){ //si el enlace/arco existe, entonces resto el peso minimo total - el peso del enlace - el peso minino del nodo anterior
				resta = pesoMin - matrizAsoc[i][j].peso - nodos[matrizAsoc[i][j].origen.id].peso_total;
				if(resta == 0 && matrizAsoc[i][j].adyacente.nombre == anterior.nombre){ //si la resta es 0 y el nodo del que proviene el enlace es el mismo a mi nodo anterior, entonces agrego el mismo a la ruta final
					pesoMin = pesoMin - matrizAsoc[i][j].peso; //recalculo el peso minimo (que debe ser igual al peso minimo del nodo actual
					anterior = matrizAsoc[i][j].origen; //actualizo el nodo anterior, que es el nodo actual
					ruta[totalNodos] = matrizAsoc[i][j].adyacente; //agrego a la ruta final el nodo anterior
					totalNodos++; //aumento la cantidad de nodos finales
				}
			}
		}
	}
	
	cout << endl << "2.- Ruta Minima de fallo (Ruta de maxima fiabilidad): " << nodos[0].nombre; //imprimo el nodo Fuente T
	for(int i = totalNodos - 1; i > -1; i--){
		cout << " -> "<< ruta[i].nombre; //imprimo el resto de la ruta
	}
}

void dijkstra(Nodo nodos[nNodos], Arco matrizAsoc[nNodos][nNodos]){
	priority_queue< Nodo , vector<Nodo> , comparadorMinimo > cola_prioridad;  //cola de prioridad propia de c++, usamos el comparador minimo que definimos para que el de menor valor este en el tope
    Nodo actual; //variable para almacenar nodo actual 
    
	//nodos[0].peso_total = 0; //no inicializo aqui, porque ya lo hice en la definicion de nodos 
    cola_prioridad.push(nodos[0]); // agregamos el nodo inicial a la cola de prioridad
    
    while( !cola_prioridad.empty() ){ //vamos a iterar mientras la cola de prioridad no esté vacía
        actual = cola_prioridad.top(); //guardamos el nodo de menor peso de la cola
        cola_prioridad.pop(); //elminamos el nodo de la cola, para no regresar a él
        
        if( nodos[actual.id].visitado == true ){ //Verificamos si el nodo actual ya fue visitado, si es asi continuo al siguiente elemento de la cola
			continue; 
		}
        nodos[actual.id].visitado = true; //Si el nodo actual no ha sifo visitado, lo marco como visitado

		for(int i = 0; i < nNodos; i++){ //Recorro la matrpiz de adyacencia
			for(int j = 0; j < nNodos; j++){
				if(matrizAsoc[i][j].existe == true && matrizAsoc[i][j].origen.nombre == actual.nombre){ //si el enlace/arista existe y el nodo de origen del mismo es igual al nodo actual
					if(nodos[matrizAsoc[i][j].adyacente.id].visitado == false){  //si el vertice adyacente no ha sido visitado
						matrizAsoc[i][j].adyacente = actualizarVertice(actual, matrizAsoc[i][j].adyacente, matrizAsoc[i][j].peso); //intentamos actualizar el peso minimo del nodo adyacente
						cola_prioridad.push(matrizAsoc[i][j].adyacente); //agregamos adyacente a la cola de prioridad
						nodos[actual.id].peso_total = actual.peso_total; //Actualizo en el vector de nodos el peso minimo de mi nodo actual
						if( strcmp(matrizAsoc[i][j].adyacente.tipo, "pozo") == 0){ //en caso de que un adyacente sea el nodo final o pozo, Actualizamos en el vector de nodos el peso minimo de mi nodo final pero con la información del adyacente (porque el nodo pozo nunca va a ser el nodo actual por no poseer arista/enlaces)
							nodos[matrizAsoc[i][j].adyacente.id].peso_total = matrizAsoc[i][j].adyacente.peso_total;
						}
					}
				}
			}
		}
    }

    cout << " ** Pesos minimos desde el Origen T hasta cada nodo ** " << endl << endl; 
    for( int i = 0 ; i < nNodos ; i++ ){ //Imprimo los pesos minimos desde el origen hasta cada nodo del grafo
        cout <<  "Vertice: " << nodos[i].nombre << " -> peso minimo: "  << nodos[i].peso_total << endl;
    }

    cout << endl <<  " ** Resultado Final ** " << endl; //Imprimo peso minimo entre nodos T y R, ruta o camino final (camino de maxima fiabilidad)
    imprimirCamino(matrizAsoc, nodos);
    cout << endl;
}
