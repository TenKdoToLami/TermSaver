#pragma once
#include "MenuState.hpp"
#include "StateManager.hpp"

/**
 * @class SolidBlockSettingsState
 * @brief Submenu for configuring Solid Block settings.
 */
class SolidBlockSettingsState : public MenuState {
public:
    SolidBlockSettingsState(const std::string& path);
    
    void handle_input(int ch, StateManager& mgr) override;
    void draw(StateManager& mgr) override;

protected:
    void on_select(StateManager& mgr, int index) override;
    void on_back(StateManager& mgr) override;
    
    void update_options_text(StateManager& mgr);
};
