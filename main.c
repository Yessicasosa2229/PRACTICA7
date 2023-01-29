#include "lib/include.h"
volatile float temperatura1 = 0;

int main(void)
{   
   // int8_t word[2];
    uint16_t count = 0;
    Configurar_PLL(_10MHZ);  //Confiuracion de velocidad de reloj 50MHZ
    Configurar_GPIO();
    Configurar_UART0();
    Configurar_SSI3();
    while(1)
    {
        temperatura1 = SPI_readMAX();
    }

}
