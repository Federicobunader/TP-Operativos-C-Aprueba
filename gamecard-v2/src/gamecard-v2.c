#include "gamecard-v2.h"

int main(void)
{

	//DATO = POSICION_X-POSICION_Y=CANTIDAD
	//char* obtener_posicion_del_dato(char* dato_a_escribir) -> POSICION_X-POSICION_Y
	//int obtener_cantidad_del_dato(char* dato_a_escribir) -> CANTIDAD

	//disminuir_cantidad_de_pokemon_en_la_posicion("Pikachu",10,2); -> CATCH
	//NEW => void guardar_informacion(char* nombre_pokemon,int posicion_x,int posicion_y,int cantidad)
	//GET => t_list* armar_mensaje_get(char* nombre_pokemon) => Devuelve una lista de DATOS

	/*LISTA DE FREES :
	 * 1) Agregue en utils,c (linea 247) 2 frees y anda bien
	 * 2) Agregue 1 free en bitmap.c linea 17 y anda
	 * 3) Agregue 1 free en bitmap.c linea 24 y anda
	 * 4) Bloques.c linea 187, puse 1 free y anda [free(path_bloque_a_escribir);]
	 * 5) Bloques.c linea 192, puse 1 free y anda [free(dato_a_escribir_restante);]
	 * 6) Bloques.c linea 186, puse 1 free y anda [free(a_escribir);]
	 * 7) Bloques.c linea 195, puse 1 free y anda [free(a_escribir_restante);]
	 * 8) Bloques.c linea 195, puse 1 free y anda [free(string_bloque_a_escribir);]
	 * 9) Bloques.c linea 335, puse 1 free y anda [free(pivot_partido);]
	 * 10) Bloques.c linea 172, puse 1 free y anda [free(string_bloque_a_escribir);]
	 * 11) Bloques.c linea 518, puse 1 free y anda [free(string_i);]
	 */


	iniciar_gamecard();

	guardar_informacion("Pikachu",105,50,226);
	guardar_informacion("Squirtle",105,50,10);
	guardar_informacion("Pikachu",105,50,1111);
	guardar_informacion("Pikachu",11,51,1111);
	guardar_informacion("Pikachu",12,52,1111);
	guardar_informacion("Pikachu",13,53,1111);
	guardar_informacion("Pikachu",14,54,1111);
	guardar_informacion("Pikachu",15,55,1111);
	guardar_informacion("Pikachu",155,555,1111);
	guardar_informacion("Pikachu",17,57,1111);
	guardar_informacion("Pikachu",19,5,10);
	guardar_informacion("Pikachu",5,3,17);
	guardar_informacion("Pikachu",60,5,10);
	guardar_informacion("Pikachu",1,2,17);
	guardar_informacion("Pikachu",12,3,100);
	guardar_informacion("Pikachu",1,5,17);
	guardar_informacion("Pikachu",4,7,100);
	guardar_informacion("Pikachu",9,12,18);
	guardar_informacion("Pikachu",5,3,17);
	guardar_informacion("Pikachu",6,5,10);
	guardar_informacion("Pikachu",1,2,17);
	guardar_informacion("Pikachu",12,3,10);
	guardar_informacion("Pikachu",10,1,17); // ACA ROMPE -> LA POSICION 12-3 = 10 la guarda bien
	guardar_informacion("Pikachu",10,2,1);
	guardar_informacion("Pikachu",10,3,17);
	guardar_informacion("Pikachu",10,4,17);
	guardar_informacion("Pikachu",10,5,17);
	guardar_informacion("Pikachu",10,6,17);
	guardar_informacion("Pikachu",55,6,17);
	guardar_informacion("Pikachu",51,6,17);
	guardar_informacion("Pikachu",52,6,17);
	guardar_informacion("Pikachu",1,555,10);
	guardar_informacion("Pikachu",52,65,17);
	guardar_informacion("Pikachu",52,61,17);
	guardar_informacion("Pikachu",52,22,17);
	guardar_informacion("Pikachu",52,13,17);
	guardar_informacion("Pikachu",1,5,1000);
	guardar_informacion("Pikachu",512,621,17);
	guardar_informacion("Pikachu",100,12,18);

	guardar_informacion("Squirtle",1,1,10);
	guardar_informacion("Squirtle",2,2,20);
	guardar_informacion("Squirtle",3,3,30);
	guardar_informacion("Squirtle",5,5,50);


	t_list* posiciones = armar_mensaje_get("Pikachu");
	if(!list_is_empty(posiciones)){
		for(int i = 0; i<list_size(posiciones);i++ ){
			log_info(nuestro_log,"Posicion %d : %s",i,list_get(posiciones,i));
		}
	}

	disminuir_cantidad_de_pokemon_en_la_posicion("Pikachu",10,2);
	disminuir_cantidad_de_pokemon_en_la_posicion("Pikachu",10,2);
	disminuir_cantidad_de_pokemon_en_la_posicion("Pikachu",100,202);
	log_info(nuestro_log,"PRUEBA 0.0");
	disminuir_cantidad_de_pokemon_en_la_posicion("Pikachu",105,50);
	log_info(nuestro_log,"PRUEBA 0.1");
	disminuir_cantidad_de_pokemon_en_la_posicion("Pikachu",105,50);
	log_info(nuestro_log,"PRUEBA 0.2");
	disminuir_cantidad_de_pokemon_en_la_posicion("Pikachu",105,50);
	log_info(nuestro_log,"PRUEBA 0.3");

	posiciones = armar_mensaje_get("Pikachu");
	if(!list_is_empty(posiciones)){
		for(int i = 0; i<list_size(posiciones);i++ ){
			log_info(nuestro_log,"Posicion %d : %s",i,list_get(posiciones,i));
		}
	}

	log_info(nuestro_log,"PRUEBA 1");

	disminuir_cantidad_de_pokemon_en_la_posicion("PokemonInexistente",105,50);

	posiciones = armar_mensaje_get("Squirtle");
	if(!list_is_empty(posiciones)){
		for(int i = 0; i<list_size(posiciones);i++ ){
			log_info(nuestro_log,"Posicion %d : %s",i,list_get(posiciones,i));
		}
	}

	//buscar_posicion_en_el_archivo(path_nombre_metadata,"52-13");

	for(int j = 0;j<10;j++){
		log_info(nuestro_log,"VALOR DEL BITMAP %d: %d",j,bitarray_test_bit(bitmap,j));
	}

	log_info(nuestro_log,"Termine");
	//log_info(nuestro_log,config_get_string_value(config,"miele"));
	terminar_gamecard();
	return EXIT_SUCCESS;
}


