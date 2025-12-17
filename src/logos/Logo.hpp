#pragma once
#include <ncurses.h>
#include <cstdlib>

/**
 * @class Logo
 * @brief Abstract base class for all animated logos.
 * 
 * Provides basic bouncing physics and position management.
 * Specific rendering logic must be implemented by derived classes.
 */
class Logo {
protected:
    int x, y;           ///< Current screen position (top-left)
    int dx, dy;         ///< Current movement direction (+1 or -1)
    int max_colors;     ///< Maximum available colors in the terminal
    int current_pair;   ///< Currently active color pair ID

public:
    Logo();
    virtual ~Logo();

    /**
     * @brief Selects a new random color from the available palette.
     */
    void pick_new_color();

    /**
     * @brief Hook called whenever the color changes.
     * 
     * Derived classes can override this to regenerate content.
     */
    virtual void on_color_change() {}

    /**
     * @brief Callback triggered when the logo hits a screen boundary.
     * 
     * Default implementation picks a new random color.
     */
    virtual void on_bounce();

    /**
     * @brief Helper for rainbow effects. Converts HSV hue to Xterm-256 color index.
     * @param h Hue value (0.0 to 360.0).
     * @return int Color pair index.
     */
    int get_color_from_hue(double h);

    /**
     * @brief Initializes the logo's position to the center of the screen
     * and randomizes initial direction.
     * 
     * @param scr_height Screen height
     * @param scr_width Screen width
     */
    virtual void init_position(int scr_height, int scr_width);

    /**
     * @brief Gets the width of the logo.
     * @return Width in characters.
     */
    virtual int get_width() const = 0;

    /**
     * @brief Gets the height of the logo.
     * @return Height in lines.
     */
    virtual int get_height() const = 0;

    /**
     * @brief Updates the logo's state and position.
     * 
     * Handles basic physics (bouncing off walls) and movement.
     * 
     * @param scr_height Height of the screen.
     * @param scr_width Width of the screen.
     */
    virtual void update(int scr_height, int scr_width);

    /**
     * @brief Renders the logo to the ncurses window.
     * 
     * Must be implemented by derived classes.
     */
    virtual void draw() = 0;
};
