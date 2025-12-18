#pragma once
#include "MenuState.hpp"
#include "StateManager.hpp"

/**
 * @class SettingsState
 * @brief Menu for configuring global application settings (FPS, etc).
 */
class SettingsState : public MenuState {
public:
    SettingsState(const std::string& path);
    
    /** @brief Handles value adjustment via Left/Right keys. */
    void handle_input(int ch, StateManager& mgr) override;

    /** @brief Draws the menu with live-updated value text. */
    void draw(StateManager& mgr) override;

protected:
    void on_select(StateManager& mgr, int index) override;
    void on_back(StateManager& mgr) override;
    
    void update_options_text(StateManager& mgr);
    std::string current_path;
};
