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
    
    bool is_running() const { return running && !states.empty(); }
    void quit() { running = false; }
    
    // Global Context Data
    int context_category_index = 0;
    int context_art_index = 0;

    /**
     * @struct AppSettings
     * @brief Holds global configuration options.
     */
    struct AppSettings {
        int target_fps = 20;          ///< Desired framerate for animations.
        int noise_percent_w = 50;     ///< Width coverage % for static noise.
        int noise_percent_h = 50;     ///< Height coverage % for static noise.
        bool global_noise_size = false; ///< If true, applies noise dimensions to all effects.
        
        // Dynamic Noise Settings
        int dynamic_noise_percent_w = 50;     ///< Width coverage % for dynamic noise.
        int dynamic_noise_percent_h = 50;     ///< Height coverage % for dynamic noise.
        bool global_dynamic_noise_size = false; ///< If true, applies dynamic noise dimensions to all effects.
    } settings;

private:
    std::stack<std::unique_ptr<State>> states;
    bool running;
};
