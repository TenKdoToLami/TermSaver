#include "Menu.hpp"
#include <ncurses.h>
#include <vector>
#include <string>
#include <unistd.h>

void draw_menu_static(const std::string& title, const std::vector<std::string>& options, int choice) {
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
    attron(A_BOLD | A_UNDERLINE);
    
    std::string display_title = title;
    int max_title_len = box_width - 4; // 2 chars padding on each side
    if ((int)display_title.length() > max_title_len) {
        display_title = "..." + display_title.substr(display_title.length() - (max_title_len - 3));
    }
    
    mvprintw(start_y + 1, center_x - (display_title.length()/2), "%s", display_title.c_str());
    attroff(A_BOLD | A_UNDERLINE);
    
    // Header Separator
    mvhline(start_y + 2, start_x + 1, ACS_HLINE, box_width - 2);
    
    // Footer Separator
    mvhline(start_y + box_height - 2, start_x + 1, ACS_HLINE, box_width - 2);
    
    // Count Indicator (X/Total)
    std::string count_str = std::to_string(choice + 1) + "/" + std::to_string(options.size());
    mvprintw(start_y + box_height - 1, center_x - (count_str.length()/2), "%s", count_str.c_str());

    // Draw Options "Wheel"
    int list_top = start_y + 3;
    int list_bottom = start_y + box_height - 3;
    int list_center_y = list_top + ((list_bottom - list_top) / 2);

    // Draw Highlight Block (Center row + padding rows above/below)
    attron(A_BOLD | A_REVERSE);
    for (int y = list_center_y - 1; y <= list_center_y + 1; ++y) {
        if (y >= list_top && y <= list_bottom) {
             mvhline(y, start_x + 1, ' ', box_width - 2);
        }
    }
    attroff(A_BOLD | A_REVERSE);

    for (int i = 0; i < (int)options.size(); ++i) {
        int offset = i - choice;
        
        int rel_y = 0;
        if (offset > 0) rel_y = offset + 1;
        else if (offset < 0) rel_y = offset - 1;
        
        int item_y = list_center_y + rel_y;
        
        if (item_y >= list_top && item_y <= list_bottom) {
            if (offset == 0) {
                // Selected Item - Must match the background highlight
                attron(A_BOLD | A_REVERSE);
                std::string label = "--> " + options[i] + " <--"; 
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
}    

