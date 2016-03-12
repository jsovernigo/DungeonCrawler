/**
 *Author: Giuliano Sovernigo 0948924
 *
 *DUE: 3/13/2016
 *
 *Project A3, CIS*2500_W16
 *
 *  This file contains the definitions for functions that
 *are used in a rogue-like terminal game.  Their implementation
 *exists in the driver.c file.  These functions are used to set
 *up game elements, initialize structs, and then simply run the
 *game in a loop.
 *
 *
 */
#ifndef __GSOVERNIGO_DRIVER_H__
#define __GSOVERNIGO_DRIVER_H__

#define MIN_ROWS 45
#define MIN_COLS 80

#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "roomgen.h"
#include "entities.h"


/**
 *cleanup
 *deletes the structures that are used in the game.  Cleans up the window.
 *IN:   map and player, the game structures to be freed
 *OUT:  0 on success.
 *POST: map and player have been invalidated, ncurses has been terminated.
 *ERROR:no ncurses mode has been declared.
 */
int cleanup(Map* map, Player* player, Enemy* enemies[], int* numEnemies);


/**
 *combat
 *this function models combat between two structs.
 *IN:   player and enemy, the two structs that will fight.
 *OUT:  returns -1 on error, 0 on successful combat, and 1 on combat with victory
 *POST: player->health or enemy->health have been decreased.
 *ERROR:player or enemy are null (-1 returned)
 */
int combat(Player* player, Enemy* enemy);


/**
 *drawHalls
 *This function draws hallways to stdscr, such that all doors are hallway'd.
 *IN:   a pointer to the map that has been drawn to the field.
 *OUT:  0 on success, 1 on failure.
 *POST: hallways have been drawn to stdscr.
 *ERROR:map is invalid.
 */
int drawHalls(Map* map);


/**
 *drawMap
 *draws the map to the screen, and then grabs the player's position
 *IN:   map and player, the game structures to be drawn and located
 *OUT:  0 on success, -1 on failure.
 *POST: player->x, player->y have been modified.
 *ERROR:map or player are invalid (returns -1)
 */
int drawMap(Map* map, Player* player, Enemy* enemies[], int* numEnemies);


/**
 *enemyMove
 *this function causes the enemy to move.
 *IN:   the enemy to be moved
 *OUT:  0 on success, -1 on error
 *POST: enemy->x and enemy->y have been altered.
 *ERROR:enemy is null
 */
int enemyMove(Enemy* enemy);


/**
 *exit
 *This function exits the game, printing the victory message (or death)
 *IN:   the player and the win condition
 *OUT:  N/A
 *POST: screen has been cleared and the message has been written.
 *ERROR:player is NULL.
 */
void gameOver(Player* player, int win);

/**
 *findPlayer
 *locates the player that was drawn on the map, and then moves the cursor to it.
 *IN:   N/A
 *OUT:  N/A
 *POST: the cursor has been moved to the player's location.
 *ERROR:No player has been located, so cursor's location is undefined.
 */
void findPlayer();


/**
 *kill
 *This function kills an enemy, and removes it from the screen.
 *IN:   the enemy array, the number of enemies, and the enemy itself.
 *OUT:  -1 on error, 0 on success.
 *POST: enemy has been deleted, numEnemies has been decreased, enemies has been reordered, and has had an entitiy removed.
 *ERROR:any of the parameters are NULL
 */
int kill(Enemy* enemies[], int* numEnemies, Enemy* enemy);


/**
 *moveTo
 *moves the player to a new location, as specifed by command
 *IN:   the map, the player, and a command character, enemies the enemy array and the number of enemies.
 *OUT:  1 on successful move, 0 on successful move with a win
 *      condition, and -1 for a failed move.
 *POST: cursor has been moved as as the player to a new spot
 *ERROR:map or player are NULL.
 */
int moveTo(Map* map, Player* p, Enemy* enemies[], int* numEnemies, char command);


/**
 *readMap
 *this function takes in a file name and generates a map struct
 *  based off of its specifications.
 *IN:   the file path string.
 *OUT:  the malloced Map pointer, or NULL on error.
 *POST: a Map struct pointer has been malloc'd (so free it), and numEnemies has been set
 *ERROR:fname is invalid, or malloc failed to initialize map
 */
Map* readMap(char *fname);


/**
 *play
 *this is the main play function.  It loops and performs the game actions.
 *IN:   a map pointer, and the player pointer.
 *OUT:  1 on win, 0 on loss -1 on error (null structs)
 *POST: the game will have been played, and information will have been
 *      written to the ncurses terminal
 *ERROR:N/A
 */
int play(Map* map, Player* player, Enemy* enemies[], int* numEnemies);


/**
 *printMessage
 *This function moves the cursor to the top left of the screen and then prints
 *  msg there.
 *IN:   msg, the null-terminated string message.
 *OUT:  the number of characters written.
 *POST: msg has been written to stdscr.
 *ERROR:printw fails to print
 */
int printMessage(char* msg);


/**
 *printStats
 *This function is used to print the player's stats to the bottom row of the screen
 *IN:   the player struct whose stats will be printed
 *OUT:  the number of characters written or -1 on failure.
 *POST: a new line of text has been written to the bottom of the screen
 *ERROR:player is null (returns -1);
 */
int printStats(Player* player);


/**
 *setup
 *This function sets up the required structs
 *IN:   N/A
 *OUT:  0 on success.
 *POST: terminal has been set to ncurses mode.
 *ERROR:initscr failed.
 */
int setup(Map* map, Player* player, Enemy* enemies[], int* numEnemies);

/**
 *spawnHall
 *this function takes a y,x of a door, and a char for which face its on, then spawns a hallway.
 *IN:   doorY, doorX, the coordinates of the door, and face, either n,s,w,e.
 *OUT:  0 on success, -1 on failure.
 *POST: hallways have been written to stdscr.
 *ERROR:doorX and doorY are out of bounds, or face is not n,s,e,w.
 */
int spawnHall(int doorY, int doorX, char face);


#endif
