#include "CellFader.hpp"
#include <cstdlib>

CellFader::CellFader(int w, int h, int low_thresh, int high_thresh, int chance)
    : width(w), height(h),
      low_threshold(low_thresh), high_threshold(high_thresh), change_chance(chance),
      visibility(h, std::vector<bool>(w, true)), // All visible initially
      total_cells(w * h), visible_cells(w * h), is_reappearing_mode(false) {}

bool CellFader::update(int r, int c) {
    if (r >= height || c >= width) return true;

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
    return visibility[r][c];
}
