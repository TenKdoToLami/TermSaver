#include <ncurses.h>
#include <clocale>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include "StateManager.hpp"
#include "ConcreteStates.hpp"

/**
 * @brief Application Entry Point.
 * 
 * Initializes ncurses, sets up the color palette, and enters the main event loop
 * managed by StateManager.
 */
int main() {
    // 1. Setup Environment
    setlocale(LC_ALL, ""); // Enable UTF-8 for smooth drawing characters
    srand(time(NULL));     // Seed RNG

    // 2. Initialize ncurses
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE); 
    start_color();
    use_default_colors();

    // 3. Initialize Color Pairs (1-255)
    int max_c = COLORS > 255 ? 255 : COLORS - 1;
    for (int i = 1; i <= max_c; ++i) {
        init_pair(i, i, COLOR_BLACK);
    }

    // 4. Initialize State Manager
    StateManager app;
    app.push_state(std::make_unique<CategorySelectState>());

    // 5. Main Event Loop
    while (app.is_running()) {
        State* current = app.current_state();
        if (!current) break;

        // Configure input blocking based on state type
        if (current->is_blocking()) {
            timeout(-1); // Blocking (Menus)
        } else {
            timeout(0);  // Non-blocking (Animations)
        }

        // Render Frame
        erase(); 
        current->update(app);
        current->draw(app);
        refresh();

        // Handle Input
        int ch = getch();
        if (ch != ERR) {
            current->handle_input(ch, app);
        } else {
             // Frame Limiter for Animation States
             if (!current->is_blocking()) {
                 int sleep_us = 33333; // Default fallback
                 if (app.settings.target_fps > 0) {
                     sleep_us = 1000000 / app.settings.target_fps;
                 }
                 usleep(sleep_us); 
             }
        }
    }

    // 6. Cleanup
    endwin();
    return 0;
}
