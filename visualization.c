#include "visualization.h"
#include "config.h"
#include "utils.h"
#include "pheromones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Console initialization and management
void init_console(void) {
    // Get console handle
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (console == INVALID_HANDLE_VALUE) {
        print_error("Failed to get console handle");
        return;
    }
    
    // Set console size
    COORD size;
    size.X = 120;
    size.Y = 40;
    SetConsoleScreenBufferSize(console, size);
    
    // Set window size
    SMALL_RECT window;
    window.Left = 0;
    window.Top = 0;
    window.Right = 119;
    window.Bottom = 39;
    SetConsoleWindowInfo(console, TRUE, &window);
    
    // Hide cursor
    hide_cursor();
    
    // Clear screen
    clear_screen();
    
    print_info("Console initialized successfully");
}

void cleanup_console(void) {
    show_cursor();
    clear_screen();
}

void set_color(int color) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (console != INVALID_HANDLE_VALUE) {
        SetConsoleTextAttribute(console, (WORD)color);
    }
}

void clear_screen(void) {
    system("cls");
}

void hide_cursor(void) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (console != INVALID_HANDLE_VALUE) {
        CONSOLE_CURSOR_INFO cursor_info;
        GetConsoleCursorInfo(console, &cursor_info);
        cursor_info.bVisible = FALSE;
        SetConsoleCursorInfo(console, &cursor_info);
    }
}

void show_cursor(void) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (console != INVALID_HANDLE_VALUE) {
        CONSOLE_CURSOR_INFO cursor_info;
        GetConsoleCursorInfo(console, &cursor_info);
        cursor_info.bVisible = TRUE;
        SetConsoleCursorInfo(console, &cursor_info);
    }
}

// World rendering
void render_world(const World* world) {
    if (world == NULL) return;
    
    clear_screen();
    render_border(world);
    
    // Render each cell
    for (int y = 0; y < world->height; y++) {
        for (int x = 0; x < world->width; x++) {
            render_cell(&world->grid[y][x], x, y, world);
        }
    }
    
    // Render statistics below the world
    render_statistics(world);
    render_legend();
    render_controls();
}

void render_cell(const Cell* cell, int x, int y, const World* world) {
    if (cell == NULL || world == NULL) return;
    
    // Check if there's an ant at this position
    Ant* ant_at_position = NULL;
    for (int i = 0; i < world->colony_count; i++) {
        Ant* current = world->colonies[i].ants_head;
        while (current != NULL) {
            if (current->pos.x == x && current->pos.y == y && !(current->state & ANT_STATE_DEAD)) {
                ant_at_position = current;
                break;
            }
            current = current->next;
        }
        if (ant_at_position) break;
    }
    
    // Position cursor for this cell
    gotoxy(x + 2, y + 2); // +2 for border offset
    
    if (ant_at_position) {
        // Render ant
        render_ant(ant_at_position, x, y);
    } else {
        // Render terrain and pheromones
        char symbol = get_terrain_symbol(cell->terrain);
        int color = get_terrain_color(cell->terrain);
        
        // If empty cell, show pheromone intensity
        if (cell->terrain == TERRAIN_EMPTY) {
            float max_pheromone = (cell->pheromone_food > cell->pheromone_home) ? 
                                  cell->pheromone_food : cell->pheromone_home;
            if (max_pheromone > 0.0f) {
                symbol = get_pheromone_symbol(max_pheromone);
                color = get_pheromone_color(max_pheromone);
            }
        }
        
        set_color(color);
        printf("%c", symbol);
    }
}

void render_ant(const Ant* ant, int x, int y) {
    if (ant == NULL) return;
    
    char symbol = get_ant_symbol(ant);
    int color = get_colony_color(ant->colony_id);
    
    set_color(color);
    printf("%c", symbol);
}

void render_border(const World* world) {
    if (world == NULL) return;
    
    set_color(COLOR_WHITE);
    
    // Top border
    printf("╔");
    for (int i = 0; i < world->width; i++) {
        printf("═");
    }
    printf("╗\n");
    
    // Side borders and world content
    for (int y = 0; y < world->height; y++) {
        printf("║");
        // World content will be rendered here by render_cell
        for (int x = 0; x < world->width; x++) {
            printf(" "); // Placeholder, will be overwritten
        }
        printf("║\n");
    }
    
    // Bottom border
    printf("╚");
    for (int i = 0; i < world->width; i++) {
        printf("═");
    }
    printf("╝\n");
}

