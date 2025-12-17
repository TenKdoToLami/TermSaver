#pragma once

#include <string>
#include <vector>

/**
 * @brief Renders the static menu UI.
 * 
 * Draws a centered box with the title, paginated options using a wheel style,
 * and highlights the selected option. Handles truncation for long titles.
 *
 * @param title Display title for the menu box.
 * @param options List of menu options.
 * @param choice Currently selected index (0-based).
 */
void draw_menu_static(const std::string& title, const std::vector<std::string>& options, int choice);
