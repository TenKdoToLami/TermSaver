#pragma once
#include "MenuState.hpp"
#include "StateManager.hpp"

/**
 * @class CategorySelectState
 * @brief Main Menu for selecting the visual effect category.
 */
class CategorySelectState : public MenuState {
public:
    /**
     * @brief Construct a new Category Select State object.
     */
    CategorySelectState();

protected:
    void on_select(StateManager& mgr, int index) override;
    void on_back(StateManager& mgr) override;
};
