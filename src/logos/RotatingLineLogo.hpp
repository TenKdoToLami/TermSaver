#pragma once
#include "AsciiLogo.hpp"
#include <vector>
#include <deque>
#include <cmath>

/**
 * @class RotatingLineLogo
 * @brief Renders a rotating radar/scanner line effect over ASCII art.
 * 
 * Features:
 * - Full-screen diagonal sweep.
 * - Gap-free interpolated drawing.
 * - Rainbow Hue-cycling trails.
 * - Renders underlying ASCII art (Barty, Noise, etc.) from base AsciiLogo.
 */
class RotatingLineLogo : public AsciiLogo {
private:
    struct Point { int x, y; };
    
    /**
     * @struct LineState
     * @brief Snapshot of a line's position and color for the trail effect.
     */
    struct LineState {
        std::vector<Point> points;
        int color;
    };
    
    std::vector<std::vector<int>> cell_colors; ///< Persistent color grid
    double angle;                   ///< Current angle in radians
    const double angle_speed = 0.08;///< Rotation speed (radians per frame)
    
    double current_brush_hue;       ///< Hue of the "brush" currently sweeping
    int color_change_interval;      ///< Interval in degrees for color change
    bool initialized = false;       ///< Tracks if grid is initialized

    /**
     * @brief Generates raster points for a line using Bresenham's algorithm.
     */
    std::vector<Point> get_line_points(int x1, int y1, int x2, int y2);

public:
    RotatingLineLogo(const std::vector<std::string>& art_data, int interval = 100);
    // Using AsciiLogo's get_width/get_height for the art dimensions

    /**
     * @brief Updates rotation state and generates new trail segments.
     * 
     * Calculates the new line position, fills gaps via interpolation,
     * and updates the color hue.
     */
    void update(int scr_height, int scr_width) override;

    /**
     * @brief Draws the trail of lines, fading from old to new.
     */
    void draw() override;
};
