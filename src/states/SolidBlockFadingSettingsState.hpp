#pragma once
#include "MenuState.hpp"
#include "StateManager.hpp"

/**
 * @class SolidBlockFadingSettingsState
 * @brief Menu for configuring the solid block fading effect.
 */
class SolidBlockFadingSettingsState : public MenuState {
public:
    explicit SolidBlockFadingSettingsState(const std::string& parent_path);

    void handle_input(int ch, StateManager& mgr) override;
    void draw(StateManager& mgr) override;

protected:
    void on_select(StateManager& mgr, int index) override;
    void on_back(StateManager& mgr) override;

private:
    void update_options_text(StateManager& mgr);
};
