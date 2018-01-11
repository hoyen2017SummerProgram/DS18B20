 
#ifndef __18B20_H
#define __18B20_H 

#include "stm32f0xx.h"
/*	内部上拉电阻主要是用于推挽输出和单纯的输入模式哦，这两种情况只能是输出或者输入模式，而开漏模式同时支持输出和输入查询，必须外接上拉电阻。
		配置成开漏输出，接上拉电阻，需要输入时ODR写1就可以了。
		如果改输出配置，一般需要读回来改一下再写回去。
		但是如果你自己优化一下，应该也可以用一次写完成——所以这两种方式如果做到最好其实可以达到一样的最高频率。
*/

//寄存器赋值
#define BIT32_11 0xFFFFFFFF
//#define BIT32_00 ~BIT32_11
#define BIT16_1 0xFFFF
//#define BIT16_0 ~BIT16_1
#define BIT32_SET(reg,pin,value) (reg = ((reg & ~(0x00000003 << (pin * 2))) | (value << (pin * 2))))//reg:寄存器  pin：管教号  value：参考手册上的设定值
#define BIT16_SET(reg,pin,value) (reg = ((reg & ~(0x0001 << pin)) | (value << pin)))//reg:寄存器  pin：管教号  value：参考手册上的设定值
#define BIT16_READ(reg,pin)      ((reg & (0x0001 << pin)) >> pin)//reg:寄存器  pin：管教号  value：参考手册上的设定值

//IO方向设置
#define DS18B20_IO_IN()  				BIT32_SET(GPIOC->MODER,11,0x00)//GPIOA_14模式：输人											00: 输入模式( 复位状态)	01: 通用输出模式	10: 复用功能模式	11: 模拟模式
#define DS18B20_IO_OUT() 				BIT32_SET(GPIOC->MODER,11,0x01)//GPIOA_14模式：输出											00: 输入模式( 复位状态)	01: 通用输出模式	10: 复用功能模式	11: 模拟模式
////IO操作函数											   
#define	DS18B20_SDA_OUT(level)	BIT16_SET(GPIOC->ODR,11,level)//GPIOA_14输出数据：								0: 低电平	1: 高电平
#define	DS18B20_SDA_IN  				BIT16_READ(GPIOC->IDR,11)//GPIOA_14输入数据：

uint8_t Init18b20 (void);
void WriteByte (unsigned char wr);  //单字节写入
void read_bytes (unsigned char j);
unsigned char Temp_CRC (unsigned char j);
unsigned int GetTemp (void);
double work_temp(unsigned int tem);
void Config18b20 (void);
void ReadID (void);
double TemperatuerResult(void);

#endif
