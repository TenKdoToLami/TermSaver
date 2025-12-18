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
    /**
     * @brief The type of noise being configured.
     */
    enum class NoiseType { STATIC, DYNAMIC };

    /**
     * @brief Construct a new Noise Settings State object.
     * 
     * @param path Parent menu path for breadcrumbs.
     * @param type The type of noise variables to modify.
     */
    NoiseSettingsState(const std::string& path, NoiseType type);
    
    void handle_input(int ch, StateManager& mgr) override;
    void draw(StateManager& mgr) override;

protected:
    void on_select(StateManager& mgr, int index) override;
    void on_back(StateManager& mgr) override;
    
    void update_options_text(StateManager& mgr);
    
    NoiseType type;
    std::string current_path;
};
