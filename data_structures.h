#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdint.h>

// Forward declarations
typedef struct Ant Ant;
typedef struct Colony Colony;
typedef struct World World;

// Position struct for coordinates
typedef struct {
    int x;
    int y;
} Position;

// Terrain types
typedef enum {
    TERRAIN_EMPTY = 0,
    TERRAIN_WALL,
    TERRAIN_FOOD,
    TERRAIN_NEST,
    TERRAIN_WATER
} TerrainType;

// Cell struct for world grid
typedef struct {
    TerrainType terrain;
    float pheromone_food;
    float pheromone_home;
    int food_amount;
    int colony_id;  // For nests
    int has_colony;  // Boolean flag for colony presence
    int has_food;    // Boolean flag for food presence
} Cell;

// Path node for tracking ant movement history
typedef struct PathNode {
    Position pos;  // Keep as pos to match usage in code
    Position position;  // Add alias for compatibility with existing code
    float pheromone_strength;
    struct PathNode* next;
} PathNode;

// Ant struct with linked list support
typedef struct Ant {
    int id;
    Position pos;  // Changed back to pos to match existing code usage
    Position last_pos;
    Position position;  // Add alias for compatibility with existing code
    uint8_t state;  // Bitwise flags for states
    int colony_id;
    float energy;
    int food_carrying;
    int steps_taken;
    int food_delivered;
    int food_collected; // Added to match usage in tests
    float pheromone_strength;
    float exploration_rate;
    struct Ant* next;  // Linked list pointer
    PathNode* path_history;
} Ant;

// Colony struct
typedef struct Colony {
    int id;
    Position nest_pos;  // Keep as nest_pos to match usage
    Position position;  // Also add position alias for some code that expects it
    int food_collected;
    int total_ants;
    int active_ants;
    Ant* ants_head;  // Keep as ants_head to match usage
    Ant* ants;      // Also add ants alias for some code that expects it
    int ant_count;  // Current ant count
    float efficiency_score;
    int color;  // For visualization
    int total_food_collected;  // Total food collected over time
    float total_distance_traveled;  // Total distance traveled by all ants
    float pheromone_strength;  // Colony pheromone strength
    float exploration_rate;  // Colony exploration rate
    int territory_size;  // Territory size in cells
} Colony;

// World struct containing the entire simulation
typedef struct World {
    int width;
    int height;
    Cell** grid;  // Dynamic 2D array
    Colony* colonies;
    int colony_count;
    int current_step;
    int is_running;
    int paused;
    int render_delay_ms;
} World;

#endif // DATA_STRUCTURES_H
