/**
 * @file RotatingLineLogo.cpp
 * @brief Implementation of the radar/sonar sweep effect.
 */

#include "RotatingLineLogo.hpp"

RotatingLineLogo::RotatingLineLogo(const std::vector<std::string>& art_data) 
    : AsciiLogo(art_data), angle(0.0), current_brush_hue(0.0), initialized(false) {}

std::vector<RotatingLineLogo::Point> RotatingLineLogo::get_line_points(int x1, int y1, int x2, int y2) {
    std::vector<Point> pts;
    int dx = std::abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -std::abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        pts.push_back({x1, y1});
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
    return pts;
}

void RotatingLineLogo::update(int scr_height, int scr_width) {
    // 0. Update Center Position for Art
    // Center the underlying AsciiLogo
    x = (scr_width - width) / 2;
    y = (scr_height - height) / 2;
    if (x < 0) x = 0;
    if (y < 0) y = 0;

    // 0.5 Initialize Grid if needed
    if (!initialized || (int)cell_colors.size() != height || (int)cell_colors[0].size() != width) {
        cell_colors.assign(height, std::vector<int>(width, get_color_from_hue(current_brush_hue))); 
        // Initialize cells with a default starting color
        int initial_color = get_color_from_hue(200); // Start with Blue-ish
 
        for(auto& row : cell_colors) std::fill(row.begin(), row.end(), initial_color);
        
        initialized = true;
    }

    // 1. Calculate center and radius for Sweep (center of screen)
    int cx = scr_width / 2;
    int cy = scr_height / 2;
    
    int half_w = scr_width / 2;
    int half_h = scr_height / 2;
    int radius = (int)std::ceil(std::sqrt(half_w*half_w + half_h*half_h)) + 5;
    
    // 2. Interpolate rays to fill gaps between large angle steps
    // Increased multiplier to 4.0 to prevent gaps at terminal edges
    int steps = (int)std::ceil(radius * angle_speed * 4.0);
    if (steps < 1) steps = 1;

    for (int i = 1; i <= steps; ++i) {
        double t = (double)i / steps;
        double current_theta = angle + (angle_speed * t);
        
        int x1 = cx + (int)std::round(cos(current_theta) * radius);
        int y1 = cy + (int)std::round(sin(current_theta) * radius);
        int x2 = cx - (int)std::round(cos(current_theta) * radius); 
        int y2 = cy - (int)std::round(sin(current_theta) * radius);

        std::vector<Point> ray_pts = get_line_points(x1, y1, x2, y2);
        
        // PAINT Logic
        int brush_color = get_color_from_hue(current_brush_hue);
        for (const auto& p : ray_pts) {
             int local_r = p.y - y;
             int local_c = p.x - x;
             
             if (local_r >= 0 && local_r < height && local_c >= 0 && local_c < width) {
                 // Update persistent color grid
                 cell_colors[local_r][local_c] = brush_color;
                 
                 // Update content if dynamic (ONLY on touch)
                 if (cell_generator) {
                     char new_c = cell_generator(local_r, local_c);
                     // Ensure lines matches dimensions (it should)
                     // Safely update the character in the buffer
                     if (local_r < (int)lines.size() && local_c < (int)lines[local_r].length()) {
                         lines[local_r][local_c] = new_c;
                     }
                 }
             }
        }
    }
    
    // 4. Update Physics
    double old_angle = angle;
    angle += angle_speed;
    
    // Check for Half Rotation (PI) crossing
    if (old_angle < 3.14159265359 && angle >= 3.14159265359) {
        current_brush_hue += 45.0; 
        if (current_brush_hue >= 360.0) current_brush_hue -= 360.0;
    }

    // Check for Full Rotation (2 PI)
    if (angle >= 6.28318530718) { // 2*PI
        angle -= 6.28318530718;
        // Cycle the Brush Hue again at 360
        current_brush_hue += 45.0; 
        if (current_brush_hue >= 360.0) current_brush_hue -= 360.0;
    }
}

void RotatingLineLogo::draw() {
    // 1. Draw Underlying Art using Persistent Colors
    for (int i = 0; i < height; ++i) {
         if (i >= (int)lines.size()) break;
         const std::string& line = lines[i];
         for (int j = 0; j < (int)line.length(); ++j) {
             char c = line[j];
             if (c == ' ') continue;
             int color = cell_colors[i][j];
             attron(COLOR_PAIR(color));
             attron(A_BOLD); 
             
             wchar_t wc = (wchar_t)c;
             wchar_t wstr[2] = {wc, 0};
             mvaddwstr(y + i, x + j, wstr);
             
             attroff(A_BOLD);
             attroff(COLOR_PAIR(color));
         }
    }

    // No Overlay drawn. The "line" is invisible, it just paints.
}
