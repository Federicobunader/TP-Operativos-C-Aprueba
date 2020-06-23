
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

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		log_error(logger, string_from_format("El cliente fallo al conectarse en IP: %s y PUERTO: %s", ip, puerto));

	freeaddrinfo(server_info);
	log_info(logger, string_from_format("El cliente se conecto exitosamente en IP: %s y PUERTO: %s", ip, puerto));
	return socket_cliente;
}

void liberar_conexion(int socket) {
	close(socket);
}

void levantar_servidor(char* ip, int port, t_log* logger) {

	int socket_servidor;
	char* puerto = string_itoa(port);
	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &servinfo);

	for (p=servinfo; p != NULL; p = p->ai_next) {
		if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			continue;

	    if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
	        close(socket_servidor);
	        socket_servidor = -1;
	        continue;
	    }
	    break;
	}

	if(socket_servidor == -1) {
		log_error(mi_log, "La IP o el PUERTO estan ocupados");
	} else {
		log_info(logger, string_from_format("Servidor levantado en IP: %s y PUERTO: %s", ip, puerto));

		listen(socket_servidor, SOMAXCONN);
		freeaddrinfo(servinfo);

		log_info(mi_log, "Esperando conexiones");

		while(1){
			esperar_cliente(socket_servidor,logger);
		}
	}
}

void esperar_cliente(int socket_servidor,t_log* logger)
{

	pthread_t* espera;
	int err;

	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	if(socket_cliente == -1) {
		log_error(mi_log, "Hubo un error en la conexion con el cliente");
	} else {
		log_info(mi_log,string_from_format("Estableci una conexion con un modulo de socket: %d\n",socket_cliente));
		//log_info(logger,"Estableci una conexion con un modulo\n");

		err = pthread_create(&espera,NULL,(void*)servir_cliente,&socket_cliente);
		if( err != 0){
			log_info(logger,string_from_format("Hubo un error al intentar crear el thread: %s",strerror(err)));
		}
		pthread_detach(espera);
	}
}

void servir_cliente(int* socket)
{
	int cod_modulo;
	if(recv(*socket, &cod_modulo, sizeof(int), MSG_WAITALL) == -1)
		cod_modulo = -1;
	log_info(mi_log, string_from_format("El codigo de modulo es %d", cod_modulo));
	process_request(cod_modulo, *socket);
}

void process_request(int cod_modulo, int socket_cliente) {
	int cod_op;
	recv(socket_cliente,&cod_op,sizeof(int),MSG_WAITALL);
	log_info(mi_log, string_from_format("El codigo de operacion es %d", cod_op));
	switch_cola(cod_op,socket_cliente,cod_modulo);
}
