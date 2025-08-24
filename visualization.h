#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include "data_structures.h"

// Console initialization and management
void init_console(void);
void cleanup_console(void);
void set_color(int color);
void clear_screen(void);
void hide_cursor(void);
void show_cursor(void);

// World rendering
void render_world(const World* world);
void render_cell(const Cell* cell, int x, int y, const World* world);
void render_ant(const Ant* ant, int x, int y);
void render_border(const World* world);

// Statistics and information display
void render_statistics(const World* world);
void render_colony_info(const Colony* colony, int row);
void render_legend(void);
void render_controls(void);

// Color management
int get_terrain_color(TerrainType terrain);
int get_colony_color(int colony_id);
// get_pheromone_color declaration moved to pheromones.h

// Symbol management
char get_terrain_symbol(TerrainType terrain);
char get_ant_symbol(const Ant* ant);

// Console positioning
void gotoxy(int x, int y);
void set_console_size(int width, int height);

#endif // VISUALIZATION_H
