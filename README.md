
# Las Vegas Style Slot Machine Game

## Project Overview

This repository will contain a full-featured Las Vegas-style slot machine game built primarily with C++. 
The game will deliver an authentic casino experience with realistic mechanics, engaging graphics, and immersive sound effects. (In-Progress)

Recent Screenshot:

<img width="640" height="354" alt="SlotMachine-Screenshot5" src="https://github.com/user-attachments/assets/31bd9d49-1b9b-4ef7-b85f-7ff3e5bd0408" />


## Features

- **Authentic Gameplay**: True-to-casino random number generation and payout algorithms
- **Multiple Reel Configurations**: Support for 3, 5, and 7-reel gameplay with configurable pay lines
- **Rich Bonus Features**: Free spins, multipliers, bonus games, and progressive jackpots
- **High-Quality Graphics**: Smooth animations and eye-catching visual effects
- **Immersive Audio**: Dynamic sound effects and background music
- **Player Statistics**: Comprehensive tracking of play history and win statistics
- **Configurable Settings**: Customizable bet sizes, autoplay, and game speed

## Technology Stack

- **Core Development**: C++17/C++20
- **Build System**: CMake
- **Graphics**: SFML (Simple and Fast Multimedia Library)
- **Audio**: OpenAL
- **UI Components**: Custom C++ UI system
- **Version Control**: Git/GitHub
- **IDE**: CLion
- **Testing**: Google Test Framework

## Getting Started

### Prerequisites

- CMake 3.15+
- C++17 compatible compiler (GCC 9+, Clang 10+, MSVC 19.20+)
- Git
- CLion IDE (recommended) or any C++ IDE

### Installation (The project is still ongoing, so please be patient. A Development roadmap is provided further below.)

1. Clone the repository:
   ```bash
   git clone https://https://github.com/AareonJackson/SlotMachineGame.git
   cd Slot-Machine
   ```

2. Initialize and update submodules:
   ```bash
   git submodule init
   git submodule update
   ```

3. Generate build files with CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

4. Build the project:
   ```bash
   cmake --build . --config Release
   ```

5. Run the executable:
   ```bash
   ./SlotMachine
   ```

### Using CLion

1. Open CLion
2. Select "Open" and navigate to the cloned project directory
3. CLion will automatically detect the CMake configuration
4. Click the build button or press Shift+F9 to build
5. Click the run button or press Shift+F10 to run

## Project Structure

```
slot-machine/
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
    ├── unit/                       # Unit tests
    └── integration/                # Integration tests
```

## Game Configuration

The game can be configured by editing the JSON files in the `assets/config/` directory:

- `game_config.json`: General game settings
- `reels_config.json`: Reel strips and symbol configurations
- `paylines_config.json`: Payline definitions
- `paytable_config.json`: Symbol payout values
- `features_config.json`: Bonus features configuration

Example configuration:

```json
{
    "game_name": "Lucky Vegas",
    "reel_count": 5,
    "row_count": 3,
    "payline_count": 20,
    "default_bet": 1.00,
    "min_bet": 0.20,
    "max_bet": 100.00,
    "rtp_percentage": 96.5
}
```

## Development Roadmap

- [x] Project setup and architecture design
- [x] Core game mechanics
- [x] Basic UI implementation
- [ ] Reel animations and graphics
- [ ] Sound effects and music integration
- [ ] Bonus features implementation
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
