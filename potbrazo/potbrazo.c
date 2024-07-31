#include "potbrazo.h"

void initADC()
{
    // Habilitar reloj GPIOA con funciones y ADC1
    RCC->APB2ENR |= (1<<0)|(1 << 2) | (1 << 9);
    //COnfiguramos los puertos PA0 al PA3 como puertos analógicos
    GPIOA->CRL &= ~((0xF << 0) | (0xF << 4) | (0xF << 8) | (0xF << 12));

    //Desabilitamos el ADC para iniciar su configuracion
    ADC1->CR2 &= ~(0x1 << 0); 

    //Limpiamos los registros SQR ADC para iniciar una configuracion nueva
    ADC1->SQR1 &= ~(0xFFFFFF); 
    ADC1->SQR2 &= ~(0x3FFFFFFF); 
    ADC1->SQR3 &= ~(0x3FFFFFFF); 

    // Configuraramos tiempos de muestreo para canales 0-3
    ADC1->SMPR2 |= (0x7 << 0) | (0x7 << 3) | (0x7 << 6) | (0x7 << 9);

    ADC1->CR2 |= (0x1 << 0); // Habilitar ADC1
}


uint16_t leerPuerto(uint8_t canal)
{
    //Limpiamos la sesion de conversion
    ADC1->SQR3 &= ~(0x3FFFFFFF); 
    // Tenemos a la ventana el canal arbitrario para su conversion
    ADC1->SQR3 |= (canal << 0); 
    //Inicamos la conversion
    ADC1->CR2 |= (0x1 << 22);
    while (!(ADC1->SR & (0x1 << 1))); // Esperar hasta que la conversión termine

    uint16_t valorADC = ADC1->DR & 0xFFF; // Leer valor convertido
    ADC1->SR &= ~(1 << 1); // Limpiar bandera de fin de conversión
    return valorADC;
}
