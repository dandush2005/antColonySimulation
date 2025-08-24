# Ant Colony Optimization Simulator

A console-based simulation of ant colony optimization algorithms implemented in C, demonstrating emergent behavior through pheromone trails and collective pathfinding.

## Project Overview

This simulator recreates the natural behavior of ant colonies:
- **Ants** leave their nest searching for food randomly
- When an ant finds food, it returns to the nest following pheromone trails
- Ants deposit **pheromones** (chemical markers) as they travel
- Other ants preferentially follow stronger pheromone trails
- Over time, shorter paths accumulate more pheromones (because ants complete them faster)
- The colony collectively discovers the optimal path without central coordination

## Technical Requirements Met

This project demonstrates mastery of the following C programming concepts:

1. **Strings** - For file paths, menus, status messages
2. **Structs** - Multiple complex data structures (Ant, Cell, Colony, World)
3. **Dynamic Memory** - 2D array allocated at runtime for the world grid
4. **File I/O** - Save/load simulation states and export data
5. **Linked Lists** - For ant management within colonies
6. **Preprocessor Directives** - Configuration macros and debug mode
7. **Bitwise Operations** - For ant states and flags
8. **Sorting** - Quicksort implementation for ant efficiency ranking
9. **Searching** - Binary search for ant lookup

## Project Structure

```
antColonySimulation/
├── src/
│   ├── main.c                 # Entry point and main loop
│   ├── config.h              # All #define constants and configuration
│   ├── data_structures.h     # All struct definitions
│   ├── world.h/.c           # World grid management functions
│   ├── ant_logic.h/.c       # Ant behavior and movement
│   ├── pheromones.h/.c      # Pheromone calculations
│   ├── visualization.h/.c    # Console rendering
│   ├── file_io.h/.c         # Save/load functionality
│   ├── algorithms.h/.c       # Quicksort and binary search
│   ├── utils.h/.c           # Helper functions
│   ├── test_suite.h/.c      # Comprehensive test framework
│   ├── test_runner.c        # Test execution program
│   ├── multi_colony.h/.c    # Multiple colony support
│   └── map_editor.h/.c      # Interactive map editor
├── data/
│   ├── maps/                # Pre-made obstacle layouts
│   └── saves/               # Saved simulation states
├── AntColonySimulator.sln   # Visual Studio solution
└── README.md                # This file
```

## Building the Project

### Prerequisites
- Visual Studio 2022 (Community Edition is fine)
- Windows 10/11
- No external libraries required (uses only Windows.h and standard C library)

### Build Steps
1. Open `AntColonySimulator.sln` in Visual Studio 2022
2. Select your preferred configuration (Debug/Release) and platform (x86/x64)
3. Build the solution (Ctrl+Shift+B or Build → Build Solution)
4. The executable will be created in the output directory

### Alternative: Command Line Build
```bash
# Using MSBuild (if available in PATH)
msbuild AntColonySimulator.sln /p:Configuration=Release /p:Platform=x64

# Using Developer Command Prompt
cl /Fe:AntColonySimulator.exe src\*.c /I:src /link user32.lib
```

## Running the Simulation

1. **Launch**: Run the compiled executable
2. **Main Menu**: Choose from:
   - New Simulation
   - Load Simulation
   - Settings
   - Exit

3. **Simulation Controls**:
   - **SPACE**: Pause/Resume
   - **S**: Save simulation
   - **L**: Load simulation
   - **Q**: Quit
   - **+/-**: Speed up/down
   - **R**: Reset simulation

## Simulation Parameters

### World Settings
- **Default Size**: 60x30 cells
- **Maximum Size**: 100x100 cells
- **Terrain Types**: Empty, Wall, Food, Nest, Water

### Ant Behavior
- **Initial Energy**: 1000 steps
- **Energy per Step**: 1 unit
- **Food Energy Boost**: 500 units
- **Pheromone Following**: 80% probability
- **Random Exploration**: 20% probability

### Pheromone System
- **Deposit Amount**: 100 units
- **Evaporation Rate**: 2% per step
- **Diffusion Rate**: 1% to neighbors
- **Maximum Level**: 1000 units

## Visualization

The simulation renders in the console using:
- **N**: Nest (colony home)
- **F**: Food source
- **•**: Ant without food
- **●**: Ant carrying food
- **█**: Wall/Obstacle
- **░▒▓**: Pheromone intensity (weak to strong)
- **Colors**: Different colonies have different colors

## File Formats

### Save Files (.sav)
Binary format containing:
- World dimensions and terrain
- Colony information
- Ant positions and states
- Pheromone levels
- Simulation statistics

### Map Files (.map)
Text format for terrain layouts:
- ASCII representation of world
- Legend and metadata
- Easy to edit manually

### Statistics Files (.csv)
CSV format for data analysis:
- Timestamp, food collected, ant counts
- Efficiency metrics
- Path length tracking

## Key Algorithms

### Ant Movement Decision
```c
void decide_direction(Ant* ant, World* world) {
    if (ant->state & STATE_SEARCHING) {
        // Looking for food
        if (random() < FOLLOW_PROBABILITY) {
            follow_pheromone_gradient(ant, world, FOOD_PHEROMONE);
        } else {
            move_randomly(ant, world);
        }
        deposit_pheromone(ant, world, HOME_PHEROMONE);
    } else if (ant->state & STATE_RETURNING) {
        // Returning with food
        follow_pheromone_gradient(ant, world, HOME_PHEROMONE);
        deposit_pheromone(ant, world, FOOD_PHEROMONE);
    }
}
```

