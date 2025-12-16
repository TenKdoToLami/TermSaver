#include "Logo.hpp"
#include <cmath>
#include <cstdlib>

Logo::Logo() {
    x = 0; y = 0;
    dx = 1; dy = 1;
    max_colors = 255; 
    current_pair = 1;
}

Logo::~Logo() {}

void Logo::pick_new_color() {
    // Select a new random color from the palette (1-255)
    current_pair = (rand() % max_colors) + 1;
}

void Logo::on_bounce() {
    pick_new_color();
}

void Logo::init_position(int scr_height, int scr_width) {
    int w = get_width();
    int h = get_height();
    
    // Start centered
    x = (scr_width - w) / 2;
    y = (scr_height - h) / 2;
    
    // Randomize initial direction
    dx = (rand() % 2 == 0) ? 1 : -1;
    dy = (rand() % 2 == 0) ? 1 : -1;
    
    pick_new_color();
}

void Logo::update(int scr_height, int scr_width) {
    int w = get_width();
    int h = get_height();

    x += dx;
    y += dy;

    // Bounce X
    if (x <= 0 || x + w >= scr_width) {
        dx = -dx;
        // Clamp to screen
        if (x < 0) x = 0;
        if (x + w > scr_width) x = scr_width - w; // Fix: was scr_width - width
        on_bounce();
    }

    // Bounce Y
    if (y <= 0 || y + h >= scr_height) {
        dy = -dy;
        // Clamp
        if (y < 0) y = 0;
        if (y + h > scr_height) y = scr_height - h;
        on_bounce();
    }
}

int Logo::get_color_from_hue(double h) {
    double s = 1.0, v = 1.0;
    double c = v * s;
    double x = c * (1 - std::abs(std::fmod(h / 60.0, 2) - 1));
    double m = v - c;
    double r=0, g=0, b=0;
    
    if (h < 60) { r=c; g=x; b=0; }
    else if (h < 120) { r=x; g=c; b=0; }
    else if (h < 180) { r=0; g=c; b=x; }
    else if (h < 240) { r=0; g=x; b=c; }
    else if (h < 300) { r=x; g=0; b=c; }
    else { r=c; g=0; b=x; }
    
    int ri = (int)((r + m) * 5);
    int gi = (int)((g + m) * 5);
    int bi = (int)((b + m) * 5);
    
    return 16 + (36 * ri) + (6 * gi) + bi;
}
