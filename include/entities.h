/**
 *Author: Giuliano Sovernigo 0948924
 *
 *DUE: 2/17/2016
 *
 *Project A2, CIS*2500_W16
 *
 *  This header file contains the definition of the functions
 *and structs that are used in a rogue-like terminal game.
 *This includes a struct for the player, and a struct for the
 *enemies that will be present in the dungeon.
 *
 *
 */

#ifndef __GSOVERNIGO_ENTITIES_H__
#define __GSOVERNIGO_ENTITIES_H__

#include <stdlib.h>
#include <string.h>

struct player
{
    char* name;
    char standingOn;
    int health;
    int gold;
    int x;
    int y;
    int room;
};

typedef struct player Player;

/**
 *addGold
 *adds an amount of gold to the player struct.
 *IN:   player, the pointer to the player to be modified, and type, either '8' or '*'
 *OUT:  the amount of gold that was found (and added), 0 if type is invalid, or -1 on error
 *POST: player->gold has been increased by a certain amount.
 *ERROR:player is NULL.
 */
int addGold(Player* player, char type);


/**
 *delPlayer
 *Deconstructor function for player structs.
 *IN:   Player* player, a pointer to a player struct
 *OUT:  N/A
 *POST: argument player has been deallocated and is now invalid.
 *ERROR:N/A
 */
void delPlayer(Player* player);

/**
 *initPlayer
 *This is the constructor function for a Player struct. It returns the new player.
 *IN:   name, a string name for the player.
 *OUT:  player, a new pointer to a Player struct (initialized on the heap)
        or NULL, if allocation failed.
 *POST: memory has been ALLOCATED ON THE HEAP. (Remember to free the returned player)
 *ERROR:System is out of memory, so cannot be allocated.
 */
Player* initPlayer(char* name);


#endif
