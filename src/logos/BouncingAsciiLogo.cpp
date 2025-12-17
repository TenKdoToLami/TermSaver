/**
 * @file BouncingAsciiLogo.cpp
 * @brief Implementation of the DVD-screensaver style bouncing logo.
 */

#include "BouncingAsciiLogo.hpp"

BouncingAsciiLogo::BouncingAsciiLogo(const std::vector<std::string>& art_data) : lines(art_data) {
    height = lines.size();
    width = 0;
    for (const auto& line : lines) {
        if ((int)line.length() > width) {
            width = line.length();
        }
    }
}

int BouncingAsciiLogo::get_width() const { return width; }
int BouncingAsciiLogo::get_height() const { return height; }

void BouncingAsciiLogo::draw() {
    attron(COLOR_PAIR(current_pair)); 
    attron(A_BOLD);
    for (int i = 0; i < height; ++i) {
         const std::string& line = lines[i];
         for (int j = 0; j < (int)line.length(); ++j) {
             char c = line[j];
             if (c == ' ') continue; // Transparency
             
             wchar_t wc = (wchar_t)c;
             wchar_t wstr[2] = {wc, 0};
             mvaddwstr(y + i, x + j, wstr);
         }
    }
    attroff(A_BOLD);
    attroff(COLOR_PAIR(current_pair));
}
