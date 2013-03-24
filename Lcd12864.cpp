#include <arduino.h>
#include <SPI.h>
#include "Lcd12864.h"

Lcd12864::Lcd12864(byte pBgLight, byte pMOSI, byte pCLK, byte pAO, byte pREST, byte pCS)
{
	pinBGLight = pBgLight;
	pinMOSI = pMOSI;
	pinCLK = pCLK;
	pinAO = pAO;
	pinREST = pREST;
	pinCS = pCS;
}

void Lcd12864::sendByte(unsigned char Dbyte)
{
  digitalWrite(pinCS, LOW);
#if PHYSICAL_SPI
  SPI.transfer(Dbyte);
#else
  unsigned char TEMP; 
  TEMP=Dbyte;
  for(int i = 0; i < 8; i++)
  {
    digitalWrite(pinCLK, LOW);
    TEMP= (Dbyte << i) & 0X80;
    digitalWrite(pinMOSI, TEMP);
    digitalWrite(pinCLK, HIGH);
  }
#endif
  digitalWrite(pinCS, HIGH);
}

void Lcd12864::sendCmd(unsigned char Cbyte)
{
  digitalWrite(pinAO, LOW);
  sendByte(Cbyte);
}

void Lcd12864::sendData(unsigned char Dbyte)
{
  digitalWrite(pinAO, HIGH);
  sendByte(Dbyte);
}

void Lcd12864::switchBgLight(boolean value) {
	digitalWrite(pinBGLight, !value);
}

void Lcd12864::reset() {
    digitalWrite(pinCS, LOW);
    digitalWrite(pinREST, LOW);  
    delay(200);
    digitalWrite(pinREST, HIGH);
    delay(1000);	
    
	sendCmd(0xE2); //system reset
    delay(200);
}

void Lcd12864::setup()
{
  pinMode(pinBGLight, OUTPUT);
  pinMode(pinMOSI, OUTPUT);  
  pinMode(pinCLK, OUTPUT);
  pinMode(pinAO, OUTPUT);
  pinMode(pinREST, OUTPUT);  
  pinMode(pinCS, OUTPUT);  

#if PHYSICAL_SPI
  // initialize SPI:
  SPI.begin(); 
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
#endif	

  reset();
  
  sendCmd(0x24);//SET VLCD RESISTOR RATIO
  sendCmd(0xa2);//BR=1/9
  sendCmd(0xa0);//set seg direction
  sendCmd(0xc8);//set com direction
  sendCmd(0x2f);//set power control
  sendCmd(0x40);//set scroll line
  sendCmd(0x81);//SET ELECTRONIC VOLUME
  sendCmd(0x20);//set pm: 通过改变这里的数值来改变电压 
  //sendCmd(0xa6);//set inverse display	   a6 off, a7 on
  //sendCmd(0xa4);//set all pixel on
  sendCmd(0xaf);//set display enable

  clear();
}

/*************************
 * 8*8字符，取模顺序是列行式，
 * 从上到下，高位在前，从左到右；
 * 先选择页地址0-7，再选择列0-130
 * 页码是直接读取8位数据作为地址；
 * 列是先读取高四位，后读取低四位；
 **********************/
void Lcd12864::render8x8(unsigned char row,unsigned char col,unsigned char count,unsigned char const *put)
{		
  unsigned int X=0;
  unsigned int i=0,j=0;
  sendCmd(0xb0+row);
  sendCmd(0x10+(8*col/16));		
  sendCmd(0x00+(8*col%16));
  for(j=0;j<count;j++)
    for(i=0;i<8;i++) sendData(put[X++]); 	
}

/*****************
 * 8*16字符，取模顺序是列行式，
 * 从上到下，高位在前，从左到右；
 * 先选择页地址0-7，再选择列0-127
 * 页码是直接读取8位数据作为地址；
 * 列是先读取高四位，后读取低四位；
 **********************/
void Lcd12864::render8x16(unsigned char row,unsigned char col,unsigned char count,unsigned char const *put)
{		
  unsigned int X=0;
 unsigned int i=0,j=0;
  sendCmd(0xb0+row);
  sendCmd(0x10+(8*col/16));		
  sendCmd(0x00+(8*col%16));
  for(j=0;j<count;j++)
  { 
    for(i=0;i<8;i++) sendData(put[X++]); 
    sendCmd(0xb1+row);	
    sendCmd(0x10+(8*col/16));		
    sendCmd(0x00+(8*col%16));
    for(i=0;i<8;i++) sendData(put[X++]);
    sendCmd(0xb0+row);
    col=col+1; 
  } 
}

