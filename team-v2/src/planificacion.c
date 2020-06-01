#include "planificacion.h"

float estimar_siguiente_rafaga(entrenador* entrenador){
	float alpha = 0.5;
	float estimacion = alpha * entrenador->cpu_disponible +
			(1-alpha) * entrenador->cpu_estimado_anterior;

	return estimacion;
}

void asignar_rafaga_estimada_al_entrenador(entrenador* entrenador){
	entrenador->cpu_estimado_anterior = estimar_siguiente_rafaga(entrenador);
	entrenador->cpu_estimado_restante = entrenador->cpu_estimado_anterior;
}

int tiene_menor_rafaga(entrenador* entrenador1,entrenador* entrenador2){
	return entrenador1->cpu_estimado_restante <= entrenador2->cpu_estimado_restante;
}

entrenador* entrenador_con_menor_rafaga_estimada(t_list* entrenadores_con_rafagas_estimadas){
	list_sort(entrenadores_con_rafagas_estimadas,tiene_menor_rafaga);
	return list_remove(entrenadores_con_rafagas_estimadas,0);
}


void planificar(){

	log_info(nuestro_log, "Estoy en el planificador");

	switch (leer_algoritmo_planificacion()) {
	    case FIFO:
	    	fifo();
	    	break;
	    case RR:
	    	round_robin();
	    	break;
	    case SJFSD:
	    	sjf_sin_desalojo();
	    	break;
	    case SJFCD:
	    	sjf_con_desalojo();
	    	break;
	    case ALGORITMO_DESCONOCIDO:
	    	log_error(nuestro_log, "El algoritmo ingresado no existe");
	    	break;
	}

}


//NO ESTOY TENIENDO EN CUENTA LAS ENTRADAS/SALIDAS, no se como manejarlas en codigo y como
//hacer toda esa parte

void fifo(){

	while(1){
		log_info(nuestro_log, "Estoy en FIFO preparado para planificar");
		if(list_size(entrenadores_ready) == 0) {
			pthread_mutex_lock(&lock_de_planificacion);
		}
		entrenador* entrenador_a_ejecutar = list_remove(entrenadores_ready, 0);

		while(cpu_restante_entrenador(entrenador_a_ejecutar) != 0){
			ejecutar(entrenador_a_ejecutar);
		}

	}
}

void round_robin(){

	int tiempo = 0;
	int quantum_consumido = 0;

	//QUANTUM = 2   (Del ejemplo del config)

	printf("\n QUANTUM TOTAL : %d", quantum);

	while(1){

		if(list_size(entrenadores_ready) == 0) {
			pthread_mutex_lock(&lock_de_planificacion);
		}

			entrenador* entrenador_a_ejecutar = list_remove(entrenadores_ready, 0);

			printf("\n CPU USADO ENTRENADOR : %d", entrenador_a_ejecutar->cpu_usado);
			//printf("\n CPU REQUERIDO ENTRENADOR : %d", entrenador_a_ejecutar->accion_a_ejecutar->cpu_requerido);
			printf("\nPOSICION ENTRENADOR : X->%d e Y->%d",entrenador_a_ejecutar->posicion->posicion_x, entrenador_a_ejecutar->posicion->posicion_y);

			for(int j = 0; j<list_size(entrenador_a_ejecutar->pokemons_adquiridos); j++){
				printf("\nPOKEMONS ENTRENADOR : %s", list_get(entrenador_a_ejecutar->pokemons_adquiridos, j));
			}

			while(cpu_restante_entrenador(entrenador_a_ejecutar) != 0 && quantum_consumido < quantum){
				ejecutar(entrenador_a_ejecutar);
				tiempo ++;
				quantum_consumido ++;
				printf("\n CPU USADO ENTRENADOR : %d", entrenador_a_ejecutar->cpu_usado);
				printf("\n CPU DISPONIBLE ENTRENADOR : %d", entrenador_a_ejecutar->cpu_disponible);
				printf("\n QUANTUM CONSUMIDO : %d", quantum_consumido);
				printf("\n CPU TIEMPO : %d", tiempo);
			}

			if(cpu_restante_entrenador(entrenador_a_ejecutar)){
				list_add(entrenadores_ready,entrenador_a_ejecutar);
			}

			quantum_consumido = 0;

	}

}

void sjf_sin_desalojo(){

	log_info(nuestro_log, "Estoy por entrar a SJF sin desalojo");

	t_list* entrenadores_con_rafagas_estimadas = list_create();

	while(1){

		log_info(nuestro_log, "Estoy en SJF SIN DESALOJO preparado para planificar");

		if(list_size(entrenadores_ready) == 0 && list_size(entrenadores_con_rafagas_estimadas) == 0) {
			pthread_mutex_lock(&lock_de_planificacion);
		}



		while(list_size(entrenadores_ready) > 0){
			entrenador* entrenador_a_estimar = list_remove(entrenadores_ready,0);
			asignar_rafaga_estimada_al_entrenador(entrenador_a_estimar);
			list_add(entrenadores_con_rafagas_estimadas,entrenador_a_estimar);
		}

		entrenador* entrenador_a_ejecutar = entrenador_con_menor_rafaga_estimada(entrenadores_con_rafagas_estimadas);

		printf("\n CPU USADO ENTRENADOR : %d", entrenador_a_ejecutar->cpu_usado);
		printf("\n CPU DISPONIBLE ENTRENADOR : %d", entrenador_a_ejecutar->cpu_disponible);
		printf("\nPOSICION ENTRENADOR : X->%d e Y->%d",entrenador_a_ejecutar->posicion->posicion_x, entrenador_a_ejecutar->posicion->posicion_y);

		for(int j = 0; j<list_size(entrenador_a_ejecutar->pokemons_adquiridos); j++){
			printf("\nPOKEMONS ENTRENADOR : %s", list_get(entrenador_a_ejecutar->pokemons_adquiridos, j));
		}

		while(cpu_restante_entrenador(entrenador_a_ejecutar) != 0){
			ejecutar(entrenador_a_ejecutar);
		}

	}

}



void sjf_con_desalojo(){

	t_list* entrenadores_con_rafagas_estimadas = list_create();

	while(1){

		if(list_size(entrenadores_ready) == 0 && list_size(entrenadores_con_rafagas_estimadas) == 0) {
			pthread_mutex_lock(&lock_de_planificacion);
		}

		while(list_size(entrenadores_ready) > 0){
			entrenador* entrenador_a_estimar = list_remove(entrenadores_ready,0);
			asignar_rafaga_estimada_al_entrenador(entrenador_a_estimar);
			list_add(entrenadores_con_rafagas_estimadas,entrenador_a_estimar);
		}

		entrenador* entrenador_a_ejecutar = entrenador_con_menor_rafaga_estimada(entrenadores_con_rafagas_estimadas);

		printf("\n CPU USADO ENTRENADOR : %d", entrenador_a_ejecutar->cpu_usado);
		printf("\n CPU DISPONIBLE ENTRENADOR : %d", entrenador_a_ejecutar->cpu_disponible);
		printf("\nPOSICION ENTRENADOR : X->%d e Y->%d",entrenador_a_ejecutar->posicion->posicion_x, entrenador_a_ejecutar->posicion->posicion_y);

		for(int j = 0; j<list_size(entrenador_a_ejecutar->pokemons_adquiridos); j++){
			printf("\nPOKEMONS ENTRENADOR : %s", list_get(entrenador_a_ejecutar->pokemons_adquiridos, j));
		}

		ejecutar(entrenador_a_ejecutar);
		entrenador_a_ejecutar->cpu_estimado_restante -= 1;


	}
}

