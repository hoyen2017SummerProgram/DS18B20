 
 /*************************此部分为18B20的驱动程序*************************************/

#include "stm32f0xx.h"
#include "18B20.h"

unsigned char  temp_buff[9]; //存储读取的字节，read scratchpad为9字节，read rom ID为8字节
unsigned char  id_buff[8];
unsigned char  *p;
unsigned char  crc_data;
extern void delay_us(uint16_t time);
extern void delay_ms(uint16_t time); 


const unsigned char  CrcTable [256]={
0,  94, 188,  226,  97,  63,  221,  131,  194,  156,  126,  32,  163,  253,  31,  65,
157,  195,  33,  127,  252,  162,  64,  30,  95,  1,  227,  189,  62,  96,  130,  220,
35,  125,  159,  193,  66,  28,  254,  160,  225,  191,  93,  3,  128,  222,  60,  98,
190,  224,  2,  92,  223,  129,  99,  61,  124,  34,  192,  158,  29,  67,  161,  255,
70,  24,  250,  164,  39,  121,  155,  197,  132,  218,  56,  102,  229,  187,  89,  7,
219,  133, 103,  57,  186,  228,  6,  88,  25,  71,  165,  251,  120,  38,  196,  154,
101,  59, 217,  135,  4,  90,  184,  230,  167,  249,  27,  69,  198,  152,  122,  36,
248,  166, 68,  26,  153,  199,  37,  123,  58,  100,  134,  216,  91,  5,  231,  185,
140,  210, 48,  110,  237,  179,  81,  15,  78,  16,  242,  172,  47,  113,  147,  205,
17,  79,  173,  243,  112,  46,  204,  146,  211,  141,  111,  49,  178,  236,  14,  80,
175,  241, 19,  77,  206,  144,  114,  44,  109,  51,  209,  143,  12,  82,  176,  238,
50,  108,  142,  208,  83,  13,  239,  177,  240,  174,  76,  18,  145,  207,  45,  115,
202,  148, 118,  40,  171,  245,  23,  73,  8,  86,  180,  234,  105,  55,  213, 139,
87,  9,  235,  181,  54,  104,  138,  212,  149,  203,  41,  119,  244,  170,  72,  22,
233,  183,  85,  11,  136,  214,  52,  106,  43,  117,  151,  201,  74,  20,  246,  168,
116,  42,  200,  150,  21,  75,  169,  247,  182,  232,  10,  84,  215,  137,  107,  53}; 


/************************************************************
*Function:18B20初始化
*parameter:
*Return: 0.存在  1.不存在
*Modify:
*************************************************************/
uint8_t Init18b20 (void)
{
	uint8_t flag;
	DS18B20_SDA_OUT(1); //数据线置高电平
	delay_us(2); //延时2微秒
	DS18B20_SDA_OUT(0);	//置低
	delay_us(490);   //delay 530 uS//80
  DS18B20_SDA_OUT(1);	 //置高
	delay_us(75);   //delay 100 uS//14

	if(DS18B20_SDA_IN == 0)
		flag = 1;   //detect 1820 success!
	else
		flag = 0;    //detect 1820 fail!
	delay_us(495);        //延时480微秒
	DS18B20_SDA_OUT(1);  //数据线置高电平
	return !flag;
}

/************************************************************
*Function:向18B20写入一个字节
*parameter:
*Return:
*Modify:
*************************************************************/
void WriteByte (unsigned char  wr)  //单字节写入
{
	unsigned char  i;
	
	for (i=0;i<8;i++)
	{		
		DS18B20_SDA_OUT(0);	 //D18B20 = 0;
    delay_us(2);
		//D18B20=wr&0x01;
		if(wr&0x01)	DS18B20_SDA_OUT(1);
		else  DS18B20_SDA_OUT(0);
		delay_us(58);   //delay 45 uS //5
		
		DS18B20_SDA_OUT(1);  //D18B20=1;
    delay_us(2);
		wr >>= 1;
	}
}