/*****************
 * 16*16字符，取模顺序是列行式，
 * 从上到下，高位在前，从左到右；
 * 先选择页地址0-7，再选择列0-127
 * 页码是直接读取8位数据作为地址；
 * 列是先读取高四位，后读取低四位；
 **********************/
void Lcd12864::render16x16(unsigned char row,unsigned char col,unsigned char count,unsigned char const *put)
{		
  unsigned int X=0;
 unsigned int i=0,j=0;
  sendCmd(0xb0+row);
  sendCmd(0x10+(8*col/16));		
  sendCmd(0x00+(8*col%16));
  for(j=0;j<count;j++)
  { 
    for(i=0;i<16;i++) sendData(put[X++]); 
    sendCmd(0xb1+row);	
    sendCmd(0x10+(8*col/16));		
    sendCmd(0x00+(8*col%16));
    for(i=0;i<16;i++) sendData(put[X++]);
    sendCmd(0xb0+row);	 
    col=col+2; 
  }
}

/*****************
 * 24*24字符，取模顺序是列行式，
 * 从上到下，高位在前，从左到右；
 * 先选择页地址0-7，再选择列0-127
 * 页码是直接读取8位数据作为地址；
 * 列是先读取高四位，后读取低四位；
 **********************/
void Lcd12864::render24x24(unsigned char row,unsigned char col,unsigned char count,unsigned char const *put)
{		
  unsigned int X=0;
 unsigned int i=0,j=0;
  sendCmd(0xb0+row);
  sendCmd(0x10+(8*col/16));		
  sendCmd(0x00+(8*col%16));
  for(j=0;j<count;j++)
  { 
    for(i=0;i<24;i++) sendData(put[X++]); 
    sendCmd(0xb1+row);	
    sendCmd(0x10+(8*col/16));		
    sendCmd(0x00+(8*col%16));
    for(i=0;i<24;i++) sendData(put[X++]);
    sendCmd(0xb2+row);	
    sendCmd(0x10+(8*col/16));		
    sendCmd(0x00+(8*col%16));
    for(i=0;i<24;i++) sendData(put[X++]);
    sendCmd(0xb0+row);
    col=col+3; 
  }
}

/*****************
 * 图片；取模顺序是列行式，
 * 从上到下，低在前，从左到右；
 * 先选择页地址0-7，再选择列0-127
 * 页码是直接读取8位数据作为地址；
 * 列是先读取高四位，后读取低四位；
 **********************/
void Lcd12864::renderBmp(unsigned char const *put)
{		
  unsigned int X=0;
  unsigned int i=0,j=0;
  for(j=0;j<8;j++)
  {
    sendCmd(0xb0+j); 
    sendCmd(0x10);		
    sendCmd(0x00);
    for(i=0;i<128;i++) sendData(put[X++]); 
  }	
}

/*****************
 * 图片反显；取模顺序是列行式，
 * 从上到下，位在前，从左到右；
 * 先选择页地址0-7，再选择列0-127
 * 页码是直接读取8位数据作为地址；
 * 列是先读取高四位，后读取低四位；
 **********************/
void Lcd12864::renderReversedBmp(unsigned char const *put)
{		
  unsigned int X=0;
 unsigned int i=0,j=0;
  for(j=0;j<8;j++)
  {
    sendCmd(0xb0+j); 
    sendCmd(0x10);		
    sendCmd(0x00);
    for(i=0;i<128;i++) sendData(~put[X++]); 
  }	
}

/*****************
 * 清屏；取模顺序是列行式，
 * 从上到下，低位在前，从左到右；
 * 先选择页地址0-7，再选择列0-127
 * 页码是直接读取8位数据作为地址；
 * 列是先读取高四位，后读取低四位；
 **********************/
void Lcd12864::clear()
{	 
  unsigned char x,y;
  for(y=0;y<8;y++)
  {    
    sendCmd(0xb0+y);
    sendCmd(0x10);		
    sendCmd(0x00);
    for(x=0;x<132;x++)  sendData(0); 
  }	
}	 
