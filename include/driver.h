/**
 *Author: Giuliano Sovernigo 0948924
 *
 *DUE: 2/17/2016
 *
 *Project A2, CIS*2500_W16
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
 *changeRoom
 *This function is used to move the player to a new room on the map
 *IN:   map, the map that exists on the screen, and the player to move
 *OUT:  1 on successful room change, 0 on failure, -1 on error.
 *POST: player->room was modified, the cursor has been moved, and the
 *      player on the map has been moved.
 *ERROR:map or player is invalid.
 */
int changeRoom(Map* map, Player* player);


/**
 *cleanup
 *deletes the structures that are used in the game.  Cleans up the window.
 *IN:   map and player, the game structures to be freed
 *OUT:  0 on success.
 *POST: map and player have been invalidated, ncurses has been terminated.
 *ERROR:no ncurses mode has been declared.
 */
int cleanup(Map* map, Player* player);


/**
 *drawMap
 *draws the map to the screen, and then grabs the player's position
 *IN:   map and player, the game structures to be drawn and located
 *OUT:  0 on success, -1 on failure.
 *POST: player->x, player->y have been modified.
 *ERROR:map or player are invalid (returns -1)
 */
int drawMap(Map* map, Player* player);


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
 *moveTo
 *moves the player to a new location, as specifed by command
 *IN:   the map, the player, and a command character
 *OUT:  1 on successful move, 0 on successful move with a win
 *      condition, and -1 for a failed move.
 *POST: cursor has been moved as as the player to a new spot
 *ERROR:map or player are NULL.
 */
int moveTo(Map* map, Player* p, char command);

/**
 *readMap
 *this function takes in a file name and generates a map struct
 *  based off of its specifications.
 *IN:   the file path string.
 *OUT:  the malloced Map pointer, or NULL on error.
 *POST: a Map struct pointer has been malloc'd (so free it)
 *ERROR:fname is invalid, or malloc failed to initialize map
 */
Map* readMap(char *fname);



/**
 *play
 *this is the main play function.  It loops and performs the game actions.
 *IN:   a map pointer, and the player pointer.
 *OUT:  the amount of gold that the player collectd.
 *POST: the game will have been played, and information will have been
 *      written to the ncurses terminal
 *ERROR:N/A
 */
int play(Map* map, Player* player);

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
 *setup
 *This function sets up the required structs
 *IN:   N/A
 *OUT:  0 on success.
 *POST: terminal has been set to ncurses mode.
 *ERROR:initscr failed.
 */
int setup(Map* map, Player* player);


#endif
