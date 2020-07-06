

#ifndef BITMAP_H_
#define BITMAP_H_

#include "utils.h"
#include "bloques.h"

void crear_bitmap();
int obtener_nuevo_bloque_libre();
void escribir_bitmap();
void destruir_bloque();
void liberar_bloque(int bloque);
void escribir_bloque_asignado(int bloque);

#endif /* BITMAP_H_ */
