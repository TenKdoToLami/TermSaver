#pragma once
#include <vector>
#include <cstdlib>

/**
 * @class CellFader
 * @brief Helper class to manage the probabilistic fading state of individual cells.
 */
class CellFader {
public:
    CellFader(int w, int h, int low_thresh, int high_thresh, int chance)
        : width(w), height(h),
          low_threshold(low_thresh), high_threshold(high_thresh), change_chance(chance),
          visibility(h, std::vector<bool>(w, true)), // All visible initially
          total_cells(w * h), visible_cells(w * h), is_reappearing_mode(false) {}

    /**
     * @brief Updates the state of a specific cell and returns whether it should be visible.
     * 
     * @param r Row index
     * @param c Column index
     * @return true if visible, false if hidden
     */
    bool update(int r, int c) {
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

private:
    int width, height;
    int low_threshold, high_threshold, change_chance;
    std::vector<std::vector<bool>> visibility;
    int total_cells;
    int visible_cells;
    bool is_reappearing_mode;
};
