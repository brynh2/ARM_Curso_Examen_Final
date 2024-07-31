/*
Codigo para el examen final
En este caso no se ha desarrollado e inciso de configurar el usart3 para la trasmision y recepcion de informacion
se ha utilizado en su defecto el uart1 visto en clase. 
*/
#include "potbrazo/potbrazo.h"
#include "servobrazo/servobrazo.h"
#include "uartbrazo/uartbrazo.h"

#define POSICIONES_MAX 10
#define SERVOS 4

uint16_t posiciones[POSICIONES_MAX][SERVOS];
uint8_t posIndex = 0;

void inicializarSistema();
void modoInhabilitado();
void operativo();
void guardarPosiciones();
void recorrerPosiciones();
void reinicializarArreglo();

int main() {
    inicializarSistema();
    reinicializarArreglo();

    while (1) {
        if (USART1->SR & (1 << 5))
        {
            char comando = leerCaracter();
            switch (comando)
            {
                case 'a':
                    modoInhabilitado();
                    break;
                case 'b':
                    operativo();
                    break;
                case 'g':
                    guardarPosiciones();
                    break;
                case 'h':
                    recorrerPosiciones();
                    break;
                case 'i':
                    reinicializarArreglo();
                    break;
            }
        } else
        {
            operativo(); // Continuar en modo teleoperación si no hay comando
        }
    }
}



void inicializarSistema(){
    initADC();
    initPWM();
    initUart();
    enviarPalabra("BIENVENIDOS AL PROGRAMA BRAZO ROBOTICO");
    nuevaLinea();
}

void modoInhabilitado() {
    // No hacer nada
}

void operativo() {
    for (int i = 0; i < SERVOS; i++) {
        uint16_t valor = leerPuerto(i);
        cambiarAngulo(i, valor);
    }
}

void guardarPosiciones() {
    if (posIndex < POSICIONES_MAX) {
        for (int i = 0; i < SERVOS; i++) {
            posiciones[posIndex][i] = leerPuerto(i);
        }
        posIndex++;
    } else {
        enviarPalabra("ARREGLO LLENO");
        nuevaLinea();
    }
}

void recorrerPosiciones() {
    for (int i = 0; i < posIndex; i++) {
        for (int j = 0; j < SERVOS; j++) {
            cambiarAngulo(j, posiciones[i][j]);
        }
        for (int k = 0; k < 1000000; k++); // Simple delay
    }
}

void reinicializarArreglo() {
    for (int i = 0; i < POSICIONES_MAX; i++) {
        for (int j = 0; j < SERVOS; j++) {
            posiciones[i][j] = 6000;
        }
    }
    posIndex = 0;
}