void iniciar_gamecard() {
	iniciar_config();
	logger = log_create("/home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/src/resources/logger.txt", "gamecard", false, LOG_LEVEL_INFO);
	nuestro_log = log_create("/home/utnso/Documentos/tp-2020-1c-C-aprueba/gamecard-v2/src/resources/nuestro_log.txt", "gamecard", true, LOG_LEVEL_INFO);
	punto_montaje_tallgrass = leer_punto_montaje_tallgrass();
	crear_directorio(punto_montaje_tallgrass);
	crear_archivo_metadata(64,1000);
	creacion_archivo_files_metadata(devolver_path_directorio_files(),"Y","0","","N");

	pthread_mutex_init(&Mutex_Bitmap,NULL);
	pthread_mutex_init(&asignar_bloque,NULL);
	pthread_mutex_init(&mutex_liberar_bloque,NULL);
	pthread_mutex_init(&mutex_facu,NULL);

	//Iniciamos las variables globales del constructor
	flag_bloques_libres = 1; //hay bloques libres
	ultimo_bloque_asignado = 0; //inicio como ultimo bloque asignado el primero

	if(!se_creo_el_bloque()){
		crear_bloque();
	}
	crear_bitmap(); //levanto el bitarray;

}

void terminar_gamecard() {
	destruir_config();
	bitarray_destroy(bitmap);
	log_destroy(logger);
	log_destroy(nuestro_log);
}
