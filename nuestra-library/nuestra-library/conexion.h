/*
 * conexion.h
 *
 *  Created on: 23 abr. 2020
 *      Author: utnso
 */

#ifndef CONEXION_H_
#define CONEXION_H_

#include<sys/socket.h>
#include<netdb.h>
#include<signal.h>
#include<unistd.h>
#include<commons/log.h>
#include<commons/string.h>
#include<stdio.h>
#include<stdlib.h>

/* crear_conexion_del_cliente:
 * Recibe como parametros la ip y el puerto del servidor a conectarse
 * y el logger en donde registrar los sucesos
 *
 * Devuelve como entero la conexion al cliente (socket_cliente)
 * si devuelve -1 la conexion fallo.
 * */
int crear_conexion_del_cliente(char *ip, char* puerto, t_log* logger);

/* liberar_conexion:
 * Recibe un socket al que finalizaremos
 * */
void liberar_conexion(int socket);

/* levantar_servidor:
 * Recibe como parametros la ip y el puerto en donde se levantar el servidor
 * y el logger en donde registrar los sucesos
 *
 * Devuelve como entero el servidor levantado.
 * */
int levantar_servidor(char* ip, char* puerto, t_log* logger);


#endif /* CONEXION_H_ */
