#include "AnimationState.hpp"
#include "ScanningAsciiLogo.hpp"
#include "TypewriterAsciiLogo.hpp"
#include <ncurses.h>
#include <stdlib.h> // for rand
#include <memory>

// --- AnimationState Implementation ---

AnimationState::AnimationState(StateManager& mgr) : app(nullptr) {
    int cat = mgr.context_category_index;
    int art = mgr.context_art_index;

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    bool force_small = (cat == 0); // Only force small for Kinetic Bounce

        std::vector<std::string> dynamic_art;
        const std::vector<std::string>* art_ptr = nullptr;

        if (art == 2 || art == 3 || art == 4 || art == 5 || art == 6 || art == 7) { // Random / Dynamic / Solid Block / Fading
            int noise_w = max_x;
            int noise_h = max_y;

            if (art == 3 || art == 7) { // DYNAMIC NOISE & DYNAMIC FADING
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
            } else { // STATIC NOISE (art == 2) OR STATIC FADING (art == 6)
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
            } else if (art == 7) { // Dynamic Fading (Start empty/random?)
                // Initial buffer doesn't matter much as it regenerates, but size matters
                dynamic_art = generate_noise_art(noise_w, noise_h, 0); // 0 space for dense start?
            } else {
                // Static Noise (2) & Static Fading (6) & Dynamic (3)
                int space = (art == 3 || art == 7) ? mgr.settings.dynamic_noise_space_percent : mgr.settings.noise_space_percent;
                dynamic_art = generate_noise_art(noise_w, noise_h, space);
            }
            art_ptr = &dynamic_art;
        } else {
            const LogoVariants* variants = (art == 0) ? &BARTY_VARIANTS : &POP_VARIANTS;
            art_ptr = &select_art(*variants, force_small, max_y, max_x);
        }

        // Create Animation Instance
        Logo* raw_ptr = nullptr;

        if (art == 2 || art == 3 || art == 4 || art == 5 || art == 6 || art == 7) {
            stored_art = dynamic_art;
            if (cat == 0) raw_ptr = new BouncingAsciiLogo(stored_art);
            else if (cat == 1) raw_ptr = new RippleAsciiLogo(stored_art);
            else if (cat == 2) raw_ptr = new HeartbeatAsciiLogo(stored_art);
            else if (cat == 3) raw_ptr = new RotatingLineLogo(stored_art, mgr.settings.sonar_color_interval);
            else if (cat == 4) raw_ptr = new ScanningAsciiLogo(stored_art, mgr.context_scan_direction);
            else if (cat == 5) raw_ptr = new TypewriterAsciiLogo(stored_art, mgr.context_typewriter_direction);
        } else {
            if (cat == 0) raw_ptr = new BouncingAsciiLogo(*art_ptr);
            else if (cat == 1) raw_ptr = new RippleAsciiLogo(*art_ptr);
            else if (cat == 2) raw_ptr = new HeartbeatAsciiLogo(*art_ptr);
            else if (cat == 3) raw_ptr = new RotatingLineLogo(*art_ptr, mgr.settings.sonar_color_interval);
            else if (cat == 4) raw_ptr = new ScanningAsciiLogo(*art_ptr, mgr.context_scan_direction);
            else if (cat == 5) raw_ptr = new TypewriterAsciiLogo(*art_ptr, mgr.context_typewriter_direction);
        }

        app = raw_ptr;

        // Setup Dynamic Generator if needed
        if (app && (art == 3 || art == 5 || art == 6 || art == 7)) { // Dynamic Noise / Fading Block / Static Fading / Dynamic Fading
            AsciiLogo* ascii_logo = dynamic_cast<AsciiLogo*>(app);
            if (ascii_logo) {
                int noise_w = max_x;
                int noise_h = max_y;

                // Use DYNAMIC settings specifically here
                if (cat == 0 || mgr.settings.global_dynamic_noise_size) {
                    if (art == 5) {
                        noise_w = (max_x * mgr.settings.fading_block_w_percent) / 100;
                        noise_h = (max_y * mgr.settings.fading_block_h_percent) / 100;
                    } else if (art == 3 || art == 7) {
                        noise_w = (max_x * mgr.settings.dynamic_noise_percent_w) / 100;
                        noise_h = (max_y * mgr.settings.dynamic_noise_percent_h) / 100;
                    } else if (art == 6) { // STATIC FADING uses static sizing
                         noise_w = (max_x * mgr.settings.noise_percent_w) / 100;
                         noise_h = (max_y * mgr.settings.noise_percent_h) / 100;
                    }

                    if (noise_w < 1) noise_w = 1;
                    if (noise_h < 1) noise_h = 1;
                }

                // Shared Fader Logic Helper
                auto create_fader = [&](int w, int h) {
                    // Use shared settings
                    return std::make_shared<CellFader>(w, h, 
                                                       mgr.settings.noise_fading_low, 
                                                       mgr.settings.noise_fading_high, 
                                                       mgr.settings.noise_fading_change_chance);
                };

                // Capture settings by value for lambdas
                if (cat == 0) { // Kinetic Bounce - Regenerate Full Art
                    if (art == 5) {
                        // SOLID BLOCK FADING (Bounce)
                        auto fader = std::make_shared<CellFader>(noise_w, noise_h, 
                                                                 mgr.settings.fading_low_threshold, 
                                                                 mgr.settings.fading_high_threshold, 
                                                                 mgr.settings.fading_change_chance);
                        char sym = mgr.settings.fading_block_symbol;

                        ascii_logo->set_art_generator([fader, noise_w, noise_h, sym]() {
                            std::vector<std::string> buffer;
                            buffer.reserve(noise_h);
                            for (int r = 0; r < noise_h; ++r) {
                                std::string line = "";
                                line.reserve(noise_w);
                                for (int c = 0; c < noise_w; ++c) {
                                    line += (fader->update(r, c) ? sym : ' ');
                                }
                                buffer.push_back(line);
                            }
                            return buffer;
                        });
                    } else if (art == 7) { // DYNAMIC FADING
                         auto fader = create_fader(noise_w, noise_h);
                         ascii_logo->set_art_generator([fader, noise_w, noise_h]() {
                             std::vector<std::string> buffer;
                             buffer.reserve(noise_h);
                             const std::string chars = "!\"#$%&'()*+,-./:;<>=?&[]\\^|}{~€ƒ‡—";
                             for (int r = 0; r < noise_h; ++r) {
                                 std::string line = "";
                                 line.reserve(noise_w);
                                 for (int c = 0; c < noise_w; ++c) {
                                     bool visible = fader->update(r, c);
                                     if (visible) {
                                         line += chars[rand() % chars.length()];
                                     } else {
                                         line += ' ';
                                     }
                                 }
                                 buffer.push_back(line);
                             }
                             return buffer;
                         });

                    } else if (art == 6) { // STATIC FADING
                        std::vector<std::string> base_art = dynamic_art; 
                        auto fader = create_fader(noise_w, noise_h);
                        
                        ascii_logo->set_art_generator([fader, base_art, noise_w, noise_h]() {
                             std::vector<std::string> buffer;
                             buffer.reserve(noise_h);
                             for (int r = 0; r < noise_h; ++r) {
                                 std::string line = "";
                                 line.reserve(noise_w);
                                 for (int c = 0; c < noise_w; ++c) {
                                     bool visible = fader->update(r, c);
                                     // Use character from base_art
                                     if (visible) {
                                         if (r < (int)base_art.size() && c < (int)base_art[r].length())
                                             line += base_art[r][c];
                                         else 
                                             line += '?'; // Fallback
                                     } else {
                                         line += ' ';
                                     }
                                 }
                                 buffer.push_back(line);
                             }
                             return buffer;
                         });

                    } else { // Normal Dynamic Noise (art == 3)
                         int space_pct = mgr.settings.dynamic_noise_space_percent;
                         ascii_logo->set_art_generator([=]() {
                             return generate_noise_art(noise_w, noise_h, space_pct);
                         });
                    }
                } else { // Ripple/Heartbeat - Regenerate per Cell
                    if (art == 5) { // SOLID BLOCK FADING LOGIC
                        auto fader = std::make_shared<CellFader>(ascii_logo->get_width(), ascii_logo->get_height(), 
                                                                 mgr.settings.fading_low_threshold, 
                                                                 mgr.settings.fading_high_threshold, 
                                                                 mgr.settings.fading_change_chance);
                        char sym = mgr.settings.fading_block_symbol;
                        ascii_logo->set_cell_generator([fader, sym](int r, int c) -> char {
                            return fader->update(r, c) ? sym : ' ';
                        });

                    } else if (art == 7)  { // DYNAMIC FADING
                         auto fader = create_fader(ascii_logo->get_width(), ascii_logo->get_height());
                         ascii_logo->set_cell_generator([fader](int r, int c) -> char {
                              bool visible = fader->update(r, c);
                              if (!visible) return ' ';
                              static const std::string chars = "!\"#$%&'()*+,-./:;<>=?&[]\\^|}{~€ƒ‡—";
                              return chars[rand() % chars.length()];
                         });

                    } else if (art == 6) { // STATIC FADING
                         std::vector<std::string> base_art = dynamic_art;
                         auto fader = create_fader(ascii_logo->get_width(), ascii_logo->get_height());
                         
                         ascii_logo->set_cell_generator([fader, base_art](int r, int c) -> char {
                             bool visible = fader->update(r, c);
                             if (!visible) return ' ';
                             if (r < (int)base_art.size() && c < (int)base_art[r].length())
                                 return base_art[r][c];
                             return ' ';
                         });
                    
                    } else if (art == 3) {
                        // Dynamic Noise (Random Chars)
                        int space_pct = mgr.settings.dynamic_noise_space_percent;
                        ascii_logo->set_cell_generator([space_pct](int, int) -> char {
                            if (rand() % 100 < space_pct) return ' ';
                            static const std::string chars = "!\"#$%&'()*+,-./:;<>=?&[]\\^|}{~€ƒ‡—";
                            return chars[rand() % chars.length()];
                        });
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

AnimationState::~AnimationState() {
    if (app) delete app;
}

void AnimationState::handle_input(int ch, StateManager& mgr) {
    if (ch == 'q' || ch == 'Q' || ch == KEY_LEFT) {
        mgr.pop_state();
    }
}

void AnimationState::update(StateManager& /*mgr*/) {
    if (app) {
        int max_x, max_y;
        getmaxyx(stdscr, max_y, max_x);
        app->update(max_y, max_x);
    }
}

void AnimationState::draw(StateManager& /*mgr*/) {
    if (app) app->draw();
}
