#pragma once
#include "Logo.hpp"
#include <vector>
#include <string>

/**
 * @class BouncingAsciiLogo
 * @brief Renders a static ASCII art image that bounces around the screen.
 * 
 * Takes a vector of strings as input and treats spaces as transparent.
 */
class BouncingAsciiLogo : public Logo {
private:
    std::vector<std::string> lines; ///< The ASCII art data
    int width;                      ///< Cached width of the art
    int height;                     ///< Cached height of the art

public:
    /**
     * @brief Constructs a new BouncingAsciiLogo.
     * 
     * @param art_data The ASCII art lines to display.
     */
    BouncingAsciiLogo(const std::vector<std::string>& art_data);

    int get_width() const override;
    int get_height() const override;

    /**
     * @brief Draws the ASCII art at the current position.
     * 
     * Applies bold styling and skips transparent (space) characters.
     */
    void draw() override;
};
