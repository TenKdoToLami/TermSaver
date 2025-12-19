/**
 * @file TypewriterAsciiLogo.cpp
 * @brief Implementation of the directional typewriter effect.
 */

#include "TypewriterAsciiLogo.hpp"
#include <ncurses.h>
#include <algorithm>

TypewriterAsciiLogo::TypewriterAsciiLogo(const std::vector<std::string>& art_data, int dir) 
    : AsciiLogo(art_data), cursor_r(0), cursor_c(0), direction(dir), base_hue(0.0) {
    cell_colors.resize(height, std::vector<int>(width, -1));
    
    switch(direction) {
        case 0: cursor_r = 0; cursor_c = 0; break;           // Top-Left
        case 1: cursor_r = 0; cursor_c = 0; break;           // Top
        case 2: cursor_r = 0; cursor_c = width - 1; break;   // Top-Right
        case 3: cursor_r = 0; cursor_c = width - 1; break;   // Right
        case 4: cursor_r = height - 1; cursor_c = width - 1; break; // Bottom-Right
        case 5: cursor_r = height - 1; cursor_c = 0; break;  // Bottom
        case 6: cursor_r = height - 1; cursor_c = 0; break;  // Bottom-Left
        case 7: cursor_r = 0; cursor_c = 0; break;           // Left
    }
}

void TypewriterAsciiLogo::init_position(int scr_height, int scr_width) {
    x = (scr_width - width) / 2;
    y = (scr_height - height) / 2;
    dx = 0;
    dy = 0;
}

void TypewriterAsciiLogo::update(int /*scr_height*/, int /*scr_width*/) {
    if (cursor_r >= 0 && cursor_r < height && cursor_c >= 0 && cursor_c < width) {
        cell_colors[cursor_r][cursor_c] = get_color_from_hue(base_hue);
    }
    
    bool wrapped = false;
    
    do {
        switch(direction) {
            // Top-Left: diagonal bands from top-left corner
            case 0: {
                int max_diag = height + width - 2;
                int current_diag = cursor_r + cursor_c;
                // Move down-left along current diagonal
                if (cursor_r < height - 1 && cursor_c > 0) {
                    cursor_r++;
                    cursor_c--;
                } else {
                    // Move to start of next diagonal
                    current_diag++;
                    if (current_diag > max_diag) {
                        wrapped = true;
                        cursor_r = 0;
                        cursor_c = 0;
                    } else {
                        // Start of next diagonal
                        if (current_diag < width) {
                            cursor_r = 0;
                            cursor_c = current_diag;
                        } else {
                            cursor_r = current_diag - width + 1;
                            cursor_c = width - 1;
                        }
                    }
                }
                break;
            }
            // Top: row-by-row from top
            case 1: {
                cursor_c++;
                if (cursor_c >= width) {
                    cursor_c = 0;
                    cursor_r++;
                }
                if (cursor_r >= height) {
                    cursor_r = 0;
                    cursor_c = 0;
                    wrapped = true;
                }
                break;
            }
            // Top-Right: diagonal bands from top-right corner
            case 2: {
                int max_diag = height + width - 2;
                int mirrored_c = width - 1 - cursor_c;
                int current_diag = cursor_r + mirrored_c;
                // Move down-right along current diagonal
                if (cursor_r < height - 1 && cursor_c < width - 1) {
                    cursor_r++;
                    cursor_c++;
                } else {
                    // Move to start of next diagonal
                    current_diag++;
                    if (current_diag > max_diag) {
                        wrapped = true;
                        cursor_r = 0;
                        cursor_c = width - 1;
                    } else {
                        if (current_diag < width) {
                            cursor_r = 0;
                            cursor_c = width - 1 - current_diag;
                        } else {
                            cursor_r = current_diag - width + 1;
                            cursor_c = 0;
                        }
                    }
                }
                break;
            }
            // Right: column-by-column from right
            case 3: {
                cursor_r++;
                if (cursor_r >= height) {
                    cursor_r = 0;
                    cursor_c--;
                }
                if (cursor_c < 0) {
                    cursor_c = width - 1;
                    cursor_r = 0;
                    wrapped = true;
                }
                break;
            }
            // Bottom-Right: diagonal bands from bottom-right corner
            case 4: {
                int max_diag = height + width - 2;
                int rev_r = height - 1 - cursor_r;
                int rev_c = width - 1 - cursor_c;
                int current_diag = rev_r + rev_c;
                // Move up-right along current diagonal
                if (cursor_r > 0 && cursor_c < width - 1) {
                    cursor_r--;
                    cursor_c++;
                } else {
                    // Move to start of next diagonal
                    current_diag++;
                    if (current_diag > max_diag) {
                        wrapped = true;
                        cursor_r = height - 1;
                        cursor_c = width - 1;
                    } else {
                        if (current_diag < width) {
                            cursor_r = height - 1;
                            cursor_c = width - 1 - current_diag;
                        } else {
                            cursor_r = height - 1 - (current_diag - width + 1);
                            cursor_c = 0;
                        }
                    }
                }
                break;
            }
            // Bottom: row-by-row from bottom
            case 5: {
                cursor_c++;
                if (cursor_c >= width) {
                    cursor_c = 0;
                    cursor_r--;
                }
                if (cursor_r < 0) {
                    cursor_r = height - 1;
                    cursor_c = 0;
                    wrapped = true;
                }
                break;
            }
            // Bottom-Left: diagonal bands from bottom-left corner
            case 6: {
                int max_diag = height + width - 2;
                int rev_r = height - 1 - cursor_r;
                int current_diag = rev_r + cursor_c;
                // Move up-left along current diagonal
                if (cursor_r > 0 && cursor_c > 0) {
                    cursor_r--;
                    cursor_c--;
                } else {
                    // Move to start of next diagonal
                    current_diag++;
                    if (current_diag > max_diag) {
                        wrapped = true;
                        cursor_r = height - 1;
                        cursor_c = 0;
                    } else {
                        if (current_diag < width) {
                            cursor_r = height - 1;
                            cursor_c = current_diag;
                        } else {
                            cursor_r = height - 1 - (current_diag - width + 1);
                            cursor_c = width - 1;
                        }
                    }
                }
                break;
            }
            // Left: column-by-column from left
            case 7: {
                cursor_r++;
                if (cursor_r >= height) {
                    cursor_r = 0;
                    cursor_c++;
                }
                if (cursor_c >= width) {
                    cursor_c = 0;
                    cursor_r = 0;
                    wrapped = true;
                }
                break;
            }
        }
        
        if (wrapped) break;
        
        char current_char = ' ';
        if (cursor_r >= 0 && cursor_r < (int)lines.size() && 
            cursor_c >= 0 && cursor_c < (int)lines[cursor_r].length()) {
            current_char = lines[cursor_r][cursor_c];
        }
        
        if (cell_generator || current_char != ' ') {
            break;
        }
        
    } while (true);
    
    if (wrapped) {
        base_hue += 45.0;
        if (base_hue >= 360.0) base_hue -= 360.0;
    }
}

void TypewriterAsciiLogo::draw() {
    attron(A_BOLD);
    
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            bool is_cursor = (i == cursor_r && j == cursor_c);
            
            int color_idx;
            if (is_cursor) {
                color_idx = 7;
            } else if (cell_colors[i][j] != -1) {
                color_idx = cell_colors[i][j];
            } else {
                color_idx = 0;
            }

            if (i < (int)last_colors.size() && j < (int)last_colors[i].size() && last_colors[i][j] != color_idx) {
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
