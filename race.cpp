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
void render_track() {
    screen.pen = track_color;
    for(int i=0; i<(int)std::size(seg)-1; i++) {
        screen.rectangle(Rect(
            std::min(seg[i].x, seg[i+1].x)-8, 
            std::min(seg[i].y, seg[i+1].y)-8, 
            abs(seg[i+1].x-seg[i].x)+16, 
            abs(seg[i+1].y-seg[i].y)+16)
        );
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
void render_car(Player p, Alignment a=H) {
    screen.pen = Pen(0, 0, 0);
    screen.pixel(Point(p.loc.x-1, p.loc.y-1));
    screen.pixel(Point(p.loc.x-1, p.loc.y+1));
    screen.pixel(Point(p.loc.x+1, p.loc.y-1));
    screen.pixel(Point(p.loc.x+1, p.loc.y+1));
    screen.pen = p.color;
    if(a == V) {
        screen.line(Point(p.loc.x, p.loc.y-1), Point(p.loc.x, p.loc.y+1));
    } else {
        screen.line(Point(p.loc.x-1, p.loc.y), Point(p.loc.x+1, p.loc.y));
    }
}

Player player[] = { {Point(2, 24), Pen(230, 50, 50)}, {Point(2, 31), Pen(60, 60, 250)} };
void render_player() {
    for(int i=0; i<(int)std::size(player); i++) {
        render_car(player[i]);
    }
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
void update(uint32_t time) {
}