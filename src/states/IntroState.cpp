#include "IntroState.hpp"
#include "CategorySelectState.hpp"
#include <string>
#include <vector>
#include <curses.h>
#include <memory>

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
        "     | | ___ _ __ _ __ ___| (___   __ ___   _____ _ __ ",
        "     | |/ _ \\ '__| '_ ` _ \\\\___ \\ / _` \\ \\ / / _ \\ '__|",
        "     | |  __/ |  | | | | | |___) | (_| |\\ V /  __/ |   ",
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
