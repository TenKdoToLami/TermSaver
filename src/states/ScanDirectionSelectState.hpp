/**
 * @file ScanDirectionSelectState.hpp
 * @brief Header for the ScanDirectionSelectState class.
 */

#pragma once

#include "MenuState.hpp"

/**
 * @class ScanDirectionSelectState
 * @brief Menu for selecting the direction of the Directional Scan effect.
 */
class ScanDirectionSelectState : public MenuState {
public:
    /** @brief Construct a new Scan Direction Select State object. */
    ScanDirectionSelectState();

    /**
     * @brief Handle direction selection.
     * @param mgr Reference to the state manager.
     * @param index The index of the selected direction.
     */
    void on_select(StateManager& mgr, int index) override;

    /**
     * @brief Return to the effect selection menu.
     * @param mgr Reference to the state manager.
     */
    void on_back(StateManager& mgr) override;
};
