/**
 * @file SolidBlockSettingsState.hpp
 * @brief Header for the SolidBlockSettingsState class.
 */

#pragma once
#include "MenuState.hpp"
#include "StateManager.hpp"

/**
 * @class SolidBlockSettingsState
 * @brief Submenu for configuring Solid Block settings.
 * 
 * Allows users to adjust size percentages and the symbol character.
 */
class SolidBlockSettingsState : public MenuState {
public:
    /**
     * @brief Construct a new Solid Block Settings State object.
     * @param path The menu path history.
     */
    SolidBlockSettingsState(const std::string& path);

    virtual ~SolidBlockSettingsState() = default;
    
    /** @brief Handles input for adjustment keys (Left/Right). */
    void handle_input(int ch, StateManager& mgr) override;

    /** @brief Renders the settings menu and interactive symbol selector. */
    void draw(StateManager& mgr) override;

    /** @brief Processes menu selections. */
    void on_select(StateManager& mgr, int index) override;

    /** @brief Returns to the previous state. */
    void on_back(StateManager& mgr) override;
    
private:
    /** @brief Synchronizes labels with current setting values. */
    void update_options_text(StateManager& mgr);
};
