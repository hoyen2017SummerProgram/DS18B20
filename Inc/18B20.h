 
#ifndef __18B20_H
#define __18B20_H 

#include "stm32f0xx.h"
/*	�ڲ�����������Ҫ��������������͵���������ģʽŶ�����������ֻ���������������ģʽ������©ģʽͬʱ֧������������ѯ����������������衣
		���óɿ�©��������������裬��Ҫ����ʱODRд1�Ϳ����ˡ�
		�����������ã�һ����Ҫ��������һ����д��ȥ��
		����������Լ��Ż�һ�£�Ӧ��Ҳ������һ��д��ɡ������������ַ�ʽ������������ʵ���Դﵽһ�������Ƶ�ʡ�
*/

//�Ĵ�����ֵ
#define BIT32_11 0xFFFFFFFF
//#define BIT32_00 ~BIT32_11
#define BIT16_1 0xFFFF
//#define BIT16_0 ~BIT16_1
#define BIT32_SET(reg,pin,value) (reg = ((reg & ~(0x00000003 << (pin * 2))) | (value << (pin * 2))))//reg:�Ĵ���  pin���̺ܽ�  value���ο��ֲ��ϵ��趨ֵ
#define BIT16_SET(reg,pin,value) (reg = ((reg & ~(0x0001 << pin)) | (value << pin)))//reg:�Ĵ���  pin���̺ܽ�  value���ο��ֲ��ϵ��趨ֵ
#define BIT16_READ(reg,pin)      ((reg & (0x0001 << pin)) >> pin)//reg:�Ĵ���  pin���̺ܽ�  value���ο��ֲ��ϵ��趨ֵ

//IO��������
#define DS18B20_IO_IN()  				BIT32_SET(GPIOC->MODER,11,0x00)//GPIOA_14ģʽ������											00: ����ģʽ( ��λ״̬)	01: ͨ�����ģʽ	10: ���ù���ģʽ	11: ģ��ģʽ
#define DS18B20_IO_OUT() 				BIT32_SET(GPIOC->MODER,11,0x01)//GPIOA_14ģʽ�����											00: ����ģʽ( ��λ״̬)	01: ͨ�����ģʽ	10: ���ù���ģʽ	11: ģ��ģʽ
////IO��������											   
#define	DS18B20_SDA_OUT(level)	BIT16_SET(GPIOC->ODR,11,level)//GPIOA_14������ݣ�								0: �͵�ƽ	1: �ߵ�ƽ
#define	DS18B20_SDA_IN  				BIT16_READ(GPIOC->IDR,11)//GPIOA_14�������ݣ�

uint8_t Init18b20 (void);
void WriteByte (unsigned char wr);  //���ֽ�д��
void read_bytes (unsigned char j);
unsigned char Temp_CRC (unsigned char j);
unsigned int GetTemp (void);
double work_temp(unsigned int tem);
void Config18b20 (void);
void ReadID (void);
double TemperatuerResult(void);

#endif
