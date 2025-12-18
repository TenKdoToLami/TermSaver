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
- **Dynamic Noise**: A regenerating noise field for movement-based effects.
- **Noise Fading**: Probabilistic cell fading applied to both static and dynamic patterns.
- **Interactive Menu**:
    - **Slot Machine** scrolling style centered layout.
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
- `doxygen` (optional, for generating documentation)

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

## Documentation
The project is fully documented using Doxygen. To generate the HTML documentation:

```bash
make docs
```
The output will be available in the `docs/html/` directory. Open `docs/html/index.html` in your browser.

## Project Structure

- `src/core/`: Application loop (`main`), `StateManager`, `CellFader`, `AsciiArt`, `Menu` and static helpers.
- `src/logos/`: Visual effect implementations (`Logo` subclasses).
- `src/states/`: Individual state implementations (Menus, Screens, Settings) and State management.
- `CMakeLists.txt`: Build configuration.