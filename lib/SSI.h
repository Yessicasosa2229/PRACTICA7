#ifndef SPI_SPI_H_
#define SPI_SPI_H_

#define Bdata 0xF 

extern void Configurar_SSI3(void);
extern void SPI_write(uint16_t data);
extern void SPI_write_data(void);
extern uint8_t SPI_read(void);
extern uint8_t SPI_read_data(uint8_t reg);

#endif /* SPI_SPI_H_ */


