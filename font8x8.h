#ifndef _Font8x8_H_
#define _Font8x8_H_

#include <Arduino.h>
#include <font.h>

class Font8x8 : public Font {
public:
	Font8x8();
    virtual byte* getData(char c);
};

extern Font8x8 FONT8x8;

#endif
