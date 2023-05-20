/*******************************************************************************
 * �ļ����ƣ�LCD.h
 * ��    �ܣ�LCD����
 *           ����12232����ͼ��Һ��
 * Ӳ�����ӣ�Һ��ģ����CC2530��Ӳ�����ӹ�ϵ���£�
 *                Һ��ģ��                         CC2530
 *
 *               SPI-CLK(PIN2) ��������             P1.7
 *               SPI-MOSI(PIN3)��λ�Ĵ���ʱ������   P1.6
 *               GPH(PIN5)   �洢�Ĵ���ʱ������     P1.5
 * ��    �ߣ�w
 * ��    ˾��������̫�Ƽ����޹�˾
 ******************************************************************************/

#ifndef LCD12232_H_
#define LCD12232_H_

/* ����ͷ�ļ� */
/********************************************************************/
#include "ioCC2530.h"

#define      SPIdata           P1_6
#define      SPImosi           P1_5
#define      GPH               P1_3

#define      H_SPImosi()       SPImosi = 1 
#define      L_SPImosi()       SPImosi = 0

#define      H_GPH()             GPH = 1
#define      L_GPH()             GPH = 0



#define NOP()  asm("NOP")

#define witch_MI 0x11   //���дָ��
#define witch_MD 0x19  //���д����
#define witch_SI 0x12  //�ұ�дָ��
#define witch_SD 0x1A  //�ұ�д����

void LCD_SPISendData(unsigned char Dat);

void LCD_SPIOUT(void);

void SendDataMI16(unsigned char MI,unsigned char NI);

void SendDataMD16(unsigned char MD,unsigned char ND);

void LCD_lni(void);

void Setpage(unsigned char page0,unsigned char page1);

void Setadderss(unsigned char adderss0,unsigned char adderss1);

void Putchar0(unsigned char ch);
                                                            
void Putchar1(unsigned char ch);

void LCD_clrscr(void);

void show_left_up(void);

void show_right_up(void);

void show_left_down(void);

extern  void show_right_down(unsigned char number);

#endif
 