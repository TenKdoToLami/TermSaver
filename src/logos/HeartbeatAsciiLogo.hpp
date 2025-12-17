#pragma once

#include "Logo.hpp"
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
class HeartbeatAsciiLogo : public Logo {
public:
    /**
     * @brief Construct a new Heartbeat Ascii Logo.
     * @param art_data ASCII art lines.
     */
    HeartbeatAsciiLogo(const std::vector<std::string>& art_data);

    void init_position(int scr_height, int scr_width) override;
    void update(int scr_height, int scr_width) override;
    void draw() override;

    int get_width() const override;
    int get_height() const override;

private:
    std::vector<std::string> lines;
    int width;
    int height;
    
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
