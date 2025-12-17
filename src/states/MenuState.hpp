#pragma once

#include "State.hpp"
#include "StateManager.hpp"
#include "Menu.hpp"
#include <vector>
#include <string>
#include <ncurses.h>

/**
 * @brief Abstract base state for interactive menus.
 * 
 * Handles common navigation input (Up/Down/Enter/Esc) and drawing logic.
 * Derived classes implement specific selection handling.
 */
class MenuState : public State {
public:
    /**
     * @brief Construct a new Menu State.
     * @param title Display title for the menu.
     * @param options List of selectable options.
     */
    MenuState(const std::string& title, const std::vector<std::string>& options) 
        : title(title), options(options), choice(0) {}

    /**
     * @brief Standard list navigation handling.
     * Up/Down to navigate, Right/Enter to select, Left/Q to back.
     */
    void handle_input(int ch, StateManager& mgr) override {
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

    void update(StateManager& /*mgr*/) override {
        // Static menus do not require frame updates
    }

    void draw(StateManager& /*mgr*/) override {
        draw_menu_static(title, options, choice);
    }
    
    bool is_blocking() const override { return true; }

protected:
    /**
     * @brief Called when user selects an option.
     * @param mgr State manager reference.
     * @param index Index of selected option.
     */
    virtual void on_select(StateManager& mgr, int index) = 0;

    /**
     * @brief Called when user requests to go back.
     * @param mgr State manager reference.
     */
    virtual void on_back(StateManager& mgr) = 0;

    std::string title;
    std::vector<std::string> options;
    int choice;
};
