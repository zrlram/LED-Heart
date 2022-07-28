#include <heart.h>

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  i = xy[y][x] - 1;

  return i;
}