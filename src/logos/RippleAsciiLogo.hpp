#pragma once

#include "Logo.hpp"
#include <vector>
#include <string>

/**
 * @class RippleAsciiLogo
 * @brief Displays a static, centered ASCII logo that ripples colors radially.
 */
class RippleAsciiLogo : public Logo {
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

    int get_width() const override;
    int get_height() const override;

private:
    std::vector<std::string> lines;
    int width;
    int height;
    double current_hue;
};