// Statistics and information display
void render_statistics(const World* world) {
    if (world == NULL) return;
    
    set_color(COLOR_WHITE);
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                            SIMULATION STATISTICS                           ║\n");
    printf("╠══════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ Step: %-6d | Status: %-8s | Delay: %-4d ms                              ║\n", 
           world->current_step, 
           world->paused ? "PAUSED" : "RUNNING",
           world->render_delay_ms);
    
    // Render colony information
    for (int i = 0; i < world->colony_count; i++) {
        render_colony_info(&world->colonies[i], i);
    }
    
    printf("╚══════════════════════════════════════════════════════════════════════════════╝\n");
}

void render_colony_info(const Colony* colony, int row) {
    if (colony == NULL) return;
    
    set_color(get_colony_color(colony->id));
    printf("║ Colony %d: Food: %-4d | Ants: %-2d/%-2d | Efficiency: %-6.2f | Color: ", 
           colony->id, 
           colony->food_collected, 
           colony->active_ants, 
           colony->total_ants,
           colony->efficiency_score);
    
    // Show color sample
    set_color(colony->color);
    printf("██");
    set_color(COLOR_WHITE);
    printf("                    ║\n");
}

void render_legend(void) {
    printf("\n");
    printf("LEGEND:\n");
    printf("N = Nest (colony home)  • = Ant without food  ● = Ant carrying food\n");
    printf("F = Food source         █ = Wall/Obstacle     ░▒▓ = Pheromone intensity\n");
    printf("Colors: Different colonies have different colors\n");
}

void render_controls(void) {
    printf("\n");
    printf("CONTROLS:\n");
    printf("SPACE = Pause/Resume  S = Save  L = Load  Q = Quit  +/- = Speed  R = Reset\n");
}

// Color management
int get_terrain_color(TerrainType terrain) {
    switch (terrain) {
        case TERRAIN_EMPTY: return COLOR_WHITE;
        case TERRAIN_WALL: return COLOR_BRIGHT_WHITE;
        case TERRAIN_FOOD: return COLOR_BRIGHT_GREEN;
        case TERRAIN_NEST: return COLOR_BRIGHT_YELLOW;
        case TERRAIN_WATER: return COLOR_BRIGHT_CYAN;
        default: return COLOR_WHITE;
    }
}

int get_colony_color(int colony_id) {
    // Return different colors for different colonies
    switch (colony_id % 8) {
        case 0: return COLOR_BRIGHT_RED;
        case 1: return COLOR_BRIGHT_BLUE;
        case 2: return COLOR_BRIGHT_GREEN;
        case 3: return COLOR_BRIGHT_MAGENTA;
        case 4: return COLOR_BRIGHT_CYAN;
        case 5: return COLOR_BRIGHT_YELLOW;
        case 6: return COLOR_BRIGHT_WHITE;
        case 7: return COLOR_BRIGHT_RED;
        default: return COLOR_WHITE;
    }
}

// get_pheromone_color function moved to pheromones.c to avoid duplicate definition

// Symbol management
char get_terrain_symbol(TerrainType terrain) {
    switch (terrain) {
        case TERRAIN_EMPTY: return ' ';
        case TERRAIN_WALL: return '█';
        case TERRAIN_FOOD: return 'F';
        case TERRAIN_NEST: return 'N';
        case TERRAIN_WATER: return '~';
        default: return '?';
    }
}

char get_ant_symbol(const Ant* ant) {
    if (ant == NULL) return '?';
    
    if (ant->food_carrying > 0) {
        return '●'; // Ant carrying food
    } else {
        return '•'; // Ant without food
    }
}

// Console positioning
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void set_console_size(int width, int height) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (console != INVALID_HANDLE_VALUE) {
        COORD size;
        size.X = width;
        size.Y = height;
        SetConsoleScreenBufferSize(console, size);
        
        SMALL_RECT window;
        window.Left = 0;
        window.Top = 0;
        window.Right = width - 1;
        window.Bottom = height - 1;
        SetConsoleWindowInfo(console, TRUE, &window);
    }
}
