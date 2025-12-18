/**
 * @file HeartbeatAsciiLogo.cpp
 * @brief Implementation of the pulsating heartbeat effect.
 */

#include "HeartbeatAsciiLogo.hpp"
#include <cmath>
#include <algorithm>

HeartbeatAsciiLogo::HeartbeatAsciiLogo(const std::vector<std::string>& art_data) : AsciiLogo(art_data) {
    // Initial State
    state = HeartbeatState::EXPANDING;
    radius = 0.0;
    speed = 1.5; 
    
    // Initial Colors
    current_bg_hue = 0.0;   // Start Red
    pulse_hue = 120.0;      // Pulse Green
    next_bg_hue = 240.0;    // Next Blue
}

// get_width and get_height are now in AsciiLogo

void HeartbeatAsciiLogo::init_position(int scr_height, int scr_width) {
    x = (scr_width - width) / 2;
    y = (scr_height - height) / 2;
    
    // Calculate max radius precisely based on the furthest visible character
    max_radius = 0.0;
    double center_x = width / 2.0;
    double center_y = height / 2.0;

    for (int i = 0; i < height; ++i) {
        const std::string& line = lines[i];
        for (int j = 0; j < (int)line.length(); ++j) {
            if (line[j] == ' ') continue;

            double dy = (i - center_y) * 2.0; 
            double dx = (j - center_x);
            double dist = std::sqrt(dx*dx + dy*dy);
            
            if (dist > max_radius) {
                max_radius = dist;
            }
        }
    }
    // Add a tiny buffer to ensure full coverage before retracting
    max_radius += 1.0;
}

void HeartbeatAsciiLogo::update(int /*scr_height*/, int /*scr_width*/) {
    if (state == HeartbeatState::EXPANDING) {
        radius += speed;
        if (radius >= max_radius) {
            state = HeartbeatState::RETRACTING;
            // Reveal a new background color on next retraction
            next_bg_hue = (int)(next_bg_hue + 60) % 360; 
        }
    } else {
        radius -= speed;
        if (radius <= 0) {
            radius = 0;
            state = HeartbeatState::EXPANDING;
            
            // Cycle colors:
            // 1. Fully retracted -> Previous "Next BG" becomes "Current BG"
            // 2. Pick a new pulse color for the upcoming expansion
            current_bg_hue = next_bg_hue;
            pulse_hue = (int)(pulse_hue + 40) % 360;
        }
    }
    // Regenerate art if generator is attached (Dynamic Noise)
}

void HeartbeatAsciiLogo::draw() {
    double center_x = width / 2.0;
    double center_y = height / 2.0;
    
    int bg_color = get_color_from_hue(current_bg_hue);
    int pulse_color = get_color_from_hue(pulse_hue);
    int next_bg_color = get_color_from_hue(next_bg_hue);

    attron(A_BOLD);
    
    for (int i = 0; i < height; ++i) {
         const std::string& line = lines[i];
         for (int j = 0; j < (int)line.length(); ++j) {
             
             // Radial Distance (Aspect Corrected)
             double dy = (i - center_y) * 2.0; 
             double dx = (j - center_x);
             double dist = std::sqrt(dx*dx + dy*dy);
             
             int final_color = 0;
             
             if (state == HeartbeatState::EXPANDING) {
                 // Inner circle is Pulse, Outer is BS
                 if (dist < radius) {
                     final_color = pulse_color;
                 } else {
                     final_color = bg_color;
                 }
             } else { // RETRACTING
                 // Inner circle is Pulse, Outer is NEXT BG
                 if (dist < radius) {
                     final_color = pulse_color;
                 } else {
                     final_color = next_bg_color;
                 }
             }
 
             // Dynamic Noise Logic: Regenerate cell if color changed
             if (last_colors[i][j] != final_color) {
                 last_colors[i][j] = final_color;
                 if (cell_generator) {
                     lines[i][j] = cell_generator(i, j);
                 }
             }

             char c = lines[i][j];
             if (c == ' ') continue; 

             attron(COLOR_PAIR(final_color));
             
             wchar_t wc = (wchar_t)c;
             wchar_t wstr[2] = {wc, 0};
             mvaddwstr(y + i, x + j, wstr);
             
             attroff(COLOR_PAIR(final_color));
         }
    }
    
    attroff(A_BOLD);
}
