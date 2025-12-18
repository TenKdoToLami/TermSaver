#pragma once

#include "Logo.hpp"
#include <vector>
#include <string>
#include <functional>

/**
 * @class AsciiLogo
 * @brief Base class for Logos that render ASCII art.
 * 
 * Provides storage for the ASCII lines and supports dynamic regeneration of content
 * (e.g. for "Dynamic Noise" effects).
 */
class AsciiLogo : public Logo {

public:
    /**
     * @brief Construct a new Ascii Logo object.
     * @param art_data The initial ASCII art lines.
     */
    AsciiLogo(const std::vector<std::string>& art_data);

    virtual ~AsciiLogo() = default;

    /**
     * @brief Set the content of the logo.
     * @param new_art New vector of strings.
     */
    void set_art(const std::vector<std::string>& new_art);

    /**
     * @brief Sets a generator function for dynamic content (regenerates entire art).
     * @param gen Function returning a new vector of strings.
     */
    void set_art_generator(std::function<std::vector<std::string>()> gen);

    /**
     * @brief Sets a generator function for single cells (modifies in-place tracking).
     * @param gen Function returning a single char for a given position (row, col).
     */
    void set_cell_generator(std::function<char(int r, int c)> gen);

    /**
     * @brief Called when color changes. Regenerates art if a full-art generator is set.
     * 
     * Note: Per-cell regeneration is typically handled in the draw/update loop
     * of derived classes using the cell_generator.
     */
    void on_color_change() override;

    int get_width() const override;
    int get_height() const override;

protected:
    std::vector<std::string> lines;
    int width = 0;
    int height = 0;
    
    std::function<std::vector<std::string>()> art_generator;
    
    // Per-cell dynamic noise tracking
    std::vector<std::vector<int>> last_colors;
    std::function<char(int r, int c)> cell_generator;

    /**
     * @brief Recalculates width and height based on current lines.
     * Also resets the last_colors grid for cell tracking.
     */
    void update_dimensions();
};
