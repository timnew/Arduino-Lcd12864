#ifndef _Font_H_
#define _Font_H_

#include <Arduino.h>

class Font {
public:
	byte width;
	byte height;
	byte heightInBytes;
	byte size;
	virtual byte* getData(char c) = 0; 
protected:
	inline Font(byte width, byte height)  {
		this->width = width;
		this->height = width;
	
		this->heightInBytes = height / 8;
		this->size = this->heightInBytes * width;
	}
};
#endif
