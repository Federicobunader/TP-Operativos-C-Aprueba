#include "broker-v2.h"


struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message;

int main(void)
{
	//prueba message queue

	/*	key_t key;
	    int msgid;

	    // ftok to generate unique key
	    key = ftok("Catched",65);

	    // msgget creates a message queue
	    // and returns identifier
	    msgid = msgget(key, 0666 | IPC_CREAT);

	    // msgrcv to receive message
	    msgrcv(msgid, &message, sizeof(message), 1, 0);

	    // display the message
	    printf("Data Received is : %s \n",
	                    message.mesg_text);

	    // to destroy the message queue
	    msgctl(msgid, IPC_RMID, NULL);

	    return 0;

	//prueba message queue
*/


	iniciar_broker();

	terminar_broker();

	printf("Todo joya \n");
	return EXIT_SUCCESS;

}

void iniciar_broker(){
	printf("-----------------Iniciando archivo config-----------------\n");
	iniciar_config();
	inicializar_message_queues();
	logger = log_create(leer_log_file(),"broker",false,LOG_LEVEL_INFO);
	mi_log = log_create("src/resources/mi_log_broker.txt","broker",true,LOG_LEVEL_INFO);
}

void terminar_broker(){
	printf("-----------------Destruyendo archivo config-----------------\n");
	destruir_config();
	terminar_message_queues();
	log_destroy(logger);
	log_destroy(mi_log);
}
