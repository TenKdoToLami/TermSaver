#pragma once

#include "AsciiLogo.hpp"
#include <vector>
#include <string>

/**
 * @enum HeartbeatState
 * @brief Current animation phase for the heartbeat effect.
 */
enum class HeartbeatState {
    EXPANDING,
    RETRACTING
};

/**
 * @class HeartbeatAsciiLogo
 * @brief Renders a "heartbeat" pulsing effect.
 * 
 * Expands and contracts the logo color radius while cycling background and foreground hues.
 */
class HeartbeatAsciiLogo : public AsciiLogo {
public:
    /**
     * @brief Construct a new Heartbeat Ascii Logo.
     * @param art_data ASCII art lines.
     */
    HeartbeatAsciiLogo(const std::vector<std::string>& art_data);

    /**
     * @brief Initialize position (centers the logo).
     * @param scr_height Screen height.
     * @param scr_width Screen width.
     */
    void init_position(int scr_height, int scr_width) override;

    /**
     * @brief Update the heartbeat animation state (radius expansion/retraction).
     * @param scr_height Screen height.
     * @param scr_width Screen width.
     */
    void update(int scr_height, int scr_width) override;

    /**
     * @brief Draw the logo with the pulsing color effect.
     */
    void draw() override;

private:
    
    // Animation State
    HeartbeatState state;
    double radius;
    double max_radius;
    double speed;
    
    // Color State
    double current_bg_hue;
    double pulse_hue;
    double next_bg_hue;
};
