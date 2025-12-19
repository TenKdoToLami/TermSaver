/**
 * @file NoiseSettingsState.cpp
 * @brief Implementation of the noise configuration menu (Static & Dynamic).
 */

#include "NoiseSettingsState.hpp"
#include <curses.h>
#include <string>

NoiseSettingsState::NoiseSettingsState(const std::string& path, NoiseType type) 
    : MenuState(path + (type == NoiseType::STATIC ? "/STATIC_NOISE" : "/DYNAMIC_NOISE"), {}), type(type) {
    current_path = path + (type == NoiseType::STATIC ? "/STATIC_NOISE" : "/DYNAMIC_NOISE");
    options = {"Width Coverage", "Height Coverage", "Space %", "Force Global Size", "Back"};
}

void NoiseSettingsState::update_options_text(StateManager& mgr) {
    int w, h, s;
    bool global;
    
    if (type == NoiseType::STATIC) {
        w = mgr.settings.noise_percent_w;
        h = mgr.settings.noise_percent_h;
        s = mgr.settings.noise_space_percent;
        global = mgr.settings.global_noise_size;
    } else {
        w = mgr.settings.dynamic_noise_percent_w;
        h = mgr.settings.dynamic_noise_percent_h;
        s = mgr.settings.dynamic_noise_space_percent;
        global = mgr.settings.global_dynamic_noise_size;
    }

    options[0] = "Width Coverage: " + std::to_string(w) + "%";
    options[1] = "Height Coverage: " + std::to_string(h) + "%";
    options[2] = "Space %: " + std::to_string(s) + "%";
    options[3] = "Force Global Size: " + std::string(global ? "ON" : "OFF");
    options[4] = "Back";
}

void NoiseSettingsState::draw(StateManager& mgr) {
    update_options_text(mgr);
    MenuState::draw(mgr);
}

void NoiseSettingsState::handle_input(int ch, StateManager& mgr) {
    if (ch == KEY_UP || ch == KEY_DOWN) {
        MenuState::handle_input(ch, mgr);
        return;
    }
    
    int& w = (type == NoiseType::STATIC) ? mgr.settings.noise_percent_w : mgr.settings.dynamic_noise_percent_w;
    int& h = (type == NoiseType::STATIC) ? mgr.settings.noise_percent_h : mgr.settings.dynamic_noise_percent_h;
    int& s = (type == NoiseType::STATIC) ? mgr.settings.noise_space_percent : mgr.settings.dynamic_noise_space_percent;
    bool& global = (type == NoiseType::STATIC) ? mgr.settings.global_noise_size : mgr.settings.global_dynamic_noise_size;

    if (ch == KEY_LEFT) {
        if (choice == 0) {
            w -= 5; if (w < 5) w = 5;
        } else if (choice == 1) {
            h -= 5; if (h < 5) h = 5;
        } else if (choice == 2) {
            s -= 5; if (s < 0) s = 0;
        } else if (choice == 3) {
            global = !global;
        } else if (choice == 4) {
            on_back(mgr);
        }
    } else if (ch == KEY_RIGHT) {
        if (choice == 0) {
            w += 5; if (w > 100) w = 100;
        } else if (choice == 1) {
            h += 5; if (h > 100) h = 100;
        } else if (choice == 2) {
            s += 5; if (s > 100) s = 100;
        } else if (choice == 3) {
            global = !global;
        } else if (choice == 4) {
            on_select(mgr, choice); 
        }
    } else if (ch == 10) { // Enter
        if (choice == 3) {
             global = !global;
        } else {
             on_select(mgr, choice);
        }
    } else if (ch == 'q' || ch == 'Q') {
        on_back(mgr);
    }
}

void NoiseSettingsState::on_select(StateManager& mgr, int index) {
    if (index == 4) {
        on_back(mgr);
    }
}

void NoiseSettingsState::on_back(StateManager& mgr) {
    mgr.pop_state();
}
