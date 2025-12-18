#include "NoiseFadingSettingsState.hpp"
#include <curses.h>
#include <string>

NoiseFadingSettingsState::NoiseFadingSettingsState(const std::string& parent_path) 
    : MenuState(parent_path + "/FADING_SETTINGS", {}) {
    options = {
        "Disappear Threshold", 
        "Reappear Threshold", 
        "State Change Chance",
        "Back"
    };
}

void NoiseFadingSettingsState::update_options_text(StateManager& mgr) {
    options[0] = "Disappear Threshold: " + std::to_string(mgr.settings.noise_fading_low) + "%";
    options[1] = "Reappear Threshold: " + std::to_string(mgr.settings.noise_fading_high) + "%";
    options[2] = "State Change Chance: " + std::to_string(mgr.settings.noise_fading_change_chance) + "%";
    options[3] = "Back";
}

void NoiseFadingSettingsState::draw(StateManager& mgr) {
    update_options_text(mgr);
    MenuState::draw(mgr);
}

void NoiseFadingSettingsState::handle_input(int ch, StateManager& mgr) {
    if (ch == KEY_UP || ch == KEY_DOWN) {
        MenuState::handle_input(ch, mgr);
        return;
    }
    
    int& low = mgr.settings.noise_fading_low;
    int& high = mgr.settings.noise_fading_high;
    int& chance = mgr.settings.noise_fading_change_chance;

    if (ch == KEY_LEFT) {
        if (choice == 0) { low -= 5; if (low < 0) low = 0; }
        else if (choice == 1) { high -= 5; if (high < 0) high = 0; if (high <= low) high = low + 5; } 
        else if (choice == 2) { chance -= 5; if (chance < 0) chance = 0; }
        else if (choice == 3) { on_back(mgr); }

    } else if (ch == KEY_RIGHT) {
        if (choice == 0) { low += 5; if (low > 100) low = 100; if (low >= high) low = high - 5; }
        else if (choice == 1) { high += 5; if (high > 100) high = 100; }
        else if (choice == 2) { chance += 5; if (chance > 100) chance = 100; }
        else if (choice == 3) { on_select(mgr, choice); }

    } else if (ch == 10) { // Enter
         on_select(mgr, choice);

    } else if (ch == 'q' || ch == 'Q') {
        on_back(mgr);
    }
}

void NoiseFadingSettingsState::on_select(StateManager& mgr, int index) {
    if (index == 3) {
        on_back(mgr);
    }
}

void NoiseFadingSettingsState::on_back(StateManager& mgr) {
    mgr.pop_state();
}
