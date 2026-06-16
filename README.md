

# Las Vegas Style Slot Machine Game

## Project Overview

This is a C++ slot machine game inspired by classic Las Vegas-style slot machines.
The project uses **C++17**, **CMake**, and **SFML** for graphics, window handling, and audio. Game data such as reels, paylines, and payouts are loaded from JSON configuration files.

This project is still in progress.

Recent Screenshot:

<img width="640" height="354" alt="SlotMachine-Screenshot7" src="https://github.com/user-attachments/assets/efd92f1d-300c-4570-9716-38bb68bbb8f0" />


## Features

- **Slot Machine Gameplay**: Reel-based gameplay with configurable symbols, paylines, and payouts
- **Configurable Game Data**: JSON-driven game, reel, payline, and paytable settings
- **Bonus / Free Spins**: Free-spin trigger behavior based on configured symbols
- **Player Save Data**: Balance and session statistics can be saved and loaded
- **Custom UI**: SFML-based buttons, reels, and game window rendering
- **Audio Support**: SFML audio integration
- **Testing Support**: CMake test target for core logic and save/load behavior

## Technology Stack

- **Core Development**: C++17
- **Build System**: CMake 3.15+
- **Graphics**: SFML (Simple and Fast Multimedia Library)
- **Audio**: SFML Audio / OpenAL Backend
- **UI Components**: Custom C++ UI system
- **Version Control**: Git/GitHub
- **IDE**: CLion recommended, but not required
- **Testing**: CMake / CTest with project executables

---
## Getting Started
### Prerequisites
Before building the project, make sure you have:

- **CMake 3.15+**
- **A C++17 compatible compiler**
   - GCC 9+
   - Clang 10+
   - MSVC 19.20+
- **Git**
- **OpenAL**
- **CLion IDE** recommended but not required

SFML is used for graphics, window handling, and audio.
The project can download/build SFML through CMake, but your system may still need OpenAL installed for audio support.
---
### Installation (The project is still ongoing, so please be patient. A Development roadmap is provided further below.)

### 1. Clone the repository:
   ```bash
   git clone https://github.com/AareonJackson/SlotMachineGame.git
   cd SlotMachineGame
   ```

### 2. Configure the Project:
   ```bash
   cmake -S . -B build
   ```
   Or if you have Ninja installed, you can use:
   ```bash
   cmake -S . -B build -G Ninja
   ```

### 3. Build the Project:
   ```bash
   cmake --build build
   ```
   For a release build:
   ```bash
   cmake --build build --config Release
   ```

### 4. Run the Game:
The game needs access to the `assets/` folder at runtime.

The recommended way is to run the game from the project root after building.
### macOS / Linux
   ```bash
   ./build/bin/SlotMachine
   ```
### Windows

If using Visual Studio-style builds:
   ```bash
   .\build\bin\SlotMachine.exe
   ```
   or:
   ```bash
   build\bin\Release\SlotMachine.exe
   ```
If using Ninja or Make:
   ```bash
   build\bin\SlotMachine.exe
   ```

---
### Using CLion

1. Open CLion
2. Select **Open**.
3. Choose the cloned `SlotMachineGame` folder.
4. Wait for CLion to load the CMake project.
5. Click the **Build** button.
6. Click the **Run** button.

If CLion asks for a CMake profile, the default profile is usually fine.

---

## Project Structure

```
SlotMachineGame/
├── CMakeLists.txt                  # Main CMake configuration
├── assets/                         # Game assets
│   ├── audio/                      # Sound effects and music
│   ├── config/                     # Game configuration files
│   ├── fonts/                      # Text fonts
│   └── graphics/                   # Images and animations
├── docs/                           # Documentation
├── external/                       # Third-party libraries
├── src/                            # Source code
│   ├── core/                       # Core game mechanics
│   │   ├── engine.cpp              # Main game engine
│   │   ├── rng.cpp                 # Random number generator
│   │   └── payout.cpp              # Payout calculator
│   ├── ui/                         # User interface components
│   ├── audio/                      # Audio system
│   ├── config/                     # Configuration management
│   └── main.cpp                    # Application entry point
└── tests/                          # Test suite
    ├── docs/                       # Test documentation
    ├── unit/                       # Unit tests
    └── integration/                # Integration tests
```

## Game Configuration

The game can be configured by editing the JSON files in the `assets/config/` directory:

Important configuration files include:

- `game_config.json`
- `reels_config.json`
- `paylines_config.json`
- `paytable_config.json`

These files control things like:

- Reel layout
- Symbols
- Paylines
- Payout values
- Bet settings

---

## Development Roadmap

- [x] Project setup and architecture design
- [x] Core game mechanics
- [x] Basic UI implementation
- [ ] Reel animations and graphics
- [ ] Sound effects and music integration
- [x] Bonus features implementation
- [ ] Statistics and history tracking
- [ ] Advanced visual effects
- [ ] Performance optimization
- [ ] Comprehensive testing
- [ ] Release build and deployment


## Acknowledgments

- Inspired by classic Las Vegas slot machines
- Thanks to the SFML and OpenAL communities for their excellent libraries
- Special thanks to all contributors and testers

---

**Note:** This game is intended for entertainment purposes only and does not involve real gambling or money.
