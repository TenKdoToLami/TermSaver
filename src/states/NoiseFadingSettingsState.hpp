#pragma once
#include "MenuState.hpp"
#include "StateManager.hpp"

/**
 * @class NoiseFadingSettingsState
 * @brief Menu for configuring general noise fading parameters.
 */
class NoiseFadingSettingsState : public MenuState {
public:
    explicit NoiseFadingSettingsState(const std::string& parent_path);

    void handle_input(int ch, StateManager& mgr) override;
    void draw(StateManager& mgr) override;

protected:
    void on_select(StateManager& mgr, int index) override;
    void on_back(StateManager& mgr) override;

private:
    void update_options_text(StateManager& mgr);
};
