#pragma once
#include "State.hpp"
#include <vector>
#include <stack>
#include <memory>

/**
 * @brief Manages the application state stack.
 * 
 * Controls state transitions (push/pop) and holds global application settings/context.
 */
class StateManager {
public:
    StateManager() : running(true) {}

    /** @brief Push a new state onto the stack. */
    void push_state(std::unique_ptr<State> state) {
        states.push(std::move(state));
    }

    /** @brief Remove the current state from the stack. */
    void pop_state() {
        if (!states.empty()) {
            states.pop();
        }
    }
    
    /** @brief Replace the current state with a new one. */
    void change_state(std::unique_ptr<State> state) {
        pop_state();
        push_state(std::move(state));
    }

    /** @brief Get the currently active state. */
    State* current_state() {
        if (states.empty()) return nullptr;
        return states.top().get();
    }
    
    /** @brief Check if the application loop should continue. */
    bool is_running() const { return running && !states.empty(); }

    /** @brief Signal the application to exit. */
    void quit() { running = false; }
    
    // Global Context Data
    int context_category_index = 0;
    int context_art_index = 0;

    /**
     * @struct AppSettings
     * @brief Holds global configuration options for the application.
     */
    struct AppSettings {
        /** @brief Desired framerate for animations. Default 20. */
        int target_fps = 20;
        
        // --- Static Noise Settings ---
        /** @brief Width coverage % for static noise. */
        int noise_percent_w = 50;
        /** @brief Height coverage % for static noise. */
        int noise_percent_h = 50;
        /** @brief If true, applies noise dimensions to all effects. */
        bool global_noise_size = false;
        
        // --- Dynamic Noise Settings ---
        /** @brief Dynamic noise width percentage (5-100). */
        int dynamic_noise_percent_w = 50;
        /** @brief Dynamic noise height percentage (5-100). */
        int dynamic_noise_percent_h = 50;
        /** @brief Whether dynamic noise size applies globally to all animations. */
        bool global_dynamic_noise_size = false;

        // --- Solid Block Settings ---
        /** @brief Solid block width percentage (5-100). */
        int solid_block_w_percent = 50;
        /** @brief Solid block height percentage (5-100). */
        int solid_block_h_percent = 50;
        /** @brief The symbol character used to render the solid block. */
        char solid_block_symbol = '#';
        /** @brief Whether solid block size applies globally to all animations. */
        bool solid_block_global = false;

        // --- Solid Block Fading Settings ---
        /** @brief Fading block width percentage (5-100). */
        int fading_block_w_percent = 50;
        /** @brief Fading block height percentage (5-100). */
        int fading_block_h_percent = 50;
        /** @brief The symbol character used to render the fading block. */
        char fading_block_symbol = '#';
        /** @brief Whether fading block size applies globally. */
        bool fading_block_global = false;
        
        /** @brief Threshold % of visible cells to switch to Reappearing Mode (Default 10). */
        int fading_low_threshold = 10;
        /** @brief Threshold % of visible cells to switch to Fading Mode (Default 90). */
        int fading_high_threshold = 90;
        /** @brief Probability % of a cell changing state on update (Default 10). */
        int fading_change_chance = 10;
    } settings;

private:
    std::stack<std::unique_ptr<State>> states;
    bool running;
};
