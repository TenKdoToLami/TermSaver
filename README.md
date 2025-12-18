# TermSaver

A premium terminal-based screensaver application written in C++ using `ncurses`. Featuring fluid physics-based kinetic animations, dynamic coloring, and algorithmic visual effects.

## Features

- **Kinetic Bounce**: Physics-based bouncing logos with responsive resizing logic.
- **Radial Ripple**: Hypnotic, expanding color waves.
- **Cardio Pulse**: Rhythmic, breathing heartbeat animations.
- **Sonar Sweep**: Gap-free radar sweep with interpolated rendering and rainbow trails.
- **Solid Block**: A retro-style bouncing block of solid characters.
- **Solid Block Fading**: Extensions to the Solid Block effect with probabilistic cell fading and reappearance.
- **Static Noise**: A configurable TV static effect using fixed characters.
- **Dynamic Noise**: A regenerating noise field that reacts to animation events (bounces, pulses).
- **Interactive Menu**:
    - **Slot Machine** scrolling style.
    - **Breadcrumb Navigation** (e.g., `SELECT EFFECT / SETTINGS / NOISE`).
    - **Settings Manager**: Control FPS, noise density, and global overrides.

## Controls

### Menu
- **UP / DOWN**: Navigate options.
- **RIGHT / ENTER**: Select / Confirm.
- **LEFT / Q**: Back / Exit.

### Settings
- **LEFT / RIGHT**: Adjust values (FPS, Percentages).
- **ENTER**: Toggle checkboxes.

### While Running (Animation)
- **Q / LEFT**: Stop animation and return to menu.

## Build Instructions

### Prerequisites
- Linux / Unix environment
- C++17 compliant compiler (`g++`)
- `cmake` (version 3.10+)
- `ncurses` library with wide-char support (`libncursesw5-dev` or similar)

### Compiling & Running
The project uses a simple wrapper `Makefile` for convenience.

```bash
# Build and Run
make run

# Clean Build
make clean
```

Or manually via CMake:
```bash
mkdir build && cd build
cmake ..
make
./termsaver
```

## Project Structure

- `src/core/`: Application loop (`main`), `StateManager`, and static helpers.
- `src/logos/`: Visual effect implementations (`Logo` subclasses).
- `src/states/`: Menu logic and State management classes.
- `include/`: Header files (if separated, currently merged in source tree for simplicity).
- `CMakeLists.txt`: Build configuration.