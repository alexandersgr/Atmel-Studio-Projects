//***************************************************************************
//
//  Author(s)...: Pashgan    http://ChipEnable.Ru   
//
//  Target(s)...: Mega
//
//  Compiler....: 
//
//  Description.: ������� SPI
//
//  Data........: 26.07.13
//
//***************************************************************************
#ifndef SPI_H
#define SPI_H

#include "compilers_4.h"

#define SPI_PORTX   PORTB
#define SPI_DDRX    DDRB

#define SPI_MISO   4 
#define SPI_MOSI   3
#define SPI_SCK    5
#define SPI_SS     2

/*
#define SPI_PORTX   PORTB
#define SPI_DDRX    DDRB

#define SPI_MISO   4
#define SPI_MOSI   3
#define SPI_SCK    5
#define SPI_SS     2



/*_______________ ������� ____________________*/


/*��������� SPI ����������*/
#define SPI_DisableSS_m(ss)  do{SPI_PORTX |= (1<<(ss)); }while(0)

/*��������� SPI ����������*/
#define SPI_EnableSS_m(ss)   do{SPI_PORTX &= ~(1<<(ss)); }while(0)

/*������ SPI ����������*/
#define SPI_StatSS_m(ss)    (!(SPI_PORTX & (1<<(ss))))

/*�������� ���� ������ �� SPI*/
#define SPI_WriteByte_m(data)  do{ SPDR = data; while(!(SPSR & (1<<SPIF))); }while(0)

/*��������� ���� ������ �� SPI*/
#define SPI_ReadByte_m(data)  do{ SPDR = 0xff; while(!(SPSR & (1<<SPIF))); data = SPDR;}while(0)


/* ______________ ������������ ������� _____________*/


/*�������� ���� ������ �� SPI*/
inline static uint8_t SPI_ReadByte_i(void){
   SPDR = 0xff;
   while(!(SPSR & (1<<SPIF)));
   return SPDR;   
}

/*  _________________ ������� ________________ */

/*������������� SPI ������*/
void SPI_Init(void); 

/*��������� ���� ������ �� SPI*/
void SPI_WriteByte(uint8_t data); 

/*�������� ���� ������ �� SPI*/
uint8_t SPI_ReadByte(void);

/*��������� � �������� ���� ������ �� SPI*/
uint8_t SPI_WriteReadByte(uint8_t data);

/*��������� ��������� ���� ������ �� SPI*/
void SPI_WriteArray(uint8_t num, uint8_t *data);

/*��������� � �������� ��������� ���� ������ �� SPI*/
void SPI_WriteReadArray(uint8_t num, uint8_t *data);


#endif //SPI_H