### Pheromone Following (Gradient Ascent)
```c
void follow_pheromone_gradient(Ant* ant, World* world, int type) {
    float max_pheromone = 0;
    int best_direction = -1;
    
    // Check all 8 neighboring cells
    for (int dir = 0; dir < 8; dir++) {
        int new_x = ant->x + dx[dir];
        int new_y = ant->y + dy[dir];
        
        if (is_valid_position(new_x, new_y)) {
            float pheromone = get_pheromone_intensity(world, new_x, new_y, type);
            if (pheromone > max_pheromone) {
                max_pheromone = pheromone;
                best_direction = dir;
            }
        }
    }
    
    if (best_direction >= 0) {
        move_ant(ant, world, best_direction);
    } else {
        move_randomly(ant, world);
    }
}
```

## Testing and Validation

The project includes comprehensive testing for:
- **Memory Management**: No memory leaks, proper allocation/deallocation
- **Linked Lists**: Add/remove ants, traverse lists, cleanup dead ants
- **Bitwise Operations**: Set/clear/toggle ant states, multiple states
- **File I/O**: Save/load integrity, data validation
- **Algorithms**: Sorting correctness, search accuracy
- **Simulation Stability**: No crashes, consistent behavior

### Running the Test Suite

The project includes a comprehensive test suite that verifies all C programming requirements:

```bash
# Run all tests
./AntColonySimulator --test-suite

# Run specific test categories
./AntColonySimulator --memory          # Dynamic memory tests
./AntColonySimulator --linked-lists    # Linked list operations
./AntColonySimulator --bitwise         # Bitwise operations
./AntColonySimulator --file-io         # File I/O tests
./AntColonySimulator --algorithms      # Algorithm tests
./AntColonySimulator --world           # World operations
./AntColonySimulator --ant-behavior    # Ant behavior tests
./AntColonySimulator --pheromones      # Pheromone system tests
./AntColonySimulator --benchmark       # Performance benchmarks

# With debug output
./AntColonySimulator --test-suite --debug --verbose
```

### Test Coverage

The test suite covers:
- **Dynamic Memory**: malloc/free, safe memory functions, memory leak detection
- **Linked Lists**: Ant colony management, path node operations
- **Bitwise Operations**: Ant state flags, state manipulation
- **File I/O**: Map export/import, statistics export, save/load
- **Algorithms**: Quicksort implementation, binary search, efficiency calculations
- **World Operations**: Grid creation, element placement, validation
- **Ant Behavior**: Movement, pheromone following, state transitions
- **Pheromone System**: Deposition, evaporation, diffusion
- **Performance**: Movement benchmarks, pheromone operations, file operations

## Performance Considerations

- **Rendering**: Only updates changed cells using dirty flags
- **Pheromone Updates**: Batched processing to avoid order dependencies
- **Ant Updates**: Efficient linked list traversal
- **Memory**: Minimal allocations during simulation runtime

## Phase 4 Features (COMPLETED)

### ✅ **Multiple Colony Support**
- **Competition**: Multiple ant colonies compete for food and territory
- **Interaction Types**: Neutral, competitive, cooperative, and aggressive colony relationships
- **Territory Management**: Dynamic territory calculation and conflict resolution
- **Colony Statistics**: Individual colony performance tracking and leaderboards
- **Visual Distinction**: Each colony has unique colors and identifiers

### ✅ **Interactive Map Editor**
- **Real-time Editing**: Interactive cursor-based map creation and modification
- **Multiple Tools**: Place colonies, food, obstacles, erase, fill areas, and select regions
- **Brush System**: Adjustable brush sizes for efficient editing
- **Grid & Coordinates**: Toggle grid display and coordinate labels
- **Auto-save**: Automatic backup of work in progress
- **Random Generation**: Procedural map generation with customizable density
- **File Operations**: Load, save, and export maps in various formats

### **Additional Features**
- **Statistics Tracking**: Comprehensive colony performance metrics
- **Parameter Tuning**: Real-time adjustment of simulation parameters
- **Performance Optimization**: Efficient rendering and update systems

## Future Enhancements

- Dynamic food sources
- Obstacle removal mechanics
- Energy boost cells
- Advanced pathfinding algorithms
- Performance profiling tools
- Web-based visualization

## Academic Requirements

This project fulfills the requirements for:
- **Advanced Computer Science Lab capstone** ✅
- **C programming mastery demonstration** ✅
- **Algorithm implementation and analysis** ✅
- **System design and architecture** ✅
- **Memory management and optimization** ✅

### **Phase Completion Status**
- ✅ **Phase 1: Core Foundation** - World grid, ant structures, console rendering, basic movement
- ✅ **Phase 2: Pheromone System** - Deposit, evaporation, gradient following, dual system
- ✅ **Phase 3: Required Algorithms** - Quicksort, binary search, file I/O, bitwise operations
- ✅ **Phase 4: Polish & Features** - Multiple colonies, map editor, statistics, parameter tuning

**All phases from the project brief have been successfully implemented!**

## License

This project is created for educational purposes as part of an academic capstone project.

## Contact

For questions or issues related to this academic project, please refer to your course instructor or teaching assistant.

---

**Note**: This simulator demonstrates emergent behavior in complex systems. The beauty of ant colony optimization lies in how simple individual behaviors (random exploration + pheromone following) lead to sophisticated collective intelligence (optimal pathfinding).
