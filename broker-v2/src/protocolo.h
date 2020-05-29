#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include "suscriptores.h"

pthread_t* ack;

/*Uno para verificar que tipo de modulo se conecto y el otro para verificar la conexion */

typedef enum
{
	MENSAJE=1,
	SUSCRIPCION=2
}op_code; /** Para identificar cuando se recibe una conexion que tipo de operacion se va a realizar*/

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct{
	int id;
	int id_cor;
	mq_nombre cola;
	/** Todavia no estoy seguro si hago dos listas de suscriptores (a los que ya se envio y los que falta recibir confirmacion),
	o una sola lista de una estrucutura que tenga el sucriptor y su estado */
	t_list* suscriptores_env;
	t_list* suscriptores_conf;
	t_buffer* buffer;

}t_mensaje; /** Es la estructura con la que se va a guardar los mensajes en la memoria interna del broker, contiene mas informacion */

typedef struct{
	int id;
	int id_cor;
	t_buffer* buffer;
}t_paquete; /**  Es la estructura con la que se va a enviar los mensajes a los modulos */

void agregar_suscriptor_cola(t_mq* cola,suscriptor_t* suscriptor);

/**Tambien se podria separar en dos funciones */
void recibir_y_guardar_mensaje(int socket_cliente,t_mq* queue);

t_mensaje* crear_mensaje(void* buffer,int tamanio,mq_nombre cola);

void agregar_msj_cola(t_mq* cola,t_mensaje* mensaje);

void enviar_mensaje_suscriptores(t_mq* cola);

/** Crea el paquete para manda en funcion del mensaje */
void enviar_mensaje(t_mensaje* mensaje,suscriptor_t* suscriptor);

void* serializar_paquete(t_paquete* paquete, int bytes);

void enviar_id_msj_cliente(int socket_cliente,t_mq* mq,int id_msj);

/* Falta definir los siguientes metodos */

void recibir_ACK(suscriptor_t* socket_cliente,t_mensaje* mensaje);
int asignar_id_univoco();
/*Al suscribirse un modulo, se le tienen que mandar todos los mensajes correspondientes */
void mandar_mensajes_cache();
void add_sub_lista_env_msj(t_mensaje* mensaje,suscriptor_t* suscriptor);
void add_sub_lista_conf_msj(t_mensaje* mensaje,suscriptor_t* suscriptor);


#endif PROTOCOLO_H_