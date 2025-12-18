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

             if (art == 2 || art == 3 || art == 4 || art == 5) { // Random / Dynamic / Solid Block / Fading
                 int noise_w = max_x;
                 int noise_h = max_y;
                 
                 if (art == 3) { // DYNAMIC NOISE SETTINGS
                     if (cat == 0 || mgr.settings.global_dynamic_noise_size) {
                         noise_w = (max_x * mgr.settings.dynamic_noise_percent_w) / 100;
                         noise_h = (max_y * mgr.settings.dynamic_noise_percent_h) / 100;
                     }
                     // Apply percentages if Global Toggle is ON OR Category is Kinetic Bounce (0)
                     if (cat == 0 || mgr.settings.solid_block_global) {
                         noise_w = (max_x * mgr.settings.solid_block_w_percent) / 100;
                         noise_h = (max_y * mgr.settings.solid_block_h_percent) / 100;
                     } 
                 } else if (art == 5) { // SOLID BLOCK FADING SETTINGS
                     if (cat == 0 || mgr.settings.fading_block_global) {
                         noise_w = (max_x * mgr.settings.fading_block_w_percent) / 100;
                         noise_h = (max_y * mgr.settings.fading_block_h_percent) / 100;
                     }
                     // Otherwise default to full screen
                 } else { // STATIC NOISE SETTINGS (art == 2)
                     if (cat == 0 || mgr.settings.global_noise_size) {
                         noise_w = (max_x * mgr.settings.noise_percent_w) / 100;
                         noise_h = (max_y * mgr.settings.noise_percent_h) / 100;
                     }
                 }
                 
                 if (noise_w < 1) noise_w = 1;
                 if (noise_h < 1) noise_h = 1;

                 if (art == 4) {
                     dynamic_art = generate_solid_block_art(noise_w, noise_h, mgr.settings.solid_block_symbol);
                 } else if (art == 5) {
                     dynamic_art = generate_solid_block_art(noise_w, noise_h, mgr.settings.fading_block_symbol);
                 } else {
                     dynamic_art = generate_noise_art(noise_w, noise_h);
                 }
                 art_ptr = &dynamic_art;
             } else {
                 const LogoVariants* variants = (art == 0) ? &BARTY_VARIANTS : &POP_VARIANTS;
                 art_ptr = &select_art(*variants, force_small, max_y, max_x);
             }
             
             // Create Animation Instance
             Logo* raw_ptr = nullptr;
             
             if (art == 2 || art == 3 || art == 4 || art == 5) {
                 stored_art = dynamic_art;
                 if (cat == 0) raw_ptr = new BouncingAsciiLogo(stored_art);
                 else if (cat == 1) raw_ptr = new RippleAsciiLogo(stored_art);
                 else if (cat == 2) raw_ptr = new HeartbeatAsciiLogo(stored_art);
             } else {
                 if (cat == 0) raw_ptr = new BouncingAsciiLogo(*art_ptr);
                 else if (cat == 1) raw_ptr = new RippleAsciiLogo(*art_ptr);
                 else if (cat == 2) raw_ptr = new HeartbeatAsciiLogo(*art_ptr);
             }
             
             app = raw_ptr;
             
             // Setup Dynamic Generator if needed
             if (app && (art == 3 || art == 5)) { // Dynamic Noise OR Solid Block Fading
                     AsciiLogo* ascii_logo = dynamic_cast<AsciiLogo*>(app);
                     if (ascii_logo) {
                          int noise_w = max_x;
                          int noise_h = max_y;
                          
                          // Use DYNAMIC settings specifically here
                          if (cat == 0 || mgr.settings.global_dynamic_noise_size) {
                             if (art == 5) {
                                 noise_w = (max_x * mgr.settings.fading_block_w_percent) / 100;
                                 noise_h = (max_y * mgr.settings.fading_block_h_percent) / 100;
                             } else {
                                 noise_w = (max_x * mgr.settings.dynamic_noise_percent_w) / 100;
                                 noise_h = (max_y * mgr.settings.dynamic_noise_percent_h) / 100;
                             }
                             
                             if (noise_w < 1) noise_w = 1;
                             if (noise_h < 1) noise_h = 1;
                          }

                          // Helper Class Definition (Moved Up)
                             class SolidBlockFader {
                             public:
                                 SolidBlockFader(int w, int h, char sym, int low_thresh, int high_thresh, int chance) 
                                     : width(w), height(h), symbol(sym), 
                                       low_threshold(low_thresh), high_threshold(high_thresh), change_chance(chance),
                                       visibility(h, std::vector<bool>(w, true)), // All visible initially
                                       total_cells(w * h), visible_cells(w * h), is_reappearing_mode(false) {}

                                 char update(int r, int c) {
                                     if (r >= height || c >= width) return symbol;

                                     // Determine if we should flip state
                                     // Helper: simple check against chance
                                     bool hit = (rand() % 100 < change_chance);

                                     if (!is_reappearing_mode) {
                                         // FADING MODE
                                         if (visibility[r][c] && hit) {
                                             visibility[r][c] = false;
                                             visible_cells--;
                                         }
                                         if (visible_cells < (total_cells * low_threshold) / 100) {
                                             is_reappearing_mode = true;
                                         }
                                     } else {
                                         // REAPPEARING MODE
                                         if (!visibility[r][c] && hit) {
                                             visibility[r][c] = true;
                                             visible_cells++;
                                         }
                                         if (visible_cells > (total_cells * high_threshold) / 100) {
                                             is_reappearing_mode = false;
                                         }
                                     }
                                     return visibility[r][c] ? symbol : ' ';
                                 }

                             private:
                                 int width, height;
                                 char symbol;
                                 int low_threshold, high_threshold, change_chance;
                                 std::vector<std::vector<bool>> visibility;
                                 int total_cells;
                                 int visible_cells;
                                 bool is_reappearing_mode;
                             };
                         
                         // Capture by value
                         if (cat == 0) { // Kinetic Bounce - Regenerate Full Art
                             if (art == 5) {
                                 // SOLID BLOCK FADING (Bounce)
                                 auto fader = std::make_shared<SolidBlockFader>(noise_w, noise_h, mgr.settings.fading_block_symbol,
                                                                                 mgr.settings.fading_low_threshold, mgr.settings.fading_high_threshold, mgr.settings.fading_change_chance);
                                 
                                 ascii_logo->set_art_generator([fader, noise_w, noise_h]() {
                                     // Reconstruct buffer using fader state
                                     std::vector<std::string> buffer;
                                     buffer.reserve(noise_h);
                                     for (int r = 0; r < noise_h; ++r) {
                                         std::string line = "";
                                         line.reserve(noise_w);
                                         for (int c = 0; c < noise_w; ++c) {
                                             line += fader->update(r, c);
                                         }
                                         buffer.push_back(line);
                                     }
                                     return buffer;
                                 });
                             } else {
                                 // DYNAMIC NOISE
                                 ascii_logo->set_art_generator([=]() {
                                     return generate_noise_art(noise_w, noise_h);
                                 });
                             }
                     } else { // Ripple/Heartbeat - Regenerate per Cell
                         if (art == 5) { // SOLID BLOCK FADING LOGIC
                             // Shared Fader Instance
                             // For Ripple, dimensions come from the logo itself (which was set on init)
                             auto fader = std::make_shared<SolidBlockFader>(ascii_logo->get_width(), ascii_logo->get_height(), mgr.settings.fading_block_symbol,
                                                                             mgr.settings.fading_low_threshold, mgr.settings.fading_high_threshold, mgr.settings.fading_change_chance);

                             ascii_logo->set_cell_generator([fader](int r, int c) -> char {
                                 return fader->update(r, c);
                             });

                         } else {
                             // Standard Dynamic Noise (art=3) OR Solid Block Static (art=4) which shouldn't be here normally
                             
                             if (art == 4) {
                                 // Constant Return for Static Block
                                 char sym = mgr.settings.solid_block_symbol;
                                 ascii_logo->set_cell_generator([sym](int, int) -> char {
                                     return sym; 
                                 });
                             } else {
                                 // Dynamic Noise (Random Chars)
                                 ascii_logo->set_cell_generator([](int, int) -> char {
                                     static const std::string chars = "   !\"#$%&'()*+,-./:;<>=?&[]\\^|}{~€ƒ‡—";
                                     return chars[rand() % chars.length()];
                                 });
                             }
                         }
                     }
                 }
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
    /**
     * @class SolidBlockFader
     * @brief Helper class to manage the probabilistic fading state of individual cells.
     */
    class SolidBlockFader {
    public:
        SolidBlockFader(int w, int h, char sym, int low_thresh, int high_thresh, int chance) 
            : width(w), height(h), symbol(sym), 
              low_threshold(low_thresh), high_threshold(high_thresh), change_chance(chance),
              visibility(h, std::vector<bool>(w, true)), // All visible initially
              total_cells(w * h), visible_cells(w * h), is_reappearing_mode(false) {}

        char update(int r, int c) {
            if (r >= height || c >= width) return symbol;

            // Determine if we should flip state
            // Helper: simple check against chance
            bool hit = (rand() % 100 < change_chance);

            if (!is_reappearing_mode) {
                // FADING MODE
                if (visibility[r][c] && hit) {
                    visibility[r][c] = false;
                    visible_cells--;
                }
                
                // Switch to Reappearing if < Low Threshold
                if (visible_cells < (total_cells * low_threshold) / 100) {
                    is_reappearing_mode = true;
                }
                
            } else {
                // REAPPEARING MODE
                if (!visibility[r][c] && hit) {
                    visibility[r][c] = true;
                    visible_cells++;
                }
                
                // Switch to Fading if > High Threshold
                if (visible_cells > (total_cells * high_threshold) / 100) {
                    is_reappearing_mode = false;
                }
            }
            
            return visibility[r][c] ? symbol : ' ';
        }

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
