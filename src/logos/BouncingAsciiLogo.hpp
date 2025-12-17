#pragma once
#include "AsciiLogo.hpp"
#include <vector>
#include <string>

/**
 * @class BouncingAsciiLogo
 * @brief Renders a static ASCII art image that bounces around the screen.
 * 
 * Takes a vector of strings as input and treats spaces as transparent.
 */
class BouncingAsciiLogo : public AsciiLogo {
public:
    /**
     * @brief Constructs a new BouncingAsciiLogo.
     * 
     * @param art_data The ASCII art lines to display.
     */
    BouncingAsciiLogo(const std::vector<std::string>& art_data);

    /**
     * @brief Draws the ASCII art at the current position.
     * 
     * Applies bold styling and skips transparent (space) characters.
     */
    void draw() override;
};
