#include "CategorySelectState.hpp"
#include "AnimationState.hpp"
#include "SettingsState.hpp"
#include "LogoSelectState.hpp"
#include "ScanDirectionSelectState.hpp"
#include <memory>
#include <vector>
#include <string>

CategorySelectState::CategorySelectState() : MenuState("EFFECTS", {
    "Kinetic Bounce",
    "Radial Ripple",
    "Cardio Pulse",
    "Sonar Sweep",
    "Directional Scan",
    "Settings",
    "Exit"
}) {}

void CategorySelectState::on_select(StateManager& mgr, int index) {
    mgr.context_category_index = index;
    
    if (index == 5) {
         // Settings
         mgr.push_state(std::make_unique<SettingsState>(title));
    } else if (index == 6) {
         // Exit
         mgr.quit();
    } else if (index == 4) {
         // Directional Scan -> Select Direction first
         mgr.push_state(std::make_unique<ScanDirectionSelectState>());
    } else {
         // Other -> Go to Logo Selection
         std::vector<std::string> names = {
            "KINETIC BOUNCE",
            "RADIAL RIPPLE",
            "CARDIO PULSE",
            "SONAR SWEEP",
            "DIRECTIONAL SCAN"
         };
         if (index >= 0 && index < (int)names.size()) {
            mgr.push_state(std::make_unique<LogoSelectState>(title, names[index]));
         }
    }
}

void CategorySelectState::on_back(StateManager& mgr) {
    mgr.quit();
}
