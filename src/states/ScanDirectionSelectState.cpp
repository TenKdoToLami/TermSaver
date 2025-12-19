/**
 * @file ScanDirectionSelectState.cpp
 * @brief Implementation of the scan direction selection menu.
 */

#include "ScanDirectionSelectState.hpp"
#include "LogoSelectState.hpp"

ScanDirectionSelectState::ScanDirectionSelectState() : MenuState("SCAN DIRECTION", {
    "Top Left",
    "Top",
    "Top Right",
    "Right",
    "Bottom Right",
    "Bottom",
    "Bottom Left",
    "Left"
}) {}

void ScanDirectionSelectState::on_select(StateManager& mgr, int index) {
    // Store selected direction in context
    mgr.context_scan_direction = index;
    
    // Transition to logo selection for the scan effect
    mgr.push_state(std::make_unique<LogoSelectState>("SCAN", "SCAN EFFECT"));
}

void ScanDirectionSelectState::on_back(StateManager& mgr) {
    mgr.pop_state();
}
