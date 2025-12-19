/**
 * @file ScanningAsciiLogo.hpp
 * @brief Header for the ScanningAsciiLogo class.
 */

#pragma once

#include "AsciiLogo.hpp"

/**
 * @class ScanningAsciiLogo
 * @brief Logo that displays a colored wave scanning in a specific direction.
 * 
 * Supports 8 directions: Top-Left, Top, Top-Right, Right, Bottom-Right, Bottom, Bottom-Left, Left.
 * The wave is purely atmospheric (color-based) and all characters remain visible.
 */
class ScanningAsciiLogo : public AsciiLogo {
public:
    /**
     * @brief Construct a new Scanning Ascii Logo object.
     * @param art_data Initial ASCII art lines.
     * @param direction The scan direction (0-7).
     */
    ScanningAsciiLogo(const std::vector<std::string>& art_data, int direction);

    virtual ~ScanningAsciiLogo() = default;

    /** @brief Sets the logo's position to the center of the screen. */
    void init_position(int scr_height, int scr_width) override;
    
    /** @brief Updates the internal hue/animation state. */
    void update(int scr_height, int scr_width) override;
    
    /** @brief Renders the logo with the directional color wave. */
    void draw() override;

private:
    double hue;    ///< Current base hue for the animation cycle.
    int direction; ///< Selected scan direction (0=TL, 1=T, 2=TR, 3=R, 4=BR, 5=B, 6=BL, 7=L).
};
