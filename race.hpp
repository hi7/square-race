#include "32blit.hpp"

using namespace blit;

struct Player {
    Point loc;
    Point dir;
    Pen color;
    uint8_t active_seg;
};
