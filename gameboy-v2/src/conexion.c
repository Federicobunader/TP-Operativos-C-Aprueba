
#include "conexion.h"


int crear_conexion_del_cliente(char *ip, char* puerto, t_log* logger) {
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1){
		log_error(logger, string_from_format("El cliente fallo al conectarse en IP: %s y PUERTO: %s", ip, puerto));
		log_error(mi_log, string_from_format("El cliente fallo al conectarse en IP: %s y PUERTO: %s", ip, puerto));
	}else{
		log_info(logger, string_from_format("El cliente se conecto exitosamente en IP: %s y PUERTO: %s", ip, puerto));
		log_info(mi_log, string_from_format("El cliente se conecto exitosamente en IP: %s y PUERTO: %s", ip, puerto));
	}
	freeaddrinfo(server_info);
	return socket_cliente;
}

void liberar_conexion(int socket) {
	close(socket);
}

void enviar_mensaje_appeared(t_appeared_pokemon appeared_pokemon, int socket, int puerto, int id_mensaje_correlativo){
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->modulo = GAMEBOY;
	paquete->cod_op = MENSAJE;
	paquete->mensaje = APPEARED_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(appeared_pokemon.nombre_pokemon) + 1 + sizeof(uint32_t) * 4;
	void* stream = malloc(paquete->buffer->size);
	int bytes_escritos = 0;

	memcpy(stream + bytes_escritos, &appeared_pokemon.largo_nombre_pokemon, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, appeared_pokemon.nombre_pokemon, strlen(appeared_pokemon.nombre_pokemon)+1);
	bytes_escritos += strlen(appeared_pokemon.nombre_pokemon)+1;

	memcpy(stream + bytes_escritos, &appeared_pokemon.posicionX, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, &appeared_pokemon.posicionY, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);
	int cero = 0;
	if(puerto == 4444){
		memcpy(stream + bytes_escritos, &id_mensaje_correlativo, sizeof(uint32_t));
	}else{
		memcpy(stream + bytes_escritos, &cero, sizeof(uint32_t));
	}


	paquete->buffer->stream = stream;
	int size_serializados = paquete->buffer->size + 4*sizeof(int);

	void *mensaje_a_enviar = serializar_paquete(paquete,size_serializados);
	log_info(mi_log,string_from_format("sizes: %d",size_serializados));

	if(send(socket,mensaje_a_enviar,size_serializados,0)>0){
		log_info(logger, string_from_format("Se envio el mensaje APPEARED: %s y PUERTO: %d",leer_ip_broker(), leer_puerto_broker()));
		log_info(mi_log, string_from_format("Se envio el mensaje APPEARED: %s y PUERTO: %d",leer_ip_broker(), leer_puerto_broker()));
	}else{
		log_error(logger, "No se pudo enviar el mensaje APPEARED.");
		log_error(mi_log, "No se pudo enviar el mensaje APPEARED.");
	}

	free(mensaje_a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviar_mensaje_new(t_new_pokemon new_pokemon, int socket,int puerto, int id_mensaje){
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->modulo = GAMEBOY;
	paquete->cod_op = MENSAJE;
	paquete->mensaje = NEW_POKEMON;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = new_pokemon.largo_nombre_pokemon + 1 + sizeof(uint32_t) * 5;

	void* stream = malloc(paquete->buffer->size);
	int bytes_escritos = 0;

	memcpy(stream + bytes_escritos, &new_pokemon.largo_nombre_pokemon, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, new_pokemon.nombre_pokemon, strlen(new_pokemon.nombre_pokemon)+1);
	bytes_escritos += strlen(new_pokemon.nombre_pokemon)+1;

	memcpy(stream + bytes_escritos, &new_pokemon.posicionX, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, &new_pokemon.posicionY, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, &new_pokemon.cantidad_pokemon, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	int cero = 0;
	if(puerto == 5001){
		memcpy(stream + bytes_escritos, &id_mensaje, sizeof(uint32_t));
	}else{
		memcpy(stream + bytes_escritos, &cero, sizeof(uint32_t));
	}

	paquete->buffer->stream = stream;
	int size_serializados = paquete->buffer->size + 4*sizeof(int);

	void *mensaje_a_enviar = serializar_paquete(paquete,size_serializados);
	log_info(mi_log,string_from_format("sizes: %d",size_serializados));

	if(send(socket,mensaje_a_enviar,size_serializados,0)>0){
		log_info(logger, string_from_format("Se envio el mensaje NEW: %s y PUERTO: %d",leer_ip_broker(), leer_puerto_broker));
	}else{
		log_error(logger, "No se pudo enviar el mensaje NEW.");
	}

	free(mensaje_a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

}

void enviar_mensaje_catch(t_catch_pokemon catch_pokemon, int socket,int puerto, int id_mensaje){
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->modulo = GAMEBOY;
	paquete->cod_op = MENSAJE;
	paquete->mensaje = CATCH_POKEMON;


	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(catch_pokemon.nombre_pokemon) + 1 + sizeof(uint32_t) * 3;

	void* stream = malloc(paquete->buffer->size);
	int bytes_escritos = 0;

	memcpy(stream + bytes_escritos, &catch_pokemon.largo_nombre_pokemon, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, catch_pokemon.nombre_pokemon, strlen(catch_pokemon.nombre_pokemon)+1);
	bytes_escritos += strlen(catch_pokemon.nombre_pokemon)+1;

	memcpy(stream + bytes_escritos, &catch_pokemon.posicionX, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, &catch_pokemon.posicionY, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	if(puerto == 5001){
			memcpy(stream + bytes_escritos, &id_mensaje, sizeof(uint32_t));
		}

	paquete->buffer->stream = stream;
	int size_serializados = paquete->buffer->size + 4*sizeof(int);

	void *mensaje_a_enviar = serializar_paquete(paquete,size_serializados);
	log_info(mi_log,string_from_format("sizes: %d",size_serializados));

	if(send(socket,mensaje_a_enviar,size_serializados,0)>0){
		log_info(logger, string_from_format("Se envio el mensaje CATCH: %s y PUERTO: %d",leer_ip_broker(), leer_puerto_broker));
	}else{
		log_error(logger, "No se pudo enviar el mensaje CATCH.");
	}


	free(mensaje_a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

}

void suscribirse_a_cola(t_mq cola_de_mensajes, int tiempo, int socket_broker){
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->modulo = GAMEBOY;
	paquete->cod_op = SUSCRIPCION;
	paquete->mensaje = cola_de_mensajes.nombre;

	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = sizeof(mq_nombre) + sizeof(uint32_t);

	void* stream = malloc(paquete->buffer->size);
	int bytes_escritos = 0;

	memcpy(stream + bytes_escritos, tiempo,sizeof(uint32_t));

	paquete->buffer->stream = stream;
	int size_serializados = paquete->buffer->size + 4*sizeof(int);

	void *mensaje_a_enviar = serializar_paquete(paquete,size_serializados);
	log_info(mi_log,string_from_format("sizes: %d",size_serializados));

	if(send(socket_broker,mensaje_a_enviar,size_serializados,0)>0){
		log_info(logger, string_from_format("Se suscribio correctamente a la cola %s", &cola_de_mensajes.nombre));
	}else{
		log_error(logger, string_from_format("No se pudo suscribir a la cola %s",&cola_de_mensajes.nombre));
	}


	free(mensaje_a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviar_mensaje_caught(t_caught_pokemon caught_pokemon, int socket_broker,int id_mensaje_correlativo){
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->modulo = GAMEBOY;
	paquete->cod_op = MENSAJE;
	paquete->mensaje = CAUGHT_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = sizeof(uint32_t) + sizeof(int);

	void* stream = malloc(paquete->buffer->size);
	int bytes_escritos = 0;

	memcpy(stream + bytes_escritos, &id_mensaje_correlativo, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	memcpy(stream + bytes_escritos, &caught_pokemon.atrapado, sizeof(uint32_t));

	paquete->buffer->stream = stream;
	int size_serializados = paquete->buffer->size + 4*sizeof(int);

	void *mensaje_a_enviar = serializar_paquete(paquete,size_serializados);
	log_info(mi_log,string_from_format("sizes: %d",size_serializados));

	if(send(socket_broker,mensaje_a_enviar,size_serializados,0)>0){
		log_info(logger, string_from_format("Se envio el mensaje CAUGHT: %s y PUERTO: %s",leer_ip_broker(), leer_puerto_broker));
	}else{
		log_error(logger, "No se pudo enviar el mensaje CAUGHT.");
	}


	free(mensaje_a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

}

void enviar_mensaje_get(t_get_pokemon get_pokemon, int socket,int puerto, int id_mensaje){
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->modulo = GAMEBOY;
	paquete->cod_op = MENSAJE;
	paquete->mensaje = GET_POKEMON;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = get_pokemon.largo_nombre_pokemon + 1 + sizeof(uint32_t);

	log_info(mi_log,string_from_format("paquete->buffer->size %d",   paquete->buffer->size ));
	void* stream = malloc(paquete->buffer->size);
	int bytes_escritos = 0;
	memcpy(stream + bytes_escritos, &get_pokemon.largo_nombre_pokemon, sizeof(uint32_t));
	bytes_escritos += sizeof(uint32_t);

	log_info(mi_log,string_from_format("largo nombre %d",get_pokemon.largo_nombre_pokemon));
	log_info(mi_log,string_from_format("nombre %s", get_pokemon.nombre_pokemon));

	//char* nombre = get_pokemon.nombre_pokemon;

	memcpy(stream + bytes_escritos,  get_pokemon.nombre_pokemon, strlen(get_pokemon.nombre_pokemon)+1);
	bytes_escritos += strlen(get_pokemon.nombre_pokemon)+1;

	if(puerto == 5002){
		memcpy(stream + bytes_escritos, &id_mensaje, sizeof(int));
	}
	paquete->buffer->stream = stream;  //Me quede sin abat en el celu , ahora te llamo

	int size_serializados = paquete->buffer->size + 4*sizeof(int);

	void *mensaje_a_enviar = serializar_paquete(paquete,size_serializados);
	log_info(mi_log,string_from_format("sizes: %d",size_serializados));
	int envio = send(socket,mensaje_a_enviar,size_serializados,0);

	if(envio > 0){
		log_info(mi_log,string_from_format("envio: %d",envio));
		log_info(logger, string_from_format("Se envio el mensaje GET a IP: %s y PUERTO: %d",leer_ip_broker(), leer_puerto_broker()));
	}else{
		log_error(logger, "No se pudo enviar el mensaje GET.");
	}

	free(mensaje_a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void* serializar_paquete(t_paquete* paquete, int bytes){
	void* a_enviar = malloc(bytes);
	int offset = 0;

	memcpy(a_enviar + offset, &(paquete->modulo),sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset,&(paquete->cod_op),sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset,&(paquete->mensaje),sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset,&(paquete->buffer->size),sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset,paquete->buffer->stream, paquete->buffer->size);

	return a_enviar;
}

char* leer_puerto_string(int modulo){
	switch (modulo) {
		case BROKER:
			return string_from_format("%d",leer_puerto_broker());
		case GAMECARD:
			return string_from_format("%d",leer_puerto_gamecard());
		case TEAM:
			return string_from_format("%d",leer_puerto_team());
		default:
			return "Modulo incorrecto";
	}
}

int conectarse_a(int modulo){
	switch (modulo) {
		case BROKER:
			return crear_conexion_del_cliente(leer_ip_broker(),leer_puerto_string(BROKER),logger);
		case GAMECARD:
			return crear_conexion_del_cliente(leer_ip_gamecard(),leer_puerto_string(GAMECARD),logger);
		case TEAM:
			return crear_conexion_del_cliente(leer_ip_team(),leer_puerto_string(TEAM),logger);
	}
}
/*void* serializar_paquete(t_paquete* paquete, int *bytes){
	int size_serializado = sizeof(codigo_operacion) + sizeof(tipo_mensaje) + sizeof(tipo_modulo) + sizeof(int) + (*bytes);
	void* buffer = malloc(size_serializado);

	int bytes_escritos = 0;
	log_info(mi_log,"1");

	memcpy(buffer + bytes_escritos, &paquete->modulo, sizeof(codigo_operacion));
	bytes_escritos += sizeof(codigo_operacion);
	log_info(mi_log,"1");

	memcpy(buffer + bytes_escritos, &paquete->cod_op, sizeof(tipo_mensaje));
	bytes_escritos += sizeof(tipo_mensaje);
	log_info(mi_log,"2");

	memcpy(buffer + bytes_escritos, &paquete->mensaje, sizeof(tipo_modulo));
	bytes_escritos += sizeof(tipo_modulo);
	log_info(mi_log,"3");

	memcpy(buffer + bytes_escritos, &paquete->buffer->size, sizeof(int));
	bytes_escritos += sizeof(int);
	log_info(mi_log,"4");

	memcpy(buffer + bytes_escritos, &paquete->buffer->stream, paquete->buffer->size);
	bytes_escritos += paquete->buffer->size;

	(*bytes) = size_serializado;
	return buffer;*/
