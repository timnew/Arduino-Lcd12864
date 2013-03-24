#ifndef _Font8x16_H_
#define _Font8x16_H_

#include <Arduino.h>
#include <font.h>

class Font8x16 : public Font {
public:
	Font8x16();
    virtual byte* getData(char c);
};

extern Font8x16 FONT8x16;

#endif