/************************************************************
*Function:读18B20的一个字节
*parameter:
*Return:
*Modify:
*************************************************************/
unsigned char ReadByte (void)     //读取单字节
{
	unsigned char  i,u=0;
	for(i=0;i<8;i++)
	{				
		DS18B20_SDA_OUT(0);	   	//D18B20 = 0;
		delay_us (2);
		u >>= 1;
	
		DS18B20_SDA_OUT(1);	 	//D18B20 = 1;
		delay_us (5);
		if(DS18B20_SDA_IN == 1)
		u |= 0x80;
		delay_us (50);
	}
	return(u);
}

/************************************************************
*Function:读18B20
*parameter:
*Return:
*Modify:
*************************************************************/
void read_bytes (unsigned char  j)
{
	 unsigned char  i;
	 for(i=0;i<j;i++)
	 {
		  *p = ReadByte();
		  p++;
	 }
}

/************************************************************
*Function:CRC校验
*parameter:
*Return:
*Modify:
*************************************************************/
unsigned char Temp_CRC (unsigned char j)
{
   	unsigned char  i,crc_data=0;
  	for(i=0;i<j;i++)  //查表校验
    	crc_data = CrcTable[crc_data^temp_buff[i]];
    return (crc_data);
}

/************************************************************
*Function:读取温度
*parameter:
*Return:
*Modify:
*************************************************************/
unsigned int GetTemp (void)
{
	 unsigned int Temperature = 0;
   read_bytes (9);
   if (Temp_CRC(9)==0) //校验正确
   {
	    Temperature = temp_buff[1]*0x100 + temp_buff[0];
    }
	  return Temperature;
}

/****************温度数据处理函数************************

//二进制高字节的低半字节和低字节的高半字节组成一字节,这个
//字节的二进制转换为十进制后,就是温度值的百、十、个位值,而剩
//下的低字节的低半字节转化成十进制后,就是温度值的小数部分

********************************************************/
double work_temp(unsigned int tem)
{
		double getTemp = 1;
		if(tem > 0xF800)//高5位为全1为负温度，全零为正温度，高5位一致
		{
				tem = ~(tem - 1);//负数的补码是先减一然后按位取反
				getTemp = -1;
		}
		
		getTemp *= (tem * 0.0625);
		return getTemp;
}

/************************************************************
*Function:内部配置
*parameter:
*Return:
*Modify:
*************************************************************/
void Config18b20 (void)  //重新配置报警限定值和分辨率
{
     Init18b20();
     WriteByte(0xcc);  //skip rom
     WriteByte(0x4e);  //write scratchpad
     WriteByte(0x19);  //上限
     WriteByte(0x1a);  //下限
     WriteByte(0x7f);     //set 12 bit (0.125)
     Init18b20();
     WriteByte(0xcc);  //skip rom
     WriteByte(0x48);  //保存设定值
     Init18b20();
     WriteByte(0xcc);  //skip rom
     WriteByte(0xb8);  //回调设定值
}

/************************************************************
*Function:读18B20ID
*parameter:
*Return:
*Modify:
*************************************************************/
void ReadID (void)//读取器件 id
{
	Init18b20();
	WriteByte(0x33);  //read rom
	read_bytes(8);
}

/************************************************************
*Function:18B20ID全处理
*parameter:
*Return:
*Modify:
*************************************************************/
double TemperatuerResult(void)
{
  	//p = id_buff;
  	//ReadID();
  	//Config18b20();
	
	//温度转换指令(12位精度为0.0625，需要等待最大转换时间为750ms)
	Init18b20 ();
	WriteByte(0xcc);   //skip rom
	WriteByte(0x44);   //Temperature convert

	//读取温度指令(12位精度为0.0625，需要等待最大转换时间为750ms),这里不做延时，每秒读取一次，本次读上次转换结果
	Init18b20 ();
	WriteByte(0xcc);   //skip rom
	WriteByte(0xbe);   //read Temperature
	p = temp_buff;
	return work_temp(GetTemp());
}
