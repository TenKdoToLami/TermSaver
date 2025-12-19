/**
 * @file NoiseSettingsState.hpp
 * @brief Header for the NoiseSettingsState class.
 */

#pragma once
#include "MenuState.hpp"
#include "StateManager.hpp"

/**
 * @class NoiseSettingsState
 * @brief Submenu for configuring noise specific settings (Static & Dynamic).
 * 
 * Allows the user to adjust width/height coverage and toggle global application
 * of these settings for the selected noise type.
 */
class NoiseSettingsState : public MenuState {
public:
    /** @brief The type of noise being configured. */
    enum class NoiseType { STATIC, DYNAMIC };

    /**
     * @brief Construct a new Noise Settings State object.
     * @param path Parent menu path for breadcrumbs.
     * @param type The type of noise variables to modify.
     */
    NoiseSettingsState(const std::string& path, NoiseType type);

    virtual ~NoiseSettingsState() = default;
    
    /** @brief Handles input for adjusting values. */
    void handle_input(int ch, StateManager& mgr) override;

    /** @brief Renders the noise settings menu. */
    void draw(StateManager& mgr) override;

    /** @brief Processes menu selections. */
    void on_select(StateManager& mgr, int index) override;

    /** @brief Returns to the previous state. */
    void on_back(StateManager& mgr) override;
    
private:
    /** @brief Synchronizes labels with current values. */
    void update_options_text(StateManager& mgr);
    
    NoiseType type;           ///< Which noise settings we are editing.
    std::string current_path; ///< Cached path for display.
};
