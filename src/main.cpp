#include <ncurses.h>
#include <clocale>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include "Logo.hpp"
#include "BouncingAsciiLogo.hpp"
#include "RippleAsciiLogo.hpp"
#include "HeartbeatAsciiLogo.hpp"
#include "RotatingLineLogo.hpp"
#include "Menu.hpp"
#include "AsciiArt.hpp"

int main() {
    setlocale(LC_ALL, ""); // Enable UTF-8
    srand(time(NULL));

    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE); 
    start_color();
    use_default_colors();

    if (!can_change_color() && COLORS < 8) {
        // Fallback
    }

    int max_c = COLORS > 255 ? 255 : COLORS - 1;
    for (int i = 1; i <= max_c; ++i) {
        init_pair(i, i, COLOR_BLACK);
    }

    while (true) {
        // 1. Category Selection
        timeout(-1); 
        std::vector<std::string> categories = {
            "Bouncing Logo",
            "Ripple Effect",
            "Heartbeat Pulse",
            "Radar Sweep"
        };
        
        int cat_sel = show_menu("SELECT CATEGORY", categories);
        if (cat_sel == -1) break; // Exit
        
        // 2. Art Selection
        int art_sel = 0;
        if (cat_sel != 3) {
            std::vector<std::string> art_options = {"Barty Logo", "Pop!_OS Logo"};
            art_sel = show_menu("SELECT LOGO", art_options);
            if (art_sel == -1) continue; 
        }

        // 3. Prepare Animation
        timeout(0); 
        Logo* app = nullptr;
        
        if (cat_sel == 3) { // Radar
            app = new RotatingLineLogo();
        } else {
            // Get Screen Dimensions for auto-sizing
            int max_x, max_y;
            getmaxyx(stdscr, max_y, max_x);
            
            // Determine Constraints
            bool force_small = (cat_sel == 0); // Bouncing -> Force Small
            const LogoVariants* variants = (art_sel == 0) ? &BARTY_VARIANTS : &POP_VARIANTS;
            
            // Select Art
            const std::vector<std::string>& art_data = select_art(*variants, force_small, max_y, max_x);
            
            if (cat_sel == 0) app = new BouncingAsciiLogo(art_data);
            else if (cat_sel == 1) app = new RippleAsciiLogo(art_data);
            else if (cat_sel == 2) app = new HeartbeatAsciiLogo(art_data);
        }
        
        if (app) {
            int max_x, max_y;
            getmaxyx(stdscr, max_y, max_x);
            app->init_position(max_y, max_x);

            bool back_to_menu = false;
            while (!back_to_menu) {
                clear();
                getmaxyx(stdscr, max_y, max_x);

                app->update(max_y, max_x);
                app->draw();

                refresh();

                int ch = getch();
                if (ch == 'q' || ch == 'Q' || ch == KEY_LEFT) {
                    back_to_menu = true;
                }

                usleep(100000); 
            }
            delete app;
        }
    }

    endwin();
    return 0;
}
