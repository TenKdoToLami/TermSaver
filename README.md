# Ncurses ASCII Screensaver

A terminal-based screensaver application written in C++ using `ncurses`. It features physics-based bouncing ASCII logos and algorithmic drawing effects.

## Features

- **Menu System**: Interactive selection menu with "slot machine" scrolling style.
- **Bouncing Logos**:
  - **Barty Logo**: Custom FIGlet-style text animation.
  - **Pop!_OS Logo**: Official Neofetch ASCII art.
  - *Generic Support*: Use `BouncingAsciiLogo` to animate any ASCII art.
- **Radar Sweep**: A `RotatingLineLogo` algorithm that creates a gap-free radar sweep with proper Hue-based rainbow color cycling.
- **Modular Architecture**: Clean separation of core logic, ASCII art data, and rendering implementations.

## Controls

### Menu
- **UP / DOWN**: Scroll through the list of animations.
- **RIGHT / ENTER**: Select and start the animation.
- **LEFT**: Exit the application.

### Animation
- **Q**: Quit the animation and return to the terminal.

## Build Instructions

### Prerequisites
- Linux / Unix environment
- C++17 compliant compiler (`g++`)
- `ncurses` library (`libncurses-dev` or similar)

### Compiling
Run `make` to compile the source code:

```bash
make
```

### Running
Start the application:

```bash
make run
```
Or directly:
```bash
./ascii_screensaver
```

## Project Structure

- `src/`: Source code (`.cpp`)
- `include/`: Header files (`.hpp`)
- `Makefile`: Build configuration