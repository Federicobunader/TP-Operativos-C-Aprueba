#include "team-v2.h"


int main(void){

	iniciar_team();

	log_info(nuestro_log, "La cantidad de entrenadores del equipo es de %d entrenadores", list_size(entrenadores));
	dictionary_iterator(objetivo_global, mostrar_objetivo_global);

	pthread_t* hilo_conexion_gameboy;
	pthread_create(&hilo_conexion_gameboy, NULL, atender_conexion_gameboy, NULL);
	pthread_detach(hilo_conexion_gameboy);

	pthread_t* hilo_reintento_conexion_broker;
	pthread_create(&hilo_reintento_conexion_broker, NULL, levantar_conexiones_al_broker, NULL);
	pthread_detach(hilo_reintento_conexion_broker);

	get_pokemon();

	pthread_t* hilo_entrenador_disponible;
	pthread_create(&hilo_entrenador_disponible, NULL, buscar_entrenador_disponible, NULL);
	pthread_detach(hilo_entrenador_disponible);

	pthread_t* hilo_planificacion;
	pthread_create(&hilo_planificacion, NULL, planificar, NULL);
	pthread_join(hilo_planificacion, NULL);

	return 0;
}

void mostrar_objetivo_global(char* key, void* value) {
	log_info(nuestro_log, "Necesito %d pokemons %s", value, key);
}

void iniciar_team() {
	iniciar_config();
	logger = log_create(leer_log_file(), "team", false, LOG_LEVEL_INFO);
	nuestro_log = log_create(leer_nuestro_log_file(), "team", true, LOG_LEVEL_INFO);
	//Iniciamos las variables globales del constructor
	iniciar_variables_globales();
}
