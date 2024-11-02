#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

class Color {
public:
    uint8_t r, g, b;
    Color(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0) : r(red), g(green), b(blue) {}
};

#endif
