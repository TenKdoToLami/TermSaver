#include "AsciiLogo.hpp"

AsciiLogo::AsciiLogo(const std::vector<std::string>& art_data) : lines(art_data) {
    update_dimensions();
}

void AsciiLogo::set_art(const std::vector<std::string>& new_art) {
    lines = new_art;
    update_dimensions();
}

void AsciiLogo::set_art_generator(std::function<std::vector<std::string>()> gen) {
    art_generator = gen;
}

void AsciiLogo::set_cell_generator(std::function<char(int r, int c)> gen) {
    cell_generator = gen;
}

void AsciiLogo::on_color_change() {
    if (art_generator) {
        set_art(art_generator());
    }
}

int AsciiLogo::get_width() const {
    return width;
}

int AsciiLogo::get_height() const {
    return height;
}

void AsciiLogo::update_dimensions() {
    height = lines.size();
    width = 0;
    for (const auto& line : lines) {
        if ((int)line.length() > width) {
            width = line.length();
        }
    }
    // Initialize last_colors with -1 to force initial update
    last_colors.assign(height, std::vector<int>(width, -1));
}
