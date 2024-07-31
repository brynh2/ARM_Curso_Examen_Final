#include <stdint.h>
#include "../incFiles/stm32f103x6.h"

void initUart(void);
void enviarCaracter(uint8_t car);
void enviarPalabra(uint8_t palabra[]);
uint8_t leerCaracter(void);
void nuevaLinea(void);