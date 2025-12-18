#include "StateManager.hpp"

StateManager::StateManager() : running(true) {}


void StateManager::push_state(std::unique_ptr<State> state) {
    states.push(std::move(state));
}

void StateManager::pop_state() {
    if (!states.empty()) {
        states.pop();
    }
}

void StateManager::change_state(std::unique_ptr<State> state) {
    pop_state();
    push_state(std::move(state));
}

State* StateManager::current_state() {
    if (states.empty()) return nullptr;
    return states.top().get();
}
