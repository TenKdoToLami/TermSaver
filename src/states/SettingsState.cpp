#include "SettingsState.hpp"
#include "NoiseSettingsState.hpp"
#include "SolidBlockSettingsState.hpp"
#include "SolidBlockFadingSettingsState.hpp"
#include "NoiseFadingSettingsState.hpp"
#include <curses.h>
#include <memory>
#include <string>

SettingsState::SettingsState(const std::string& path) 
    : MenuState(path + "/SETTINGS", {}), current_path(path + "/SETTINGS") {
    options = {"Target FPS", "Static Noise Settings", "Dynamic Noise Settings", "Solid Block Settings", "Fading Block Settings", "Noise Fading Settings", "Back"};
}

void SettingsState::update_options_text(StateManager& mgr) {
    options[0] = "Target FPS: " + std::to_string(mgr.settings.target_fps);
    options[1] = "Static Noise Settings";
    options[2] = "Dynamic Noise Settings";
    options[3] = "Solid Block Settings";
    options[4] = "Fading Block Settings";
    options[5] = "Noise Fading Settings";
    options[6] = "Back";
}

void SettingsState::draw(StateManager& mgr) {
    update_options_text(mgr);
    MenuState::draw(mgr);
}

void SettingsState::handle_input(int ch, StateManager& mgr) {
    if (ch == KEY_UP || ch == KEY_DOWN) {
        MenuState::handle_input(ch, mgr);
        return;
    }
    
    // Custom Left/Right navigation for values
    if (ch == KEY_LEFT) {
        if (choice == 0) { // Target FPS
            mgr.settings.target_fps -= 5;
            if (mgr.settings.target_fps < 5) mgr.settings.target_fps = 5;
        } else if (choice == 6) { // Back
            on_back(mgr);
        }
    } else if (ch == KEY_RIGHT) {
        if (choice == 0) { // Target FPS
            mgr.settings.target_fps += 5;
            if (mgr.settings.target_fps > 120) mgr.settings.target_fps = 120;
        } else if (choice == 6) { // Back
            on_select(mgr, choice); 
        }
    } else if (ch == 10) { // Enter
        on_select(mgr, choice);
    } else if (ch == 'q' || ch == 'Q') {
        on_back(mgr);
    }
}

void SettingsState::on_select(StateManager& mgr, int index) {
    if (index == 1) {
        mgr.push_state(std::make_unique<NoiseSettingsState>(current_path, NoiseSettingsState::NoiseType::STATIC));
    } else if (index == 2) {
        mgr.push_state(std::make_unique<NoiseSettingsState>(current_path, NoiseSettingsState::NoiseType::DYNAMIC));
    } else if (index == 3) {
        mgr.push_state(std::make_unique<SolidBlockSettingsState>(current_path));
    } else if (index == 4) {
        mgr.push_state(std::make_unique<SolidBlockFadingSettingsState>(current_path));
    } else if (index == 5) {
        mgr.push_state(std::make_unique<NoiseFadingSettingsState>(current_path));
    } else if (index == 6) {
        on_back(mgr);
    }
}

void SettingsState::on_back(StateManager& mgr) {
    mgr.pop_state();
}
