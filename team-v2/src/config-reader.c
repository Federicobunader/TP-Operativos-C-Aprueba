#include "config-reader.h"

void iniciar_config(void){
	config = config_create("src/resources/team.config");
	setear_tiempo_reconexion();
	setear_retardo_ciclo_cpu();
	setear_algoritmo_planificacion();
	setear_quantum();
	setear_estimacion_inicial();
	setear_ip_broker();
	setear_puerto_broker();
	setear_log_file();
}

void destruir_config(void) {
	config_destroy(config);
}

//Recibe lista como char** y la transforma a t_list con elementos char*
t_list* crear_t_list(char** array) {
	t_list* lista = list_create();
	int pos = 0;
	while(array[pos] != NULL) {
		list_add(lista, array[pos]);
		pos++;
	}
	return lista;
}

t_list* leer_posiciones_entrenadores(void){

	char** posiciones_config = config_get_array_value(config, "POSICIONES_ENTRENADORES");

	return crear_t_list(posiciones_config);
}

t_list* leer_pokemon_entrenadores(void){
	char** pokemons_config = config_get_array_value(config, "POKEMON_ENTRENADORES");

	return crear_t_list(pokemons_config);
}

t_list* leer_objetivos_entrenadores(void){
	char** objetivos_config = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");

	return crear_t_list(objetivos_config);
}

int leer_tiempo_reconexion(void){
	return tiempo_reconexion;
}

int leer_retardo_ciclo_cpu(void){
	return retardo_ciclo_cpu;
}

enum_algoritmo_planificacion leer_algoritmo_planificacion(void){
	return algoritmo_planificacion;
}

int leer_quantum(void){
	return quantum;
}

int leer_estimacion_inicial(void){
	return estimacion_inicial;
}

char* leer_ip_broker(void){
	return ip_broker;
}

char* leer_puerto_broker(void){
	return puerto_broker;
}

char* leer_log_file(void){
	return log_file;
}

//SOLO SE EJECUTAN UNA VEZ
//
void setear_tiempo_reconexion(void){
	tiempo_reconexion = config_get_int_value(config, "TIEMPO_RECONEXION");
}

void setear_retardo_ciclo_cpu(void){
	retardo_ciclo_cpu = config_get_int_value(config, "RETARDO_CICLO_CPU");
}

void setear_algoritmo_planificacion(void){
	char* algoritmo_del_config = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
	if(string_equals_ignore_case(algoritmo_del_config, "FIFO")) {
		algoritmo_planificacion = FIFO;
	}
	else if(string_equals_ignore_case(algoritmo_del_config, "RR")) {
		algoritmo_planificacion = RR;
	}
	else if(string_equals_ignore_case(algoritmo_del_config, "SJF-CD")) {
		algoritmo_planificacion = SJFCD;
	}
	else if(string_equals_ignore_case(algoritmo_del_config, "SJF-SD")) {
		algoritmo_planificacion = SJFSD;
	}
	else {
		algoritmo_planificacion = ALGORITMO_DESCONOCIDO;
	}
}

void setear_quantum(void){
	quantum = config_get_int_value(config, "QUANTUM");
}

void setear_estimacion_inicial(void){
	estimacion_inicial = config_get_int_value(config, "ESTIMACION_INICIAL");
}

void setear_ip_broker(void){
	ip_broker = config_get_string_value(config, "IP_BROKER");
}

void setear_puerto_broker(void){
	puerto_broker = config_get_string_value(config, "PUERTO_BROKER");
}

void setear_log_file(void){
	log_file = config_get_string_value(config, "LOG_FILE");
}
