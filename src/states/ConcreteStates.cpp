#include "ConcreteStates.hpp"
#include "AnimationState.hpp"
#include <memory>
#include <string>
#include <unistd.h>
#include <cmath>

// Shared constant for symbols
static const std::string BLOCK_SYMBOLS = "!\"#$%&'()*+,-./:;<>=?&[]\\^|}{~";

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

LogoSelectState::LogoSelectState(const std::string& parent_path, const std::string& effect_name) 
    : MenuState(parent_path + "/" + effect_name, {"Barty", "Pop!_OS", "Static Noise", "Dynamic Noise", "Solid Block", "Solid Block Fading", "Back"}) {}

void LogoSelectState::on_select(StateManager& mgr, int index) {
    if (index == 6) { // Back
        on_back(mgr);
    } else {
        mgr.context_art_index = index;
        mgr.push_state(std::make_unique<AnimationState>(mgr));
    }
}

void LogoSelectState::on_back(StateManager& mgr) {
    mgr.pop_state(); 
}

// --- CategorySelectState Implementation ---

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
    options = {"Target FPS", "Static Noise Settings", "Dynamic Noise Settings", "Solid Block Settings", "Fading Block Settings", "Back"};
}

void SettingsState::update_options_text(StateManager& mgr) {
    options[0] = "Target FPS: " + std::to_string(mgr.settings.target_fps);
    options[1] = "Static Noise Settings";
    options[2] = "Dynamic Noise Settings";
    options[3] = "Solid Block Settings";
    options[4] = "Fading Block Settings";
    options[5] = "Back";
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
        } else if (choice == 5) { // Back
            on_back(mgr);
        }
    } else if (ch == KEY_RIGHT) {
        if (choice == 0) { // Target FPS
            mgr.settings.target_fps += 5;
            if (mgr.settings.target_fps > 120) mgr.settings.target_fps = 120;
        } else if (choice == 5) { // Back
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
        on_back(mgr);
    }
}

void SettingsState::on_back(StateManager& mgr) {
    mgr.pop_state();
}

// --- NoiseSettingsState Implementation ---

