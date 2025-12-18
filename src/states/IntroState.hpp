#pragma once
#include "State.hpp"
#include "StateManager.hpp"

/**
 * @class IntroState
 * @brief Simple "splash screen" state.
 *
 * Displays the application title (ASCII art) and author.
 * Waits for user to press Enter to proceed to the main menu.
 */
class IntroState : public State {
public:
    /**
     * @brief Handle input events.
     * Transitions to CategorySelectState on Enter.
     * @param ch Input character code.
     * @param mgr Reference to StateManager.
     */
    void handle_input(int ch, StateManager& mgr) override;

    /**
     * @brief Update state logic.
     * Currently a no-op for this static screen.
     * @param mgr Reference to StateManager.
     */
    void update(StateManager& mgr) override;

    /**
     * @brief Draw the splash screen to the terminal.
     * @param mgr Reference to StateManager.
     */
    void draw(StateManager& mgr) override;

    /**
     * @brief Indicates this state blocks execution (wait for input).
     * @return true
     */
    bool is_blocking() const override { return true; }
};
