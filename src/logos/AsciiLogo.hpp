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
    AsciiLogo(const std::vector<std::string>& art_data) : lines(art_data) {
        update_dimensions();
    }

    virtual ~AsciiLogo() = default;

    /**
     * @brief Set the content of the logo.
     * @param new_art New vector of strings.
     */
    void set_art(const std::vector<std::string>& new_art) {
        lines = new_art;
        update_dimensions();
    }

    /**
     * @brief Sets a generator function for dynamic content (regenerates entire art).
     * @param gen Function returning a new vector of strings.
     */
    void set_art_generator(std::function<std::vector<std::string>()> gen) {
        art_generator = gen;
    }

    /**
     * @brief Sets a generator function for single cells (modifies in-place tracking).
     * @param gen Function returning a single char.
     */
    void set_cell_generator(std::function<char()> gen) {
        cell_generator = gen;
    }

    /**
     * @brief Called when color changes. Regenerates art if a full-art generator is set.
     * 
     * Note: Per-cell regeneration is typically handled in the draw/update loop
     * of derived classes using the cell_generator.
     */
    void on_color_change() override {
        if (art_generator) {
            set_art(art_generator());
        }
    }

    int get_width() const override { return width; }
    int get_height() const override { return height; }

protected:
    std::vector<std::string> lines;
    int width = 0;
    int height = 0;
    
    std::function<std::vector<std::string>()> art_generator;
    
    // Per-cell dynamic noise tracking
    std::vector<std::vector<int>> last_colors;
    std::function<char()> cell_generator;

    /**
     * @brief Recalculates width and height based on current lines.
     * Also resets the last_colors grid for cell tracking.
     */
    void update_dimensions() {
        height = lines.size();
        width = 0;
        for (const auto& line : lines) {
            if ((int)line.length() > width) {
                width = line.length();
            }
        }
        // Initialize last_colors with -1 to force initial update
        last_colors.assign(height, std::vector<int>(width, -1));
    }
};
