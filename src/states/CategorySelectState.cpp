#include "CategorySelectState.hpp"
#include "AnimationState.hpp"
#include "SettingsState.hpp"
#include "LogoSelectState.hpp"
#include <memory>
#include <vector>
#include <string>

CategorySelectState::CategorySelectState() : MenuState("EFFECTS", {
    "Kinetic Bounce",
    "Radial Ripple",
    "Cardio Pulse",
    "Sonar Sweep",
    "Settings",
    "Exit"
}) {}

void CategorySelectState::on_select(StateManager& mgr, int index) {
    mgr.context_category_index = index;
    
    if (index == 4) {
         // Settings
         mgr.push_state(std::make_unique<SettingsState>(title));
    } else if (index == 5) {
         // Exit
         mgr.quit();
    } else {
         // Other -> Go to Logo Selection (Kinetic, Ripple, Pulse, Radar)
         std::vector<std::string> names = {
            "KINETIC BOUNCE",
            "RADIAL RIPPLE",
            "CARDIO PULSE",
            "SONAR SWEEP"
         };
         // Use title (which is "SELECT EFFECT") as base
         if (index >= 0 && index < (int)names.size()) {
            mgr.push_state(std::make_unique<LogoSelectState>(title, names[index]));
         }
    }
}

void CategorySelectState::on_back(StateManager& mgr) {
    mgr.quit();
}
