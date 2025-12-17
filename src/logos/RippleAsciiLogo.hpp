#pragma once

#include "AsciiLogo.hpp"
#include <vector>
#include <string>

/**
 * @class RippleAsciiLogo
 * @brief Displays a static, centered ASCII logo that ripples colors radially.
 */
class RippleAsciiLogo : public AsciiLogo {
public:
    /**
     * @brief Construct a new Ripple Ascii Logo object
     * @param art_data Vector of strings representing the ASCII art
     */
    RippleAsciiLogo(const std::vector<std::string>& art_data);

    /**
     * @brief Initialize position (centers the logo)
     * @param scr_height Screen height
     * @param scr_width Screen width
     */
    void init_position(int scr_height, int scr_width) override;

    /**
     * @brief Update the color state (does not move position)
     * @param scr_height Screen height
     * @param scr_width Screen width
     */
    void update(int scr_height, int scr_width) override;

    /**
     * @brief Draw the logo with the current color pattern.
     */
    void draw() override;

private:
    double current_hue;
};