NoiseSettingsState::NoiseSettingsState(const std::string& path, NoiseType type) 
    : MenuState(path + (type == NoiseType::STATIC ? "/STATIC_NOISE" : "/DYNAMIC_NOISE"), {}), type(type) {
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
    
    // References to the correct variables
    int& w = (type == NoiseType::STATIC) ? mgr.settings.noise_percent_w : mgr.settings.dynamic_noise_percent_w;
    int& h = (type == NoiseType::STATIC) ? mgr.settings.noise_percent_h : mgr.settings.dynamic_noise_percent_h;
    int& s = (type == NoiseType::STATIC) ? mgr.settings.noise_space_percent : mgr.settings.dynamic_noise_space_percent;
    bool& global = (type == NoiseType::STATIC) ? mgr.settings.global_noise_size : mgr.settings.global_dynamic_noise_size;

    if (ch == KEY_LEFT) {
        if (choice == 0) { // Width Coverage
            w -= 5;
            if (w < 5) w = 5;
        } else if (choice == 1) { // Height Coverage
            h -= 5;
            if (h < 5) h = 5;
        } else if (choice == 2) { // Space percent
            s -= 5;
            if (s < 0) s = 0;
        } else if (choice == 3) { // Global Toggle
            global = !global;
        } else if (choice == 4) { // Back
            on_back(mgr);
        }
    } else if (ch == KEY_RIGHT) {
        if (choice == 0) { // Width Coverage
            w += 5;
            if (w > 100) w = 100;
        } else if (choice == 1) { // Height Coverage
            h += 5;
            if (h > 100) h = 100;
        } else if (choice == 2) { // Space percent
            s += 5;
            if (s > 100) s = 100;
        } else if (choice == 3) { // Global Toggle
            global = !global;
        } else if (choice == 4) { // Back
            on_select(mgr, choice); 
        }
    } else if (ch == 10) { // Enter
        if (choice == 3) { // Toggle on Enter for bool
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


// --- SolidBlockSettingsState Implementation ---

/**
 * @brief Constructs the SolidBlockSettingsState.
 * @param path The parent menu path.
 */
SolidBlockSettingsState::SolidBlockSettingsState(const std::string& path) 
    : MenuState(path + "/SOLID_BLOCK", {}) {
    options = {"Width %", "Height %", "Symbol", "Force Global Size", "Back"};
}

/**
 * @brief Updates the options text with current setting values.
 * @param mgr The StateManager instance.
 */
void SolidBlockSettingsState::update_options_text(StateManager& mgr) {
    options[0] = "Width %: " + std::to_string(mgr.settings.solid_block_w_percent) + "%";
    options[1] = "Height %: " + std::to_string(mgr.settings.solid_block_h_percent) + "%";
    options[2] = "Symbol: " + std::string(1, mgr.settings.solid_block_symbol);
    options[3] = "Force Global Size: " + std::string(mgr.settings.solid_block_global ? "ON" : "OFF");
    options[4] = "Back";
}

/**
 * @brief Draws the menu and the custom symbol selector UI.
 * @param mgr The StateManager instance.
 */
void SolidBlockSettingsState::draw(StateManager& mgr) {
    update_options_text(mgr);

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int center_y = max_y / 2;
    int center_x = max_x / 2;
    
    // Dynamic Box Dimensions
    int box_width = 50; 
    int box_height = (max_y * 2) / 3;
    if (box_height < 10) box_height = 10;
    
    int start_y = center_y - (box_height / 2);
    int start_x = center_x - (box_width / 2);
    
    // Draw Box
    attron(A_BOLD);
    mvhline(start_y, start_x, 0, box_width);
    mvhline(start_y + box_height, start_x, 0, box_width);
    mvvline(start_y, start_x, 0, box_height);
    mvvline(start_y, start_x + box_width, 0, box_height);
    mvaddch(start_y, start_x, ACS_ULCORNER);
    mvaddch(start_y, start_x + box_width, ACS_URCORNER);
    mvaddch(start_y + box_height, start_x, ACS_LLCORNER);
    mvaddch(start_y + box_height, start_x + box_width, ACS_LRCORNER);
    attroff(A_BOLD);

    // Header
    attron(A_BOLD | A_UNDERLINE);
    std::string display_title = title;
    mvprintw(start_y + 1, center_x - (display_title.length()/2), "%s", display_title.c_str());
    attroff(A_BOLD | A_UNDERLINE);
    mvhline(start_y + 2, start_x + 1, ACS_HLINE, box_width - 2);
    mvhline(start_y + box_height - 2, start_x + 1, ACS_HLINE, box_width - 2);
    
    // List Geometry
    int list_top = start_y + 3;
    int list_bottom = start_y + box_height - 3;
    int list_center_y = list_top + ((list_bottom - list_top) / 2);

    // Draw Highlight Block (Background)
    attron(A_BOLD | A_REVERSE);
    for (int y = list_center_y - 1; y <= list_center_y + 1; ++y) {
        if (y >= list_top && y <= list_bottom) {
             mvhline(y, start_x + 1, ' ', box_width - 2);
        }
    }
    attroff(A_BOLD | A_REVERSE);

    // Draw Options
    for (int i = 0; i < (int)options.size(); ++i) {
        int offset = i - choice;
        
        // Match MenuState spacing logic
        int rel_y = 0;
        if (offset > 0) rel_y = offset + 1;
        else if (offset < 0) rel_y = offset - 1;
        
        int item_y = list_center_y + rel_y;

        if (item_y >= list_top && item_y <= list_bottom) {
            if (offset == 0) { // Selected Item (Inside Highlight Block)
                if (i == 2) { // Symbol Option - Custom Layout
                    // Line 1 (Top): Option Name
                    std::string label = "Symbol";
                    attron(A_BOLD | A_REVERSE);
                    mvprintw(item_y - 1, center_x - (label.length()/2), "%s", label.c_str());
                    
                    // Line 2 (Middle): Arrow
                    mvaddch(item_y, center_x, 'V');
                    attroff(A_BOLD | A_REVERSE); // Reset for list handling
                    
                    // Line 3 (Bottom): Symbol List
                    // Use static defined earlier
                    const std::string& raw_symbols = BLOCK_SYMBOLS;
                    char current = mgr.settings.solid_block_symbol;
                    
                    int total_symbols = raw_symbols.length();
                    int current_idx = -1;
                    
                    // Find actual index
                    for (int k = 0; k < total_symbols; ++k) {
                        if (raw_symbols[k] == current) {
                            current_idx = k;
                            break;
                        }
                    }
                    if (current_idx == -1) current_idx = 0;

                    // Calculate visible window
                    // Available width in box allows for roughly box_width / 2 symbols (since spaced by 2)
                    // Let's safe margin: box_width - 4 padding
                    int max_visible = (box_width - 4) / 2;
                    // Ensure odd number to center perfectly
                    if (max_visible % 2 == 0) max_visible--;
                    if (max_visible < 1) max_visible = 1;
                    
                    int half_visible = max_visible / 2;
                    
                    // We want to draw 'max_visible' symbols, centered on 'current_idx'.
                    // The center of the draw area is 'center_x'
                    
                    // Start drawing from: center_x - (half_visible * 2)
                    int start_draw_x = center_x - (half_visible * 2);

                    for (int offset = -half_visible; offset <= half_visible; ++offset) {
                         // Calculate cyclic index
                         int idx = (current_idx + offset) % total_symbols;
                         if (idx < 0) idx += total_symbols;
                         
                         int draw_x = center_x + (offset * 2);
                         
                         // Double check bounds within box
                         if (draw_x >= start_x + 1 && draw_x <= start_x + box_width - 2) {
                             if (offset == 0) {
                                 // Center item (Selected) -> Positive
                                 attron(A_BOLD); 
                                 mvaddch(item_y + 1, draw_x, raw_symbols[idx]);
                                 attroff(A_BOLD);
                             } else {
                                 // Side items -> Negative
                                 attron(A_BOLD | A_REVERSE);
                                 mvaddch(item_y + 1, draw_x, raw_symbols[idx]);
                                 attroff(A_BOLD | A_REVERSE);
                             }
                         }
                    }

                } else { // Standard Selected Option
                    attron(A_BOLD | A_REVERSE);
                    std::string label = "--> " + options[i] + " <--"; 
                    mvprintw(item_y, center_x - (label.length()/2), "%s", label.c_str());
                    attroff(A_BOLD | A_REVERSE);
                }
            } else { // Unselected Item
                mvprintw(item_y, center_x - (options[i].length()/2), "%s", options[i].c_str());
            }
        }
    }
    
    // Help Footer
    const char* help = "UP/DOWN: Move | ENTER/RIGHT: Select | Q/LEFT: Back/Exit";
    mvprintw(max_y - 2, (max_x - std::string(help).length())/2, "%s", help);
}

void SolidBlockSettingsState::handle_input(int ch, StateManager& mgr) {
    if (ch == KEY_UP || ch == KEY_DOWN) {
        MenuState::handle_input(ch, mgr);
        return;
    }
    
    int& w = mgr.settings.solid_block_w_percent;
    int& h = mgr.settings.solid_block_h_percent;
    char& sym = mgr.settings.solid_block_symbol;
    bool& global = mgr.settings.solid_block_global;

    if (ch == KEY_LEFT) {
        if (choice == 0) { // Width %
            w -= 5;
            if (w < 5) w = 5;
        } else if (choice == 1) { // Height %
            h -= 5;
            if (h < 5) h = 5;
        } else if (choice == 2) { // Symbol
            size_t pos = BLOCK_SYMBOLS.find(sym);
            if (pos == std::string::npos || pos == 0) sym = BLOCK_SYMBOLS.back();
            else sym = BLOCK_SYMBOLS[pos - 1];
        } else if (choice == 3) { // Force Global
            global = !global;
        } else if (choice == 4) { // Back
            on_back(mgr);
        }
    } else if (ch == KEY_RIGHT) {
        if (choice == 0) { // Width %
            w += 5;
            if (w > 100) w = 100;
        } else if (choice == 1) { // Height %
            h += 5;
            if (h > 100) h = 100;
        } else if (choice == 2) { // Symbol
            size_t pos = BLOCK_SYMBOLS.find(sym);
            if (pos == std::string::npos) pos = 0;
            sym = BLOCK_SYMBOLS[(pos + 1) % BLOCK_SYMBOLS.size()];
        } else if (choice == 3) { // Force Global
            global = !global;
        } else if (choice == 4) { // Back
            on_select(mgr, choice); 
        }
    } else if (ch == 10) { // Enter
         if (choice == 3) { // Global Toggle
             global = !global;
         } else {
             on_select(mgr, choice);
         }
    } else if (ch == 'q' || ch == 'Q') {
        on_back(mgr);
    }
}

void SolidBlockSettingsState::on_select(StateManager& mgr, int index) {
    if (index == 4) {
        on_back(mgr);
    }
}

void SolidBlockSettingsState::on_back(StateManager& mgr) {
    mgr.pop_state();
}

// --- SolidBlockFadingSettingsState Implementation ---

SolidBlockFadingSettingsState::SolidBlockFadingSettingsState(const std::string& path) 
    : MenuState(path + "/FADING_BLOCK", {}) {
    options = {
        "Width %", 
        "Height %", 
        "Symbol", 
        "Force Global Size", 
        "Disappear Threshold", 
        "Reappear Threshold", 
        "State Change Chance",
        "Back"
    };
}

void SolidBlockFadingSettingsState::update_options_text(StateManager& mgr) {
    options[0] = "Width %: " + std::to_string(mgr.settings.fading_block_w_percent) + "%";
    options[1] = "Height %: " + std::to_string(mgr.settings.fading_block_h_percent) + "%";
    options[2] = "Symbol: " + std::string(1, mgr.settings.fading_block_symbol);
    options[3] = "Force Global Size: " + std::string(mgr.settings.fading_block_global ? "ON" : "OFF");
    options[4] = "Disappear Threshold: " + std::to_string(mgr.settings.fading_low_threshold) + "%";
    options[5] = "Reappear Threshold: " + std::to_string(mgr.settings.fading_high_threshold) + "%";
    options[6] = "State Change Chance: " + std::to_string(mgr.settings.fading_change_chance) + "%";
    options[7] = "Back";
}

void SolidBlockFadingSettingsState::draw(StateManager& mgr) {
    update_options_text(mgr);
    
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int center_y = max_y / 2;
    int center_x = max_x / 2;
    
    // Dynamic Box Dimensions
    int box_width = 50; 
    int box_height = (max_y * 2) / 3;
    if (box_height < 14) box_height = 14; 
    
    int start_y = center_y - (box_height / 2);
    int start_x = center_x - (box_width / 2);
    
    // Draw Box
    attron(A_BOLD);
    mvhline(start_y, start_x, 0, box_width);
    mvhline(start_y + box_height, start_x, 0, box_width);
    mvvline(start_y, start_x, 0, box_height);
    mvvline(start_y, start_x + box_width, 0, box_height);
    mvaddch(start_y, start_x, ACS_ULCORNER);
    mvaddch(start_y, start_x + box_width, ACS_URCORNER);
    mvaddch(start_y + box_height, start_x, ACS_LLCORNER);
    mvaddch(start_y + box_height, start_x + box_width, ACS_LRCORNER);
    attroff(A_BOLD);

    // Header
    attron(A_BOLD | A_UNDERLINE);
    std::string display_title = title;
    mvprintw(start_y + 1, center_x - (display_title.length()/2), "%s", display_title.c_str());
    attroff(A_BOLD | A_UNDERLINE);
    mvhline(start_y + 2, start_x + 1, ACS_HLINE, box_width - 2);
    mvhline(start_y + box_height - 2, start_x + 1, ACS_HLINE, box_width - 2);
    
    // List Geometry
    int list_top = start_y + 3;
    int list_bottom = start_y + box_height - 3;
    int list_center_y = list_top + ((list_bottom - list_top) / 2);

    // Draw Highlight Block (Background)
    attron(A_BOLD | A_REVERSE);
    for (int y = list_center_y - 1; y <= list_center_y + 1; ++y) {
        if (y >= list_top && y <= list_bottom) {
             mvhline(y, start_x + 1, ' ', box_width - 2);
        }
    }
    attroff(A_BOLD | A_REVERSE);

    // Draw Options
    for (int i = 0; i < (int)options.size(); ++i) {
        int offset = i - choice;
        
        int rel_y = 0;
        if (offset > 0) rel_y = offset + 1;
        else if (offset < 0) rel_y = offset - 1;
        
        int item_y = list_center_y + rel_y;

        if (item_y >= list_top && item_y <= list_bottom) {
            if (offset == 0) { // Selected Item
                if (i == 2) { // Symbol Option
                    std::string label = "Symbol";
                    attron(A_BOLD | A_REVERSE);
                    mvprintw(item_y - 1, center_x - (label.length()/2), "%s", label.c_str());
                    mvaddch(item_y, center_x, 'V');
                    attroff(A_BOLD | A_REVERSE);
                    
                    const std::string& raw_symbols = BLOCK_SYMBOLS;
                    char current = mgr.settings.fading_block_symbol;
                    
                    int total_symbols = raw_symbols.length();
                    int current_idx = 0;
                    for (int k = 0; k < total_symbols; ++k) {
                        if (raw_symbols[k] == current) {
                            current_idx = k; break;
                        }
                    }

                    int max_visible = (box_width - 4) / 2;
                    if (max_visible % 2 == 0) max_visible--;
                    if (max_visible < 1) max_visible = 1;
                    int half_visible = max_visible / 2;

                    for (int offset = -half_visible; offset <= half_visible; ++offset) {
                         int idx = (current_idx + offset) % total_symbols;
                         if (idx < 0) idx += total_symbols;
                         
                         int draw_x = center_x + (offset * 2);
                         if (draw_x >= start_x + 1 && draw_x <= start_x + box_width - 2) {
                             if (offset == 0) {
                                 attron(A_BOLD); 
                                 mvaddch(item_y + 1, draw_x, raw_symbols[idx]);
                                 attroff(A_BOLD);
                             } else {
                                 attron(A_BOLD | A_REVERSE);
                                 mvaddch(item_y + 1, draw_x, raw_symbols[idx]);
                                 attroff(A_BOLD | A_REVERSE);
                             }
                         }
                    }

                } else { // Standard Selected Option
                    attron(A_BOLD | A_REVERSE);
                    std::string label = "--> " + options[i] + " <--"; 
                    mvprintw(item_y, center_x - (label.length()/2), "%s", label.c_str());
                    attroff(A_BOLD | A_REVERSE);
                }
            } else { // Unselected Item
                mvprintw(item_y, center_x - (options[i].length()/2), "%s", options[i].c_str());
            }
        }
    }
    
    // Help Footer
    const char* help = "UP/DOWN: Move | ENTER/RIGHT: Select | Q/LEFT: Back/Exit";
    mvprintw(max_y - 2, (max_x - std::string(help).length())/2, "%s", help);
}

void SolidBlockFadingSettingsState::handle_input(int ch, StateManager& mgr) {
    if (ch == KEY_UP || ch == KEY_DOWN) {
        MenuState::handle_input(ch, mgr);
        return;
    }
    
    int& w = mgr.settings.fading_block_w_percent;
    int& h = mgr.settings.fading_block_h_percent;
    char& sym = mgr.settings.fading_block_symbol;
    bool& global = mgr.settings.fading_block_global;
    
    int& low = mgr.settings.fading_low_threshold;
    int& high = mgr.settings.fading_high_threshold;
    int& chance = mgr.settings.fading_change_chance;

    if (ch == KEY_LEFT) {
        if (choice == 0) { w -= 5; if (w < 5) w = 5; }
        else if (choice == 1) { h -= 5; if (h < 5) h = 5; }
        else if (choice == 2) { 
            size_t pos = BLOCK_SYMBOLS.find(sym);
            if (pos == std::string::npos || pos == 0) sym = BLOCK_SYMBOLS.back();
            else sym = BLOCK_SYMBOLS[pos - 1];
        }
        else if (choice == 3) { global = !global; }
        else if (choice == 4) { low -= 5; if (low < 0) low = 0; }
        else if (choice == 5) { high -= 5; if (high < 0) high = 0; if (high <= low) high = low + 5; } 
        else if (choice == 6) { chance -= 5; if (chance < 0) chance = 0; }
        else if (choice == 7) { on_back(mgr); }

    } else if (ch == KEY_RIGHT) {
        if (choice == 0) { w += 5; if (w > 100) w = 100; }
        else if (choice == 1) { h += 5; if (h > 100) h = 100; }
        else if (choice == 2) {
            size_t pos = BLOCK_SYMBOLS.find(sym);
            if (pos == std::string::npos) pos = 0;
            sym = BLOCK_SYMBOLS[(pos + 1) % BLOCK_SYMBOLS.size()];
        }
        else if (choice == 3) { global = !global; }
        else if (choice == 4) { low += 5; if (low > 100) low = 100; if (low >= high) low = high - 5; }
        else if (choice == 5) { high += 5; if (high > 100) high = 100; }
        else if (choice == 6) { chance += 5; if (chance > 100) chance = 100; }
        else if (choice == 7) { on_select(mgr, choice); }

    } else if (ch == 10) { // Enter
         if (choice == 3) { global = !global; }
         else { on_select(mgr, choice); }

    } else if (ch == 'q' || ch == 'Q') {
        on_back(mgr);
    }
}

void SolidBlockFadingSettingsState::on_select(StateManager& mgr, int index) {
    if (index == 7) {
        on_back(mgr);
    }
}

void SolidBlockFadingSettingsState::on_back(StateManager& mgr) {
    mgr.pop_state();
}
