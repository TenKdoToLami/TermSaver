/**
 * @file TypewriterAsciiLogo.hpp
 * @brief Header for the TypewriterAsciiLogo class.
 */

#pragma once

#include "AsciiLogo.hpp"
#include <vector>

/**
 * @class TypewriterAsciiLogo
 * @brief Logo that reveals itself like a typewriter, one cell at a time.
 * 
 * Follows a selected direction (8 possible) and highlights the current 
 * "cursor" position with a distinct color.
 */
class TypewriterAsciiLogo : public AsciiLogo {
public:
    /**
     * @brief Construct a new Typewriter Ascii Logo object.
     * @param art_data Initial ASCII art lines.
     * @param dir Direction: 0=TL, 1=T, 2=TR, 3=R, 4=BR, 5=B, 6=BL, 7=L
     */
    TypewriterAsciiLogo(const std::vector<std::string>& art_data, int dir = 0);

    virtual ~TypewriterAsciiLogo() = default;

    /** @brief Centers the logo on screen. */
    void init_position(int scr_height, int scr_width) override;
    
    /** @brief Updates the typewriter progress and cursor hue. */
    void update(int scr_height, int scr_width) override;
    
    /** @brief Renders the visible cells and the active cursor. */
    void draw() override;

private:
    int cursor_r;                           ///< Current row of the typewriter cursor.
    int cursor_c;                           ///< Current column of the typewriter cursor.
    int direction;                          ///< Painting direction (0-7).
    double base_hue;                        ///< Cycling hue for new text.
    std::vector<std::vector<int>> cell_colors; ///< Stored color for each cell.
};
