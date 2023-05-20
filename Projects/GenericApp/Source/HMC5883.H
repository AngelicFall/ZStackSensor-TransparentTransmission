#include "ioCC2530.h"
#include  <math.h>    //Keil library  
#include  <stdio.h>   //Keil library	
#define   uchar unsigned char
#define   uint unsigned int	
//ʹ�õĶ˿ڣ��밴�����½���
//P1_2;
#define	HMC5883_SCL P1_3      //IICʱ�����Ŷ���
#define	HMC5883_SDA P1_4      //IIC�������Ŷ���
#define HMC5883_DATAOUT P1DIR|=0x10   //�������
#define HMC5883_DATAIN  P1DIR&=(~0x10)   //�������� ����

#define	SlaveAddress   0x3C	  //����������IIC�����еĴӵ�ַ
typedef unsigned char BYTE;
BYTE BUF[8];                         //�������ݻ�����      	
void Init_HMC5883(void);            //��ʼ��5883
void  Single_Write_HMC5883(uchar REG_Address,uchar REG_data);   //����д������
uchar Single_Read_HMC5883(uchar REG_Address);                   //������ȡ�ڲ��Ĵ�������
void  Multiple_Read_HMC5883(void);                                  //�����Ķ�ȡ�ڲ��Ĵ�������
//������ģ��iicʹ�ú���-------------
void HMC5883_Start(void);
void HMC5883_Stop(void);
void HMC5883_SendACK(uchar ack);
uchar HMC5883_RecvACK(void);
void HMC5883_SendByte(BYTE dat);
BYTE HMC5883_RecvByte(void);
void HMC5883_ReadPage(void);
void HMC5883_WritePage(void);
void HMC5883_DelayUs(uint16 usec);
void HMC5883_DelayMs(uint16 msec);
//-----------------------------------
void HMC5883_DelayUs(uint16 usec)
{ usec>>= 1;
    while(usec--)
    {
        asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
        asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
    }
}
void HMC5883_DelayMs(uint16 msec)
{ while(msec--)
        HMC5883_DelayUs(1000);
  
}
/**************************************
��ʼ�ź�
**************************************/
void HMC5883_Start()
{  
    HMC5883_SDA = 1;                    //����������
    HMC5883_SCL = 1;                    //����ʱ����
    HMC5883_DelayUs(5);                 //��ʱ
    HMC5883_SDA = 0;                    //�����½���
     HMC5883_DelayUs(5);                 //��ʱ
    HMC5883_SCL = 0;                    //����ʱ����
}

/**************************************
ֹͣ�ź�
**************************************/
void HMC5883_Stop()
{   
    HMC5883_SDA = 0;                    //����������
    HMC5883_SCL = 1;                    //����ʱ����
    HMC5883_DelayUs(5);                  //��ʱ
    HMC5883_SDA = 1;                    //����������
     HMC5883_DelayUs(5);                  //��ʱ
}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void HMC5883_SendACK(uchar ack)
{   
    HMC5883_SDA = ack;                  //дӦ���ź�
    HMC5883_SCL = 1;                    //����ʱ����
     HMC5883_DelayUs(5);                  //��ʱ
    HMC5883_SCL = 0;                    //����ʱ����
    HMC5883_DelayUs(5);                  //��ʱ
}

/**************************************
����Ӧ���ź�
**************************************/
uchar HMC5883_RecvACK(void)
{  uint8 HMC5883_CY = 0x00;
  HMC5883_DATAIN;
    HMC5883_SCL = 1;                    //����ʱ����
     HMC5883_DelayUs(5);                  //��ʱ
    HMC5883_CY = HMC5883_SDA;                   //��Ӧ���ź�
    HMC5883_SCL = 0;                    //����ʱ����
     HMC5883_DelayUs(5);                 //��ʱ
     HMC5883_DATAOUT;
    return HMC5883_CY;
}

/**************************************
��IIC���߷���һ���ֽ�����
**************************************/
void HMC5883_SendByte(BYTE dat)
{
    BYTE i;

    for (i=0; i<8; i++)         //8λ������
    { 
      if (dat&0x80)HMC5883_SDA=1;
       else HMC5883_SDA=0 ;                   //�����ݿ�
      
                   //�Ƴ����ݵ����λ
        HMC5883_SCL = 1;                //����ʱ����
        HMC5883_DelayUs(5);              //��ʱ
        HMC5883_SCL = 0;                //����ʱ����
         HMC5883_DelayUs(5);              //��ʱ
         dat <<= 1;   
    }
    HMC5883_RecvACK();

}

/**************************************
��IIC���߽���һ���ֽ�����
**************************************/
BYTE HMC5883_RecvByte(void)
{
    BYTE i;
    BYTE dat = 0;

    HMC5883_SDA = 1;                    //ʹ���ڲ�����,׼����ȡ����,
     HMC5883_DelayUs(5);            //��ʱ
     HMC5883_DATAIN;
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        HMC5883_SCL = 1;                //����ʱ����
         HMC5883_DelayUs(5);            //��ʱ
        dat |= HMC5883_SDA;             //������               
        HMC5883_SCL = 0;                //����ʱ����
         HMC5883_DelayUs(5);             //��ʱ
    }
    HMC5883_DATAOUT;
    return dat;
}

//***************************************************

void Single_Write_HMC5883(uchar REG_Address,uchar REG_data)
{
    HMC5883_Start();                  //��ʼ�ź�
    HMC5883_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    HMC5883_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ����ο�����pdf 
    HMC5883_SendByte(REG_data);       //�ڲ��Ĵ������ݣ���ο�����pdf
    HMC5883_Stop();                   //����ֹͣ�ź�
}

//********���ֽڶ�ȡ�ڲ��Ĵ���*************************
uchar Single_Read_HMC5883(uchar REG_Address)
{  uchar REG_data;
    HMC5883_Start();                          //��ʼ�ź�
    HMC5883_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
    HMC5883_SendByte(REG_Address);                   //���ʹ洢��Ԫ��ַ����0��ʼ	
    HMC5883_Start();                          //��ʼ�ź�
    HMC5883_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
    REG_data=HMC5883_RecvByte();              //�����Ĵ�������
    HMC5883_SendACK(1);   
    HMC5883_Stop();                           //ֹͣ�ź�
    return REG_data; 
}
//******************************************************
//
//��������HMC5883�ڲ��Ƕ����ݣ���ַ��Χ0x3~0x5
//
//******************************************************
void Multiple_Read_HMC5883(void)
{   uchar i;
    HMC5883_Start();                          //��ʼ�ź�
    HMC5883_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
    HMC5883_SendByte(0x03);                   //���ʹ洢��Ԫ��ַ����0x32��ʼ	
    HMC5883_Start();                          //��ʼ�ź�
    HMC5883_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
    for (i=0; i<6; i++)                      //������ȡ6����ַ���ݣ��洢��BUF
    {
        BUF[i] = HMC5883_RecvByte();          //BUF[0]�洢0x32��ַ�е�����
        if (i == 5)
        {
           HMC5883_SendACK(1);                //���һ��������Ҫ��NOACK
        }
        else
        {
          HMC5883_SendACK(0);                //��ӦACK
       }
   }
    HMC5883_Stop();                          //ֹͣ�ź�
    HMC5883_DelayMs(5); 
}

//��ʼ��HMC5883��������Ҫ��ο�pdf�����޸�****
void Init_HMC5883(void)
{ HMC5883_DATAOUT;
     Single_Write_HMC5883(0x02,0x00);  //
}