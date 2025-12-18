#pragma once
#include "MenuState.hpp"
#include "StateManager.hpp"

/**
 * @class LogoSelectState
 * @brief Menu for selecting the specific ASCII model (Barty, Pop_OS, Noise).
 */
class LogoSelectState : public MenuState {
public:
    /**
     * @brief Construct a new Logo Select State object.
     * 
     * @param parent_path Path string from the parent menu.
     * @param effect_name Name of the effect category selected.
     */
    LogoSelectState(const std::string& parent_path, const std::string& effect_name);
    
protected:
    void on_select(StateManager& mgr, int index) override;
    void on_back(StateManager& mgr) override;
};
