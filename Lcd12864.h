#ifndef LCD12864_H_
#define LCD12864_H_

#include <Arduino.h>

#if PHYSICAL_SPI
#include <SPI.h>
#endif

class Lcd12864
{
public:
    Lcd12864(byte pBgLight, byte pMOSI, byte pCLK, byte pAO, byte pREST, byte pCS);  //构造函数 
	void reset();
	
    void setup();
	void switchBgLight(boolean value);

    void render8x8(unsigned char row,unsigned char col,unsigned char count,unsigned char const *put);
    void render8x16(unsigned char row,unsigned char col,unsigned char count,unsigned char const *put);
    void render16x16(unsigned char row,unsigned char col,unsigned char count,unsigned char const *put);
    void render24x24(unsigned char row,unsigned char col,unsigned char count,unsigned char const *put);
    
	void renderBmp(unsigned char const *put);
    void renderReversedBmp(unsigned char const *put);
    
	void clear();
protected:
	void sendByte(unsigned char Dbyte);
    void sendCmd(unsigned char Cbyte);
    void sendData(unsigned char Dbyte);
private:
	byte pinBGLight;
	byte pinMOSI;
	byte pinCLK;
	byte pinAO;
	byte pinREST;
	byte pinCS;
};

#endif