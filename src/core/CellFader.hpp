#pragma once
#include <vector>

/**
 * @class CellFader
 * @brief Helper class to manage the probabilistic fading state of individual cells.
 */
class CellFader {
public:
    CellFader(int w, int h, int low_thresh, int high_thresh, int chance);

    /**
     * @brief Updates the state of a specific cell and returns whether it should be visible.
     * 
     * @param r Row index
     * @param c Column index
     * @return true if visible, false if hidden
     */
    bool update(int r, int c);

private:
    int width, height;
    int low_threshold, high_threshold, change_chance;
    std::vector<std::vector<bool>> visibility;
    int total_cells;
    int visible_cells;
    bool is_reappearing_mode;
};
