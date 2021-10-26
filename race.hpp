#include "32blit.hpp"

using namespace blit;

struct Player {
    Point loc;
    Point dir;
    Point vec;
    Pen color;
    uint8_t active_seg;
};
