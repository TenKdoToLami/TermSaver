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
    /**
     * @brief Standard list navigation handling.
     * Up/Down to navigate, Right/Enter to select, Left/Q to back.
     */
    void handle_input(int ch, StateManager& mgr) override;

    void update(StateManager& /*mgr*/) override;

    void draw(StateManager& /*mgr*/) override;
    
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
