#include "lib/include.h"
extern void Configurar_SSI3(void)
{
    
    SYSCTL->RCGCSSI |= (1<<3); //se activa el Modulo 3 SSI3     
    SYSCTL->RCGCGPIO |= (1<<3);//Puerto D
    GPIOD_AHB->DIR |= (0<<3) | (1<<2) | (1<<1) | (0<<0); //selector es salida = 1
    GPIOD_AHB->AFSEL = (1<<3)|(1<<2)|(1<<1)|(1<<0); //antes del 4 al 7
    GPIOD_AHB->PCTL = (GPIOD_AHB->PCTL&0xFFFF0000) | 0x00001111; // tabla p.688
    //GPIOD_AHB->PCTL &= 0xFFFF0000;
    //GPIOD_AHB->PCTL |= 0x0000FFFF; // tabla p.688
    GPIOD_AHB->DEN |= (1<<0)|(1<<1)|(1<<2)|(1<<3);
    //                MISO    MOSI    CS    SCLK

   // GPIOD_AHB->DATA |= (1<<5); //registrar CS
    //GPIOB->PUR |= (0<<7)|(0<<6)|(0<<5)|(0<<4);
    //GPIOB->PDR |= (0<<7)|(0<<6)|(0<<5)|(0<<4);
    //GPIOB->AMSEL |= (0<<7)|(0<<6)|(0<<5)|(0<<4);
    
    SSI3->CR1 = (0<<1); //SSE=0 deshabilitar modulo
    SSI3->CR1 = (0<<2); //MS = 0 modo maestro
    SSI3->CC = (0x0<<0); //system clock = 50MHz
    //SSInClk = SysClk / (CPSDVSR * (1 + SCR))
    //2 500 000 = 50 000 000/(2*(1+SCR))
    // SCR = (50 000 000/2 500 000*2) - 1 = 9
    SSI3->CPSR = 2; // 4.3 MHZ (0x2; 0b010, 2, 1<<2)
    
    SSI3->CR0 = (5<<8); // SCR (a partir del bit 8)
    SSI3->CR0 |= (1<<7); // SPH
    SSI3->CR0 |= (1<<6); // SPO
    SSI3->CR0 |= (0<<4); // FRF
    SSI3->CR0 |= (Bdata<<0); // DSD 16-bit data
    SSI3->CR1 |= (1<<1); //SSE=1 habilitar modoulo p.961 (0x02)    
}

extern void SPI_write(uint16_t data)
{
    while (SSI3->SR & 0x2)
    {
        SSI3->DR = (uint16_t)data;
    }
    
}
/*
extern void SPI_write_data(uint8_t reg, uint8_t data)
{
    GPIOD_AHB->DATA &= ~(1<<3); // CS = 0 se niega
    SPI_write(reg & ~0x80); //escribir registro + MSB igualado a cero
    SPI_write(data);
    GPIOD_AHB->DATA |= (1<<3); //CS = 1
}
*/
extern void SPI_write_data(void)
{
    GPIOD_AHB->DATA |= (0<<1); // CS = 0 
    SPI_write(0b11101010); //escribir registro + MSB igualado a cero
    GPIOD_AHB->DATA |= (1<<1); //CS = 1
}

extern uint16_t SPI_readMAX(void)
{
    int tem = 0;
    uint16_t data = 0;
    GPIOD_AHB->DATA |= (1<<1); // CS = 0
    SPI_write(0x00); //escribir dato para generar señal de reloj
    while ((SSI3->SR & 0x10 ) == 0x10){
        //espera por bitbusy
    }
    data = SSI3->DR;
    GPIOD_AHB->DATA |= (0<<1);
    
    data = (data>>3);
    data |= (0<<12)|(0<<13);
    tem = (float)(0.25*data);
    return tem;
}
