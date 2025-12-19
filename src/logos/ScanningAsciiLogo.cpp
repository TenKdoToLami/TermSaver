/**
 * @file ScanningAsciiLogo.cpp
 * @brief Implementation of the multidirectional scanning color effect.
 */

#include "ScanningAsciiLogo.hpp"
#include <ncurses.h>

ScanningAsciiLogo::ScanningAsciiLogo(const std::vector<std::string>& art_data, int dir) 
    : AsciiLogo(art_data), hue(0.0), direction(dir) {
}

void ScanningAsciiLogo::init_position(int scr_height, int scr_width) {
    x = (scr_width - width) / 2;
    y = (scr_height - height) / 2;
    dx = 0; // Static effect
    dy = 0; 
}

void ScanningAsciiLogo::update(int /*scr_height*/, int /*scr_width*/) {
    // Cycle the base hue.
    // Subtracting makes the wave flow 'outward' from the origin.
    hue -= 6.0; 
    if (hue < 0) hue += 360.0;
}

void ScanningAsciiLogo::draw() {
    attron(A_BOLD);
    
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // Calculate spatial metric based on the selected direction
            double cell_metric = 0.0;
            switch(direction) {
                case 0: cell_metric = (double)(i + j); break;                          // Top-Left -> Bottom-Right
                case 1: cell_metric = (double)i; break;                                // Top -> Bottom
                case 2: cell_metric = (double)(i + (width - 1 - j)); break;           // Top-Right -> Bottom-Left
                case 3: cell_metric = (double)(width - 1 - j); break;                  // Right -> Left
                case 4: cell_metric = (double)((height - 1 - i) + (width - 1 - j)); break; // Bottom-Right -> Top-Left
                case 5: cell_metric = (double)(height - 1 - i); break;                 // Bottom -> Top
                case 6: cell_metric = (double)((height - 1 - i) + j); break;           // Bottom-Left -> Top-Right
                case 7: cell_metric = (double)j; break;                                // Left -> Right
                default: cell_metric = (double)(i + j); break;
            }

            // Calculate local hue based on time (current_hue) and space (cell_metric)
            double local_hue = hue + (cell_metric * 8.0);
            while (local_hue >= 360.0) local_hue -= 360.0;
            while (local_hue < 0) local_hue += 360.0;

            int color_idx = get_color_from_hue(local_hue);

            // Handle Dynamic Noise/Fading Art
            // We update the character BEFORE the visibility check to fix fading issues.
            bool valid_bounds = (i < (int)last_colors.size() && j < (int)last_colors[i].size());
            if (valid_bounds && last_colors[i][j] != color_idx) {
                last_colors[i][j] = color_idx;
                if (cell_generator) {
                    lines[i][j] = cell_generator(i, j);
                }
            }

            char c = (i < (int)lines.size() && j < (int)lines[i].length()) ? lines[i][j] : ' ';
            if (c == ' ') continue; 

            attron(COLOR_PAIR(color_idx));

            wchar_t wc = (wchar_t)c;
            wchar_t wstr[2] = {wc, 0};
            mvaddwstr(y + i, x + j, wstr);

            attroff(COLOR_PAIR(color_idx));
        }
    }
    
    attroff(A_BOLD);
}
