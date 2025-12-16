#ifndef HEARTBEAT_ASCII_LOGO_HPP
#define HEARTBEAT_ASCII_LOGO_HPP

#include "Logo.hpp"
#include <vector>
#include <string>

enum class HeartbeatState {
    EXPANDING,
    RETRACTING
};

class HeartbeatAsciiLogo : public Logo {
public:
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

#endif // HEARTBEAT_ASCII_LOGO_HPP
