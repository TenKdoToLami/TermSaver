/**
 * @file TypewriterDirectionSelectState.cpp
 * @brief Implementation of the typewriter direction selection menu.
 */

#include "TypewriterDirectionSelectState.hpp"
#include "LogoSelectState.hpp"

TypewriterDirectionSelectState::TypewriterDirectionSelectState() : MenuState("TYPEWRITER DIRECTION", {
    "Top Left",
    "Top",
    "Top Right",
    "Right",
    "Bottom Right",
    "Bottom",
    "Bottom Left",
    "Left"
}) {}

void TypewriterDirectionSelectState::on_select(StateManager& mgr, int index) {
    mgr.context_typewriter_direction = index;
    mgr.push_state(std::make_unique<LogoSelectState>("TYPEWRITER", "TYPEWRITER EFFECT"));
}

void TypewriterDirectionSelectState::on_back(StateManager& mgr) {
    mgr.pop_state();
}
