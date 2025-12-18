#include "MenuState.hpp"
#include <ncurses.h>

void MenuState::handle_input(int ch, StateManager& mgr) {
    if (ch == KEY_UP) {
        choice--;
        if (choice < 0) choice = options.size() - 1;
    } else if (ch == KEY_DOWN) {
        choice++;
        if (choice >= (int)options.size()) choice = 0;
    } else if (ch == KEY_RIGHT || ch == 10) { 
        on_select(mgr, choice);
    } else if (ch == KEY_LEFT || ch == 'q' || ch == 'Q') {
        on_back(mgr);
    }
}

void MenuState::update(StateManager& /*mgr*/) {
    // Static menus do not require frame updates
}

void MenuState::draw(StateManager& /*mgr*/) {
    draw_menu_static(title, options, choice);
}
