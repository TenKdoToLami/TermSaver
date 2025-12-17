#pragma once
#include "State.hpp"
#include "Logo.hpp"
#include "AsciiArt.hpp"
#include "BouncingAsciiLogo.hpp"
#include "RippleAsciiLogo.hpp"
#include "HeartbeatAsciiLogo.hpp"
#include "RotatingLineLogo.hpp"
#include "StateManager.hpp"

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
    AnimationState(StateManager& mgr) : app(nullptr) {
        int cat = mgr.context_category_index;
        int art = mgr.context_art_index;
        
        if (cat == 3) {
            app = new RotatingLineLogo();
        } else {
             int max_x, max_y;
             getmaxyx(stdscr, max_y, max_x);
             
             bool force_small = (cat == 0);
             
             std::vector<std::string> dynamic_art;
             const std::vector<std::string>* art_ptr = nullptr;

             if (art == 2) { // Random Noise
                 int noise_w = max_x;
                 int noise_h = max_y;
                 
                 // Apply Percentage if applicable
                 // Kinetic Bounce (cat == 0) -> ALWAYS use percent size
                 // Others -> Use percent size ONLY IF global_noise_size is TRUE
                 if (cat == 0 || mgr.settings.global_noise_size) {
                     noise_w = (max_x * mgr.settings.noise_percent_w) / 100;
                     noise_h = (max_y * mgr.settings.noise_percent_h) / 100;
                     
                     if (noise_w < 1) noise_w = 1;
                     if (noise_h < 1) noise_h = 1;
                 }
                 
                 dynamic_art = generate_noise_art(noise_w, noise_h);
                 art_ptr = &dynamic_art;
             } else {
                 const LogoVariants* variants = (art == 0) ? &BARTY_VARIANTS : &POP_VARIANTS;
                 art_ptr = &select_art(*variants, force_small, max_y, max_x);
             }
             
             // Create Animation Instance
             if (art == 2) {
                 stored_art = dynamic_art;
                 if (cat == 0) app = new BouncingAsciiLogo(stored_art);
                 else if (cat == 1) app = new RippleAsciiLogo(stored_art);
                 else if (cat == 2) app = new HeartbeatAsciiLogo(stored_art);
             } else {
                 if (cat == 0) app = new BouncingAsciiLogo(*art_ptr);
                 else if (cat == 1) app = new RippleAsciiLogo(*art_ptr);
                 else if (cat == 2) app = new HeartbeatAsciiLogo(*art_ptr);
             }
        }
        
        if (app) {
             int max_x, max_y;
             getmaxyx(stdscr, max_y, max_x);
             app->init_position(max_y, max_x);
        }
    }
    
    ~AnimationState() {
        if (app) delete app;
    }
    
    void handle_input(int ch, StateManager& mgr) override {
        if (ch == 'q' || ch == 'Q' || ch == KEY_LEFT) {
            mgr.pop_state();
        }
    }
    
    void update(StateManager& /*mgr*/) override {
        if (app) {
            int max_x, max_y;
            getmaxyx(stdscr, max_y, max_x);
            app->update(max_y, max_x);
        }
    }
    
    void draw(StateManager& /*mgr*/) override {
        if (app) app->draw();
    }
    
    bool is_blocking() const override { return false; }
    
private:
    Logo* app;
    std::vector<std::string> stored_art; 
};
