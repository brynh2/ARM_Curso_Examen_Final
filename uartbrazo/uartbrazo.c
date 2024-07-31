#include "uartbrazo.h"
/*
Estamos empleando el USART1 y no el USART3, como se especifico en el entregable.

*/
void initUart()
{
    // Habilitamos reloj para GPIOA y USART1
    RCC->APB2ENR |= (1 << 0) | (1 << 2) | (1 << 14); // Habilitar GPIOA y USART1

    // limpiamos los registros para psoteriormente actualizarlos para comunicacion PA9 TX y PA10 RX
    GPIOA->CRH &= ~(0xFF << 4); 
    GPIOA->CRH |= (0xB << 4) | (0xB << 8); 

    // Configurar la comunicacion para el USART1, limpiamos el registro BRR y luego configuraremos el baudrate de lectura. Con cr1 habilitamos USART, TX y RX
    USART1->BRR &= ~(0xFFFF); 
    USART1->BRR |= (0x34 << 4) | (0x1 << 0); // baudrate para 9600
    USART1->CR1 |= (1 << 2) | (1 << 3) | (1 << 13);
}

void enviarCaracter(uint8_t car)
{
    while (!(USART1->SR & (1 << 7))); // Esperar a que el registro de datos esté vacío
    USART1->DR = car;
    while (!(USART1->SR & (1 << 6))); // Esperar a que la transmisión haya terminado
}

void nuevaLinea()
{
    enviarCaracter('\n');
    enviarCaracter('\r');
}

void enviarPalabra(uint8_t palabra[])
{
    uint32_t i = 0;
    for (i = 0; palabra[i] != '\0'; i++)
    {
        enviarCaracter(palabra[i]);
    }
    nuevaLinea();
}

uint8_t leerCaracter(void)
{
    while (!(USART1->SR & (1 << 5))); // Esperar a que haya un dato recibido
    return USART1->DR & 0xFF; // Leer el dato recibido
}
