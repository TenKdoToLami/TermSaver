#include "RotatingLineLogo.hpp"

RotatingLineLogo::RotatingLineLogo() : angle(0.0), current_hue(0.0) {}

int RotatingLineLogo::get_width() const { return 0; }
int RotatingLineLogo::get_height() const { return 0; }

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
    // 1. Calculate geometry params
    int cx = scr_width / 2;
    int cy = scr_height / 2;
    
    // Use Diagonal distance to ensure we cover corners
    int half_w = scr_width / 2;
    int half_h = scr_height / 2;
    int radius = (int)std::ceil(std::sqrt(half_w*half_w + half_h*half_h)) + 5;
    
    // 2. Determine number of intermediate rays needed to fill gaps
    int steps = (int)std::ceil(radius * angle_speed * 2.0);
    if (steps < 1) steps = 1;

    std::vector<Point> frame_points;

    // 3. Interpolate rays
    for (int i = 1; i <= steps; ++i) {
        double t = (double)i / steps;
        double current_theta = angle + (angle_speed * t);
        
        // Endpoints
        int x1 = cx + (int)(cos(current_theta) * radius);
        int y1 = cy + (int)(sin(current_theta) * radius);
        int x2 = cx - (int)(cos(current_theta) * radius); 
        int y2 = cy - (int)(sin(current_theta) * radius);

        // Generate points
        std::vector<Point> ray_pts = get_line_points(x1, y1, x2, y2);
        frame_points.insert(frame_points.end(), ray_pts.begin(), ray_pts.end());
    }
    
    // 4. Calculate Color from Hue
    int hue_color_index = get_color_from_hue(current_hue);
    
    trail.push_front({frame_points, hue_color_index});
    
    // 5. Manage Trail Length
    if (trail.size() > 25) {
        trail.pop_back();
    }

    // 6. Advance State
    angle += angle_speed;
    current_hue += 15.0; // Increment hue faster (was 3.0)
    if (current_hue >= 360.0) current_hue -= 360.0;
}

void RotatingLineLogo::draw() {
    // Draw from back (oldest) to front (newest)
    for (int i = trail.size() - 1; i >= 0; --i) {
        const auto& state = trail[i];
        
        attron(COLOR_PAIR(state.color));
        
        // Newest = Bold
        if (i == 0) attron(A_BOLD);
        else attroff(A_BOLD);

        for (const auto& p : state.points) {
            // Use asterisk character
            mvaddch(p.y, p.x, '#'); 
        }
        
        if (i == 0) attroff(A_BOLD);
        attroff(COLOR_PAIR(state.color));
    }
}
