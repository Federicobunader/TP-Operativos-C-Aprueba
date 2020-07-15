#include "adm-memoria.h"

//PARTICIONES DINAMICAS

void inicializar_lista_particiones(){
	t_particion* primera_part = malloc(sizeof(t_particion));
	primera_part->inicio = 0;
	primera_part->fin = leer_tamano_memoria();
	primera_part->libre = true;
	lista_particiones = list_create();
	list_add(lista_particiones,primera_part);
}

void inicializar_contador_compactacion(){
	contador_compactacion = leer_frecuencia_compactacion();
}

int obtener_pos_particiones(int tamanio) {

	//pthread_mutex_lock(&mutex_memoria_principal);

	int posicion;
	int contador_compactacion = leer_frecuencia_compactacion();
	bool se_encontro_particion_libre = false;
	bool recien_se_compacto = false;

	while(se_encontro_particion_libre){
		posicion = buscar_particion_libre(tamanio);

		if( posicion >= 0 ){
			se_encontro_particion_libre = true;
			//nose si es mejor directamente retornar resultado aca
		}
		//el recien_Se_compacto lo agrego para que en el caso que la frecuencia de compactacion sea cero, no se me quede en un loop infinito y nunca libere particiones
		else if (contador_compactacion == 0 && !recien_se_compacto)
		{
				compactacion();
				contador_compactacion = leer_frecuencia_compactacion();
				recien_se_compacto = true;
		}
			else {
				liberar_particion();
				contador_compactacion--;
				recien_se_compacto = false;
			}

	}
	return posicion;
	//pthread_mutex_unlock(&mutex_memoria_principal);

}

void compactacion(){
	//todo definir compactacion

	// Basicamente resetea el valor del contador para que arranque de nuevo
	inicializar_contador_compactacion();
}

void liberar_particion(){

	switch(leer_algoritmo_reemplazo()){
		case FIFO:
			break;
		case LRU:
			break;
	}
	consolidar();
}

void consolidar();


int buscar_particion_libre(int tamanio){
	switch(leer_algoritmo_particion_libre()){
		case FF:
			return algoritmo_first_fit(tamanio);
			break;
		case BF:
			return algoritmo_best_fit(tamanio);
			break;
		}
}

int algoritmo_best_fit(int tamanio){
	t_particion* mejor_particion;
	for(int i = 0; list_size(lista_particiones) > i ; i++ ){
		t_particion* particion_actual = list_get(lista_particiones,i);

		if(mejor_particion != NULL){

		}
		else
			mejor_particion = list_get(lista_particiones,i);
	}
	return -1;
}

int algoritmo_first_fit(int tamanio){

	t_particion* particion;

	for(int i = 0;list_size(lista_particiones) > i; i++){
		particion = list_get(lista_particiones,i);

		if(esta_libre(particion)){
			/* Esto es porque hay un minimo en el tamanio de particiones */
			int diferencia = tamanio_particion(particion) - tamanio;

			if(diferencia >= 0){

				if(diferencia >= leer_tamano_minimo_particion())
				{
						return llenar_y_realizar_nueva_particion(particion,tamanio,i);
				}
				else
				{
						return llenar_particion(particion,tamanio);
				}

			}
		}
	}
	/* No se encontro espacio libre para el msj */
	return -1;
}


int llenar_y_realizar_nueva_particion(t_particion* particion,int tamanio,int posicion_en_lista){
	t_particion* nueva_particion = malloc(sizeof(t_particion));
	nueva_particion->fin = particion->fin;
	nueva_particion->inicio = nueva_particion->fin - tamanio + 1;

	//todo aunque tecnicamente no esta ocupado todavia no se me ocurre otro momento mejor para llenar este dato
	particion->tamanio_ocupado = tamanio;
	particion->fin = particion->inicio + tamanio - 1;
	particion->libre = false;

	list_add_in_index(lista_particiones,posicion_en_lista+1,nueva_particion);

	return particion->inicio;

}
/* A diferencia del de arriba esto es cuando el msj tiene el mismo tamanio que la particion o un poco menos*/
int llenar_particion(t_particion* particion, int tamanio){
	particion->tamanio_ocupado = tamanio;
	particion->libre = false;
	return particion->inicio;
}

bool esta_libre(t_particion* particion){
	return particion->libre;
}

int tamanio_particion(t_particion* particion){
	return particion->fin - particion->inicio +1;
}


//BUDDY SYSTEM

void inicializar_lista_bs(){
	int tamanio_memoria = leer_tamano_memoria();

	arbol_bs = malloc(sizeof(t_bs_nodo));
	t_particion_bs* primera_part = malloc(sizeof(t_particion_bs));
	primera_part->inicio = 0;
	primera_part->fin = tamanio_memoria-1; //TODO ver si usar el fin
	primera_part->potencia_de_dos=obtener_potencia_de_dos_mas_cercana(tamanio_memoria);
	primera_part->libre = true;

	arbol_bs->particion = primera_part;
	arbol_bs->izq = NULL;
	arbol_bs->der = NULL;
}

int obtener_posicion_bs(int tamanio) {

	int potencia_de_dos_mas_cercana = obtener_potencia_de_dos_mas_cercana(tamanio);

	pthread_mutex_lock(&mutex_memoria_principal);

	t_bs_nodo* nodo_mas_cercano = obtener_nodo_mas_cercano(arbol_bs, potencia_de_dos_mas_cercana);
	if(nodo_mas_cercano == NULL) {
		//TODO IMPLEMENTAR ALGORITMO PARA LA SUSTICION DE ALGUNA PARTICION
	} else {
		if(nodo_mas_cercano->particion->potencia_de_dos == potencia_de_dos_mas_cercana) {
			//TODO MARCAR PARTICION COMO OCUPADA
		} else {
			//TODO PARTICIONAR ARBOL HASTA LA potencia_de_dos_mas_cercana Y DEVOLVER NODO
		}

	}

	pthread_mutex_unlock(&mutex_memoria_principal);
	return -1;
}

int obtener_potencia_de_dos_mas_cercana(int valor) {
	double aux = valor;
	int cont = 0;
	if(valor < 2) {
		return 1;
	} else {
		while(aux > 1) {
			aux /= 2;
			cont ++;
		}
		return cont;
	}
}

t_bs_nodo* obtener_nodo_mas_cercano(t_bs_nodo* nodo_a_evaluar, int potencia_de_dos) {

	if(nodo_a_evaluar->particion->libre) {
		if(nodo_a_evaluar->particion->potencia_de_dos >= potencia_de_dos) {
			return nodo_a_evaluar;
		} else {
			return NULL;
		}
	} else {
		t_bs_nodo* nodo_izq = obtener_nodo_mas_cercano(nodo_a_evaluar, potencia_de_dos);
		t_bs_nodo* nodo_der = obtener_nodo_mas_cercano(nodo_a_evaluar, potencia_de_dos);

		if(nodo_izq == NULL) {
			if(nodo_der == NULL) {
				return NULL;
			} else {
				return nodo_der;
			}
		} else {
			return nodo_izq;
		}
	}

}
