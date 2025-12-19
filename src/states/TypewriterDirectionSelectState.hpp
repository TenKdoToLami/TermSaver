/**
 * @file TypewriterDirectionSelectState.hpp
 * @brief Header for the TypewriterDirectionSelectState class.
 */

#pragma once

#include "MenuState.hpp"

/**
 * @class TypewriterDirectionSelectState
 * @brief Menu for selecting the typewriter painting direction.
 */
class TypewriterDirectionSelectState : public MenuState {
public:
    TypewriterDirectionSelectState();
    
    virtual ~TypewriterDirectionSelectState() = default;

    void on_select(StateManager& mgr, int index) override;
    void on_back(StateManager& mgr) override;
};
