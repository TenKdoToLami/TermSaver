/**
 * @file RippleAsciiLogo.cpp
 * @brief Implementation of the ripple color effect.
 */

#include "RippleAsciiLogo.hpp"
#include <cmath>

RippleAsciiLogo::RippleAsciiLogo(const std::vector<std::string>& art_data) : lines(art_data), current_hue(0.0) {
    height = lines.size();
    width = 0;
    for (const auto& line : lines) {
        if ((int)line.length() > width) {
            width = line.length();
        }
    }
}

int RippleAsciiLogo::get_width() const { return width; }
int RippleAsciiLogo::get_height() const { return height; }

void RippleAsciiLogo::init_position(int scr_height, int scr_width) {
    // Center exactly once
    x = (scr_width - width) / 2;
    y = (scr_height - height) / 2;
    dx = 0; // Static
    dy = 0; // Static
}

void RippleAsciiLogo::update(int /*scr_height*/, int /*scr_width*/) {
    // Just cycle the hue
    current_hue += 2.0; // Slow breath
    if (current_hue >= 360.0) current_hue -= 360.0;
}

void RippleAsciiLogo::draw() {
    double center_x = width / 2.0;
    double center_y = height / 2.0;

    attron(A_BOLD);
    
    for (int i = 0; i < height; ++i) {
         const std::string& line = lines[i];
         for (int j = 0; j < (int)line.length(); ++j) {
             char c = line[j];
             if (c == ' ') continue; // Transparency
             
             // Calculate Radial Distance
             // Multiply Y difference by 2.0 to correct for typical terminal cell aspect ratio (tall)
             // making the gradient look more circular than elliptical
             double dy = (i - center_y) * 2.0; 
             double dx = (j - center_x);
             double dist = std::sqrt(dx*dx + dy*dy);
             
             // Hue based on base time (current_hue) minus distance (ripple out)
             double local_hue = current_hue - (dist * 5.0); 
             // Wrap hue
             while (local_hue < 0) local_hue += 360.0;
             while (local_hue >= 360.0) local_hue -= 360.0;

             int color_index = get_color_from_hue(local_hue);

             attron(COLOR_PAIR(color_index));
             
             wchar_t wc = (wchar_t)c;
             wchar_t wstr[2] = {wc, 0};
             mvaddwstr(y + i, x + j, wstr);
             
             attroff(COLOR_PAIR(color_index));
         }
    }
    
    attroff(A_BOLD);
}
