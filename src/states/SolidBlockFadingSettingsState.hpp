/**
 * @file SolidBlockFadingSettingsState.hpp
 * @brief Header for the SolidBlockFadingSettingsState class.
 */

#pragma once
#include "MenuState.hpp"
#include "StateManager.hpp"

/**
 * @class SolidBlockFadingSettingsState
 * @brief Submenu for configuring the Fading Block settings.
 * 
 * Allows users to adjust size, symbol, and fading behavior (thresholds and speed).
 */
class SolidBlockFadingSettingsState : public MenuState {
public:
    /**
     * @brief Construct a new Solid Block Fading Settings State object.
     * @param parent_path The menu path history.
     */
    explicit SolidBlockFadingSettingsState(const std::string& parent_path);

    virtual ~SolidBlockFadingSettingsState() = default;

    /** @brief Handles input for adjusting values. */
    void handle_input(int ch, StateManager& mgr) override;

    /** @brief Renders the settings menu and symbol selector. */
    void draw(StateManager& mgr) override;

    /** @brief Processes menu selections. */
    void on_select(StateManager& mgr, int index) override;

    /** @brief Returns to the previous state. */
    void on_back(StateManager& mgr) override;

private:
    /** @brief Synchronizes labels with current setting values. */
    void update_options_text(StateManager& mgr);
};
