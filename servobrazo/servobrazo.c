#include "servobrazo.h"

void initPWM()
{
    // Habilitamos el reloj para los pines de los servos y el timer
    RCC->APB2ENR |= (1 << 3); // GPIOB
    RCC->APB2ENR |= (1 << 4); // GPIOC
    RCC->APB1ENR |= (1 << 1); // Timer 3

    // Configuraramos los pines , primero limpiamos y luego asignamos
    GPIOB->CRL &= ~((0xF << 4) | (0xF << 0));
    GPIOB->CRL |= (0xB << 4) | (0xB << 0);
    GPIOA->CRL &= ~((0xF << 28) | (0xF << 24));
    GPIOA->CRL |= (0xB << 28) | (0xB << 24);

    // Configuramos el Prescalador y su ARR para llegar a una señal de 50Hz
    TIM3->PSC = 72; 
    TIM3->ARR = 20000; 

    //Se limpia y se establecen los registros para los canales del PWM 1 y 4
    TIM3->CCMR1 &= ~((0xFF << 0) | (0xFF << 8));
    TIM3->CCMR2 &= ~((0xFF << 0) | (0xFF << 8));
    TIM3->CCMR1 |= (0x6 << 4) | (0x6 << 12); 
    TIM3->CCMR2 |= (0x6 << 4) | (0x6 << 12); 

    TIM3->CCER |= (1 << 0) | (1 << 4) | (1 << 8) | (1 << 12); 
    TIM3->CR1 |= (1 << 0); 

    // Inicializamos con un pulso de 1ms 
    TIM3->CCR1 = 1000;
    TIM3->CCR2 = 1000;
    TIM3->CCR3 = 1000;
    TIM3->CCR4 = 1000;
}


//Funcion que acepta un servo y su lectura correspondiente para ser movido
void cambiarAngulo(uint8_t servo, uint16_t valorADC)
{
    //Segun el servo, configuraremos para un tiempo en ms
    uint16_t pulso = (valorADC * 1000 / 4096) + 1000; 
    switch (servo)
    {
        case 0:
            TIM3->CCR1 = pulso;
            break;
        case 1:
            TIM3->CCR2 = pulso;
            break;
        case 2:
            TIM3->CCR3 = pulso;
            break;
        case 3:
            TIM3->CCR4 = pulso;
            break;
        default:
            break;
    }
}
