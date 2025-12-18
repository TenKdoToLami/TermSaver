#pragma once
#include "State.hpp"
#include "Logo.hpp"
#include "AsciiArt.hpp"
#include "BouncingAsciiLogo.hpp"
#include "RippleAsciiLogo.hpp"
#include "HeartbeatAsciiLogo.hpp"
#include "RotatingLineLogo.hpp"
#include "StateManager.hpp"



/**
 * @class AnimationState
 * @brief State that runs the active visualization.
 * 
 * Instantiates the correct Logo subclass based on the selected Category and Art
 * from the StateManager context. Handles frame updates and input for exiting.
 */


class AnimationState : public State {
public:
    /**
     * @brief Construct a new Animation State object.
     * 
     * Initializes the specific Logo animation based on the StateManager context (category/art).
     * 
     * @param mgr Reference to the StateManager.
     */
    AnimationState(StateManager& mgr);

    /**
     * @brief Destroy the Animation State object.
     */
    ~AnimationState();
    
    /**
     * @brief Handle input during animation (e.g., exit).
     * 
     * @param ch Input character.
     * @param mgr Reference to StateManager.
     */
    void handle_input(int ch, StateManager& mgr) override;

    /**
     * @brief Update the animation frame.
     * 
     * @param mgr Reference to StateManager.
     */
    void update(StateManager& mgr) override;

    /**
     * @brief Draw the current animation frame to screen.
     * 
     * @param mgr Reference to StateManager.
     */
    void draw(StateManager& mgr) override;
    
    /**
     * @brief Returns false as animations are non-blocking (they run in a loop).
     * @return false 
     */
    bool is_blocking() const override { return false; }
    
private:
    /**
     * @class SolidBlockFader
     * @brief Helper class to manage the probabilistic fading state of individual cells.
     */
    class SolidBlockFader {
    public:
        SolidBlockFader(int w, int h, char sym, int low_thresh, int high_thresh, int chance);
        char update(int r, int c);

    private:
        int width, height;
        char symbol;
        int low_threshold, high_threshold, change_chance;
        std::vector<std::vector<bool>> visibility;
        int total_cells;
        int visible_cells;
        bool is_reappearing_mode;
    };

    Logo* app;
    std::vector<std::string> stored_art; 
};
