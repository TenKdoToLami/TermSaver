#pragma once
#include "Logo.hpp"
#include <vector>
#include <deque>
#include <cmath>

/**
 * @class RotatingLineLogo
 * @brief Renders a rotating radar/scanner line effect.
 * 
 * Features:
 * - Full-screen diagonal sweep.
 * - Gap-free interpolated drawing.
 * - Rainbow Hue-cycling trails.
 */
class RotatingLineLogo : public Logo {
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
    
    std::deque<LineState> trail;    ///< History of line positions
    double angle;                   ///< Current angle in radians
    const double angle_speed = 0.08;///< Rotation speed (radians per frame)
    double current_hue;             ///< Current color hue (0-360)

    /**
     * @brief Generates raster points for a line using Bresenham's algorithm.
     */
    std::vector<Point> get_line_points(int x1, int y1, int x2, int y2);

public:
    RotatingLineLogo();
    
    // Dynamic size (returns 0 as this logo covers the full screen)
    int get_width() const override;
    int get_height() const override;

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
