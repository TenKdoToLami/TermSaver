#include "Menu.hpp"
#include <ncurses.h>
#include <vector>
#include <string>
#include <unistd.h>

int show_menu() {
    std::vector<std::string> options = {
        "Barty Logo (Standard)",
        "Pop!_OS Logo (Neofetch)",
        "Radar Sweep (Rainbow)"
    };
    int choice = 0;
    int max_choice = options.size() - 1;
    
    while (true) {
        clear();
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);
        int center_y = max_y / 2;
        int center_x = max_x / 2;
        
        // Dynamic Box Dimensions (2/3 of screen height)
        int box_width = 50; 
        int box_height = (max_y * 2) / 3;
        if (box_height < 10) box_height = 10; // Minimum safety height
        
        int start_y = center_y - (box_height / 2);
        int start_x = center_x - (box_width / 2);
        
        // Draw Box Outline
        attron(A_BOLD);
        // Horizontal lines
        mvhline(start_y, start_x, 0, box_width);
        mvhline(start_y + box_height, start_x, 0, box_width);
        // Vertical lines
        mvvline(start_y, start_x, 0, box_height);
        mvvline(start_y, start_x + box_width, 0, box_height);
        // Corners
        mvaddch(start_y, start_x, ACS_ULCORNER);
        mvaddch(start_y, start_x + box_width, ACS_URCORNER);
        mvaddch(start_y + box_height, start_x, ACS_LLCORNER);
        mvaddch(start_y + box_height, start_x + box_width, ACS_LRCORNER);
        attroff(A_BOLD);

        // Header inside box
        const char* title = "SELECT ANIMATION";
        attron(A_BOLD | A_UNDERLINE);
        mvprintw(start_y + 1, center_x - (std::string(title).length()/2), "%s", title);
        attroff(A_BOLD | A_UNDERLINE);
        
        // Header Separator
        mvhline(start_y + 2, start_x + 1, ACS_HLINE, box_width - 2);
        
        // Footer Separator
        mvhline(start_y + box_height - 2, start_x + 1, ACS_HLINE, box_width - 2);
        
        // Count Indicator (X/Total)
        std::string count_str = std::to_string(choice + 1) + "/" + std::to_string(options.size());
        mvprintw(start_y + box_height - 1, center_x - (count_str.length()/2), "%s", count_str.c_str());

        // Draw Options "Wheel"
        // List center logic: Selected item is ALWAYS at box visual center.
        // Visual center of LIST area (not box).
        // Area: start_y + 3 (top) to start_y + box_height - 3 (bottom, above footer)
        int list_top = start_y + 3;
        int list_bottom = start_y + box_height - 3;
        int list_center_y = list_top + ((list_bottom - list_top) / 2);

        for (int i = 0; i < (int)options.size(); ++i) {
            int offset = i - choice;
            
            // Custom Spacing Logic:
            // Offset 0: 0
            // Offset > 0: offset + 1 (gap of 1 line)
            // Offset < 0: offset - 1 (gap of 1 line)
            
            int rel_y = 0;
            if (offset > 0) rel_y = offset + 1;
            else if (offset < 0) rel_y = offset - 1;
            
            int item_y = list_center_y + rel_y;
            
            // Clipping: Only draw if within the content area
            if (item_y >= list_top && item_y <= list_bottom) {
                if (offset == 0) {
                    attron(A_BOLD | A_REVERSE);
                    std::string label = "--> " + options[i] + " <--"; // Padding + Arrows
                    mvprintw(item_y, center_x - (label.length()/2), "%s", label.c_str());
                    attroff(A_BOLD | A_REVERSE);
                } else {
                    mvprintw(item_y, center_x - (options[i].length()/2), "%s", options[i].c_str());
                }
            }
        }
        
        // Help Footer
        const char* help = "UP/DOWN: Move | ENTER/RIGHT: Select | Q/LEFT: Back/Exit";
        mvprintw(max_y - 2, (max_x - std::string(help).length())/2, "%s", help);
        
        refresh();
        
        int ch = getch();
        if (ch == KEY_UP) {
            choice--;
            if (choice < 0) choice = max_choice;
        } else if (ch == KEY_DOWN) {
            choice++;
            if (choice > max_choice) choice = 0;
        } else if (ch == KEY_RIGHT || ch == 10) { 
            return choice;
        } else if (ch == KEY_LEFT || ch == 'q' || ch == 'Q') {
            return -1;
        }
        
        usleep(10000); 
    }
}
