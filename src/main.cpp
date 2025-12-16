#include <ncurses.h>
#include <clocale>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include "Logo.hpp"
#include "BouncingAsciiLogo.hpp"
#include "RotatingLineLogo.hpp"
#include "Menu.hpp"
#include "Art.hpp"

// Define Art Data that was declared in Art.hpp
const std::vector<std::string> BARTY_ART = {
    "  ____    _    ____  _____ __   __  ",
    " | __ )  / \\  |  _ \\|_   _|\\ \\ / /  ",
    " |  _ \\ / _ \\ | |_) | | |   \\ V /   ",
    " | |_) / ___ \\|  _ <  | |    | |    ",
    " |____/_/   \\_\\_| \\_\\ |_|    |_|    "
};

const std::vector<std::string> POP_ART = {
    "             /////////////             ",
    "         /////////////////////         ",
    "      ///////*7676////////////////     ",
    "    //////7676767676*//////////////    ",
    "   /////76767//7676767//////////////   ",
    "  /////767676///76767///////////////   ",
    " ///////767676///76767.///7676*/////// ",
    "/////////767676//76767///767676////////",
    "//////////76767676767////76767/////////",
    "///////////76767676//////7676//////////",
    "////////////,7676,///////767///////////",
    "/////////////*7676///////76////////////",
    "///////////////7676////////////////////",
    "///////////////7676///767////////////  ",
    "  //////////////////////'////////////  ",
    "   //////.7676767676767676767,//////   ",
    "    /////767676767676767676767/////    ",
    "      ///////////////////////////      ",
    "         /////////////////////         ",
    "             /////////////             "
};

int main() {
    setlocale(LC_ALL, ""); // Enable UTF-8
    srand(time(NULL));

    // Ncurses Initialization
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE); 
    start_color();
    use_default_colors();

    if (!can_change_color() && COLORS < 8) {
        // Fallback for limited terminals could go here
    }

    // Initialize Color Pairs (1 to 255)
    int max_c = COLORS > 255 ? 255 : COLORS - 1;
    for (int i = 1; i <= max_c; ++i) {
        init_pair(i, i, COLOR_BLACK);
    }

    // Main Application Loop
    while (true) {
        // 1. Show Menu
        // Ensure blocking input for menu
        timeout(-1); 
        int selection = show_menu();
        
        // Check for exit condition (Left Arrow in Menu)
        if (selection == -1) {
            break; // Exit the App
        }
        
        // 2. Prepare Animation
        timeout(0); // Switch to non-blocking input handling for animation

        Logo* app = nullptr;
        switch(selection) {
            case 0: app = new BouncingAsciiLogo(BARTY_ART); break;
            case 1: app = new BouncingAsciiLogo(POP_ART); break;
            case 2: app = new RotatingLineLogo(); break;
            default: app = new BouncingAsciiLogo(BARTY_ART); break;
        }

        int max_x, max_y;
        getmaxyx(stdscr, max_y, max_x);
        app->init_position(max_y, max_x);

        // 3. Animation Loop
        bool back_to_menu = false;
        while (!back_to_menu) {
            clear();
            getmaxyx(stdscr, max_y, max_x);

            app->update(max_y, max_x);
            app->draw();

            refresh();

            int ch = getch();
            // Q or Left Arrow to go back
            if (ch == 'q' || ch == 'Q' || ch == KEY_LEFT) {
                back_to_menu = true;
            }

            usleep(100000); // ~10 FPS
        }

        // Cleanup current animation
        delete app;
        
        // Loop continues -> Show Menu again
    }

    endwin();

    return 0;
}
