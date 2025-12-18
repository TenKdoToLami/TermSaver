#pragma once
#include "MenuState.hpp"
#include "StateManager.hpp"
#include "AsciiArt.hpp"
#include "Logo.hpp"
#include "BouncingAsciiLogo.hpp"
#include "RippleAsciiLogo.hpp"
#include "HeartbeatAsciiLogo.hpp"
#include "RotatingLineLogo.hpp"

// Forward declare specific animation state
class AnimationState;

/**
 * @class IntroState
 * @brief Simple "splash screen" state.
 *
 * Displays the application title (ASCII art) and author.
 * Waits for user to press Enter to proceed to the main menu.
 */
class IntroState : public State {
public:
    /**
     * @brief Handle input events.
     * Transitions to CategorySelectState on Enter.
     * @param ch Input character code.
     * @param mgr Reference to StateManager.
     */
    void handle_input(int ch, StateManager& mgr) override;

    /**
     * @brief Update state logic.
     * Currently a no-op for this static screen.
     * @param mgr Reference to StateManager.
     */
    void update(StateManager& mgr) override;

    /**
     * @brief Draw the splash screen to the terminal.
     * @param mgr Reference to StateManager.
     */
    void draw(StateManager& mgr) override;

    /**
     * @brief Indicates this state blocks execution (wait for input).
     * @return true
     */
    bool is_blocking() const override { return true; }
};

/**
 * @class LogoSelectState
 * @brief Menu for selecting the specific ASCII model (Barty, Pop_OS, Noise).
 */
class LogoSelectState : public MenuState {
public:
    LogoSelectState(const std::string& parent_path, const std::string& effect_name) 
        : MenuState(parent_path + "/" + effect_name, {"Barty", "Pop!_OS", "Static Noise", "Dynamic Noise", "Solid Block", "Solid Block Fading", "Back"}) {}
    
protected:
    void on_select(StateManager& mgr, int index) override;
    void on_back(StateManager& mgr) override {
        mgr.pop_state(); 
    }
};

/**
 * @class CategorySelectState
 * @brief Main Menu for selecting the visual effect category.
 */
class CategorySelectState : public MenuState {
public:
    CategorySelectState() : MenuState("EFFECTS", {
        "Kinetic Bounce",
        "Radial Ripple",
        "Cardio Pulse",
        "Sonar Sweep",
        "Settings",
        "Exit"
    }) {}

protected:
    void on_select(StateManager& mgr, int index) override;
    void on_back(StateManager& mgr) override;
};

/**
 * @class SettingsState
 * @brief Menu for configuring global application settings (FPS, etc).
 */
class SettingsState : public MenuState {
public:
    SettingsState(const std::string& path);
    
    /** @brief Handles value adjustment via Left/Right keys. */
    void handle_input(int ch, StateManager& mgr) override;

    /** @brief Draws the menu with live-updated value text. */
    void draw(StateManager& mgr) override;

protected:
    void on_select(StateManager& mgr, int index) override;
    void on_back(StateManager& mgr) override;
    
    void update_options_text(StateManager& mgr);
    std::string current_path;
};

/**
 * @class NoiseSettingsState
 * @brief Submenu for configuring noise specific settings (Static & Dynamic).
 * 
 * Allows the user to adjust width/height coverage and toggle global application
 * of these settings for the selected noise type.
 */
class NoiseSettingsState : public MenuState {
public:
    /**
     * @brief The type of noise being configured.
     */
    enum class NoiseType { STATIC, DYNAMIC };

    /**
     * @brief Construct a new Noise Settings State object.
     * 
     * @param path Parent menu path for breadcrumbs.
     * @param type The type of noise variables to modify.
     */
    NoiseSettingsState(const std::string& path, NoiseType type);
    
    void handle_input(int ch, StateManager& mgr) override;
    void draw(StateManager& mgr) override;

protected:
    void on_select(StateManager& mgr, int index) override;
    void on_back(StateManager& mgr) override;
    
    void update_options_text(StateManager& mgr);
    
    NoiseType type;
};

/**
 * @class SolidBlockSettingsState
 * @brief Submenu for configuring Solid Block settings.
 */
class SolidBlockSettingsState : public MenuState {
public:
    SolidBlockSettingsState(const std::string& path);
    
    void handle_input(int ch, StateManager& mgr) override;
    void draw(StateManager& mgr) override;

protected:
    void on_select(StateManager& mgr, int index) override;
    void on_back(StateManager& mgr) override;
    
    void update_options_text(StateManager& mgr);
};

/**
 * @class SolidBlockFadingSettingsState
 * @brief Menu for configuring the solid block fading effect.
 */
class SolidBlockFadingSettingsState : public MenuState {
public:
    explicit SolidBlockFadingSettingsState(const std::string& parent_path);

    void handle_input(int ch, StateManager& mgr) override;
    void draw(StateManager& mgr) override;

protected:
    void on_select(StateManager& mgr, int index) override;
    void on_back(StateManager& mgr) override;

private:
    void update_options_text(StateManager& mgr);
};

