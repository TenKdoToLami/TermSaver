#pragma once
#include <vector>
#include <string>

// Forward Declaration
class StateManager;

/**
 * @class State
 * @brief Abstract base class for all application states.
 */
class State {
public:
    virtual ~State() = default;
    
    /**
     * @brief Handle input (keyboard).
     * @param ch The input character.
     * @param mgr The StateManager (context).
     */
    virtual void handle_input(int ch, StateManager& mgr) = 0;
    
    /**
     * @brief Update state logic (animation frames, etc.).
     * @param mgr The StateManager (context).
     */
    virtual void update(StateManager& mgr) = 0;
    
    /**
     * @brief Draw the state to the screen.
     * @param mgr The StateManager (context).
     */
    virtual void draw(StateManager& mgr) = 0;

    /**
     * @brief Should this state block for input?
     * @return true if it is a menu (waits for key), false if animation (runs continuously).
     */
    virtual bool is_blocking() const = 0;
};
