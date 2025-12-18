#include "LogoSelectState.hpp"
#include "AnimationState.hpp"
#include <memory>

LogoSelectState::LogoSelectState(const std::string& parent_path, const std::string& effect_name) 
    : MenuState(parent_path + "/" + effect_name, {"Barty", "Pop!_OS", "Static Noise", "Dynamic Noise", "Solid Block", "Solid Block Fading", "Static Noise Fading", "Dynamic Noise Fading", "Back"}) {}

void LogoSelectState::on_select(StateManager& mgr, int index) {
    if (index == 8) { // Back
        on_back(mgr);
    } else {
        mgr.context_art_index = index;
        mgr.push_state(std::make_unique<AnimationState>(mgr));
    }
}

void LogoSelectState::on_back(StateManager& mgr) {
    mgr.pop_state(); 
}
