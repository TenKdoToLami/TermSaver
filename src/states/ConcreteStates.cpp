#include "ConcreteStates.hpp"
#include "AnimationState.hpp"
#include <memory>

// --- IntroState Implementation ---
 
 void IntroState::handle_input(int ch, StateManager& mgr) {
     // Transition on Enter (10) or Keypad Enter (KEY_ENTER)
     if (ch == 10 || ch == KEY_ENTER) {
         mgr.change_state(std::make_unique<CategorySelectState>());
     }
 }
 
 void IntroState::update(StateManager& /*mgr*/) {
     // Static screen, no update logic needed.
 }
 
 void IntroState::draw(StateManager& /*mgr*/) {
     int height, width;
     getmaxyx(stdscr, height, width);
     
     // ASCII Art Title
     const std::vector<std::string> art = {
         " _______                  _____                    ",
         "|__   __|                / ____|                   ",
         "    | | ___ _ __ _ __ ___| (___   __ ___   _____ _ __ ",
         "    | |/ _ \\ '__| '_ ` _ \\\\___ \\ / _` \\ \\ / / _ \\ '__|",
         "    | |  __/ |  | | | | | |___) | (_| |\\ V /  __/ |   ",
         "     |_|\\___|_|  |_| |_| |_|_____/\\__,_| \\_/ \\___|_|   "
     };
 
     // Calculate vertical centering
     int start_y = (height - (int)art.size()) / 2 - 2;
     if (start_y < 0) start_y = 0;
 
     // Draw Title
     attron(COLOR_PAIR(1)); 
     for (size_t i = 0; i < art.size(); ++i) {
         mvprintw(start_y + (int)i, (width - (int)art[i].length()) / 2, "%s", art[i].c_str());
     }
     attroff(COLOR_PAIR(1));
 
     // Draw Author
     const std::string author = "TenKdoToLami";
     mvprintw(start_y + (int)art.size() + 2, (width - (int)author.length()) / 2, "%s", author.c_str());
 
     // Draw Prompt
     const std::string prompt = "Press Enter to continue..."; 
     mvprintw(start_y + (int)art.size() + 4, (width - (int)prompt.length()) / 2, "%s", prompt.c_str());
 }

// --- LogoSelectState Implementation ---

void LogoSelectState::on_select(StateManager& mgr, int index) {
    if (index == 3) { // Back
        on_back(mgr);
    } else {
        mgr.context_art_index = index;
        mgr.push_state(std::make_unique<AnimationState>(mgr));
    }
}

// --- CategorySelectState Implementation ---

void CategorySelectState::on_select(StateManager& mgr, int index) {
    mgr.context_category_index = index;
    
    if (index == 3) { 
         // Radar -> Go straight to Animation
         mgr.push_state(std::make_unique<AnimationState>(mgr));
    } else if (index == 4) {
         // Settings
         mgr.push_state(std::make_unique<SettingsState>(title));
    } else if (index == 5) {
         // Exit
         mgr.quit();
    } else {
         // Other -> Go to Logo Selection
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

// --- SettingsState Implementation ---

SettingsState::SettingsState(const std::string& path) 
    : MenuState(path + "/SETTINGS", {}), current_path(path + "/SETTINGS") {
    options = {"Target FPS", "Static Noise Settings", "Back"};
}

void SettingsState::update_options_text(StateManager& mgr) {
    options[0] = "Target FPS: " + std::to_string(mgr.settings.target_fps);
    options[1] = "Static Noise Settings";
    options[2] = "Back";
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
        } else if (choice == 2) { // Back
            on_back(mgr);
        }
    } else if (ch == KEY_RIGHT) {
        if (choice == 0) { // Target FPS
            mgr.settings.target_fps += 5;
            if (mgr.settings.target_fps > 120) mgr.settings.target_fps = 120;
        } else if (choice == 2) { // Back
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
        mgr.push_state(std::make_unique<StaticNoiseSettingsState>(current_path));
    } else if (index == 2) {
        on_back(mgr);
    }
}

void SettingsState::on_back(StateManager& mgr) {
    mgr.pop_state();
}

// --- StaticNoiseSettingsState Implementation ---

StaticNoiseSettingsState::StaticNoiseSettingsState(const std::string& path) 
    : MenuState(path + "/NOISE", {}) {
    options = {"Width Coverage", "Height Coverage", "Force Global Size", "Back"};
}

void StaticNoiseSettingsState::update_options_text(StateManager& mgr) {
    options[0] = "Width Coverage: " + std::to_string(mgr.settings.noise_percent_w) + "%";
    options[1] = "Height Coverage: " + std::to_string(mgr.settings.noise_percent_h) + "%";
    options[2] = "Force Global Size: " + std::string(mgr.settings.global_noise_size ? "ON" : "OFF");
    options[3] = "Back";
}

void StaticNoiseSettingsState::draw(StateManager& mgr) {
    update_options_text(mgr);
    MenuState::draw(mgr);
}

void StaticNoiseSettingsState::handle_input(int ch, StateManager& mgr) {
    if (ch == KEY_UP || ch == KEY_DOWN) {
        MenuState::handle_input(ch, mgr);
        return;
    }

    if (ch == KEY_LEFT) {
        if (choice == 0) { // Width Coverage
            mgr.settings.noise_percent_w -= 5;
            if (mgr.settings.noise_percent_w < 5) mgr.settings.noise_percent_w = 5;
        } else if (choice == 1) { // Height Coverage
            mgr.settings.noise_percent_h -= 5;
            if (mgr.settings.noise_percent_h < 5) mgr.settings.noise_percent_h = 5;
        } else if (choice == 2) { // Global Toggle
            mgr.settings.global_noise_size = !mgr.settings.global_noise_size;
        } else if (choice == 3) { // Back
            on_back(mgr);
        }
    } else if (ch == KEY_RIGHT) {
        if (choice == 0) { // Width Coverage
            mgr.settings.noise_percent_w += 5;
            if (mgr.settings.noise_percent_w > 100) mgr.settings.noise_percent_w = 100;
        } else if (choice == 1) { // Height Coverage
            mgr.settings.noise_percent_h += 5;
            if (mgr.settings.noise_percent_h > 100) mgr.settings.noise_percent_h = 100;
        } else if (choice == 2) { // Global Toggle
            mgr.settings.global_noise_size = !mgr.settings.global_noise_size;
        } else if (choice == 3) { // Back
            on_select(mgr, choice); 
        }
    } else if (ch == 10) { // Enter
        if (choice == 2) { // Toggle on Enter for bool
             mgr.settings.global_noise_size = !mgr.settings.global_noise_size;
        } else {
             on_select(mgr, choice);
        }
    } else if (ch == 'q' || ch == 'Q') {
        on_back(mgr);
    }
}

void StaticNoiseSettingsState::on_select(StateManager& mgr, int index) {
    if (index == 3) {
        on_back(mgr);
    }
}

void StaticNoiseSettingsState::on_back(StateManager& mgr) {
    mgr.pop_state();
}


