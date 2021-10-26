#include "race.hpp"

using namespace blit;

void init() {
    set_screen_mode(ScreenMode::lores);
}

Point seg[] = { 
    Point(8, 28), Point(92, 28), Point(92, 88), Point(32, 88), Point(32, 68), Point(72, 68),
    Point(72, 48), Point(12, 48), Point(12, 108), Point(112, 108), Point(112, 28), Point(148, 28), 
    Point(148, 112), 
};
const Pen track_color(180, 180, 180);
Rect seg_rect(Point s1, Point s2) {
    return Rect(
        std::min(s1.x, s2.x)-8, 
        std::min(s1.y, s2.y)-8, 
        abs(s2.x-s1.x)+16, 
        abs(s2.y-s1.y)+16
    );
}
bool isZero(Point p) {
    return p.x == 0 && p.y == 0;
}
void render_track() {
    screen.pen = track_color;
    for(int i=0; i<(int)std::size(seg)-1; i++) {
        screen.rectangle(seg_rect(seg[i], seg[i+1]));
    }
    screen.pen = Pen(255, 255, 255);
    screen.rectangle(Rect(3, 20, 1, 16)); // start line
    screen.rectangle(Rect(140, 117, 16, 2)); // finnish line
    screen.pen = Pen(0, 0, 0);
    for(int i=0; i<8; i++) {
        screen.pixel(Point(140+i*2, 117));
        screen.pixel(Point(140+i*2+1, 118));
    }
}

enum Alignment { V, H };
void render_car(Point loc, Pen color, Alignment a=H) {
    screen.pen = Pen(0, 0, 0);
    screen.pixel(Point(loc.x-1, loc.y-1));
    screen.pixel(Point(loc.x-1, loc.y+1));
    screen.pixel(Point(loc.x+1, loc.y-1));
    screen.pixel(Point(loc.x+1, loc.y+1));
    screen.pen = color;
    if(a == V) {
        screen.line(Point(loc.x, loc.y-1), Point(loc.x, loc.y+1));
    } else {
        screen.line(Point(loc.x-1, loc.y), Point(loc.x+1, loc.y));
    }
}

uint8_t active = 0;
Player player[] = { 
    {Point(2, 24), Point(0, 0), Point(0, 0), Pen(230, 50, 50), 0}, 
    {Point(2, 31), Point(0, 0), Point(0, 0), Pen(60, 60, 250), 0} 
};
bool check_target(Point p) {
    for(int i=0; i<(int)std::size(player); i++) {
        if(i != active) {
            if(Rect(player[i].loc.x-1, player[i].loc.y-1, 3, 3).contains(p)) {
                return false;
            }
        }
    }
    uint8_t active_seg = player[active].active_seg;
    return 
        seg_rect(seg[active_seg], seg[active_seg+1]).contains(p)
        || seg_rect(seg[active_seg+1], seg[active_seg+2]).contains(p);
}

Point dir[] = { 
    // N          NE            E             SE
    Point(0, -3), Point(3, -3), Point(3,  0), Point(3, 3),
    // S          SW            W             NW
    Point(0,  3), Point(-3, 3), Point(-3, 0), Point(-3, -3), Point(0, 0)};
void render_target() {
    screen.alpha = 60;
    screen.pen = player[active].color;
    Point target(
        player[active].loc.x+player[active].vec.x,
        player[active].loc.y+player[active].vec.y
    );
    for(int i=0; i<(int)std::size(dir); i++) {
        if(check_target(Point(target.x+dir[i].x, target.y+dir[i].y))) {
            screen.rectangle(Rect(target.x+dir[i].x-1, target.y+dir[i].y-1, 3, 3));
        }
    }
    screen.alpha = 255;
}

void render_player() {
    for(int i=0; i<(int)std::size(player); i++) {
        render_car(player[i].loc, player[i].color);
        screen.alpha = 80;
        render_car(player[i].loc + player[i].vec + player[i].dir, player[i].color);
        screen.alpha = 255;
    }
    render_target();
}

///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of your game
//
void render(uint32_t time) {

    // reset the screen alpha and clipping mask
    screen.alpha = 255;
    screen.mask = nullptr;

    // clear the screen -- screen is a reference to the frame buffer and can be used to draw all things with the 32blit
    screen.pen = Pen(0, 120, 0);
    screen.clear();

    // draw a white rectangle
    screen.pen = Pen(255, 255, 255);
    screen.rectangle(Rect(0, 0, 160, 14));

    // draw some black text
    screen.pen = Pen(0, 0, 0);
    screen.text("square race", minimal_font, Point(5, 4));
    screen.pen = Pen(90, 90, 90);
    screen.text("track: snake", minimal_font, Point(95, 4));

    render_track();
    render_player();
}

///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the 
// amount if milliseconds elapsed since the start of your game
//
bool move = false;
void update(uint32_t time) {
    if (buttons.state & Button::DPAD_RIGHT) {
        if(check_target(player[active].loc + player[active].vec + Point(3, player[active].dir.y))) {
            player[active].dir.x = 3;
            move = true;
        }
    }
    if (buttons.state & Button::DPAD_DOWN) {
        if(check_target(player[active].loc + player[active].vec + Point(player[active].dir.x, 3))) {
            player[active].dir.y = 3;
            move = true;
        }
    }
    if (buttons.state & Button::DPAD_LEFT) {
        if(check_target(player[active].loc + player[active].vec + Point(-3, player[active].dir.y))) {
            player[active].dir.x = -3;
            move = true;
        }
    }
    if (buttons.state & Button::DPAD_UP) {
        if(check_target(player[active].loc + player[active].vec + Point(player[active].dir.x, -3))) {
            player[active].dir.y = -3;
            move = true;
        }
    }
    if (buttons.state & Button::A) {
        player[active].dir = Point(0, 0);
        move = true;
    }
    if ((buttons.state == 0) && (move)) {
        move = false;
        player[active].vec = Point(
            player[active].vec.x+player[active].dir.x, 
            player[active].vec.y+player[active].dir.y);
        player[active].loc = Point(
            player[active].loc.x+player[active].vec.x, 
            player[active].loc.y+player[active].vec.y);
        player[active].dir = Point(0, 0);
        active = (active + 1) % std::size(player);
    }
    if (buttons.state & Button::A) {
    }
}