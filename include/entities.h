/**
 *Author: Giuliano Sovernigo 0948924
 *
 *DUE: 3/13/2016
 *
 *Project A3, CIS*2500_W16
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

#define PLAYER_MAX_HP 50
#define AQUATOR_MOVESET ""
#define BAT_MOVESET "R"
#define SNAKE_MOVESET "EEWW"
#define ZOMBIE_MOVESET "NS"
#define TROLL_MOVESET "NESW"

struct item
{
    char type;
    int value;
};

typedef struct item Item;

struct player
{
    char standingOn;
    int potions;
    int health;
    int attack;
    int gold;
    int x;
    int y;
    int room;
    int items;
    Item* inv[5];
    char* name;
};

typedef struct player Player;


struct enemy
{
    char symbol;
    int x;
    int y;
    int health;
    int speed;
    int attack;
    int cycle;
    char moveCycle[5];
};

typedef struct enemy Enemy;

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
 *addPotion
 *adds a potion to player's potion count, or returns the failed condition if it's full.
 *IN:   player, the pointer to the player struct to be modified.
 *OUT:  -1 on error, 0 on success, or 1 on inventory full condition.
 *POST: player->potions has been modified.
 *ERROR:player is null (-1 returned)
 */
int addPotion(Player* player);


/**
 *addItem
 *this function adds an item to the player's inventory
 *IN:   player and item, two structs, the latter to be added to a field of the former.
 *OUT:  1 on success, 0 on failed adding, -1 on error.
 *POST: Player->items has been modified, as has player->inventory
 *ERROR:player or item are NULL
 */
int addItem(Player* player, Item* item);


/**
 *consumePotion
 *this function decreases player->potions by one, and increases player health to 25.
 *IN:   player, the player struct to be modifed
 *OUT:  -1 on error, 0 on success, 1 on failure.
 *POST: player->health has been modifed, as has player->potions 
 *ERROR:player is null (-1 returned)
 */
int consumePotion(Player* player);


/**
 *cycle
 *this function cycles through an enemy's cycle of moves
 *IN:   an Enemy pointer called enemy.
 *OUT:  one of the following characters: N, S, E, W, R, or \0
 *POST: enemy->cycle has been modified.
 *ERROR:enemy is NULL (\0 is returned)
 */
char cycle(Enemy* enemy);


/**
 *dealDamage
 *this function deals damage to an enemy, using player's attack values.
 *IN:   player, the player whose attack values will be used, and enemy, the enemy to be damaged.
 *OUT:  the amount of damage that was dealt, or -1 if player or enemy are invalid.
 *POST: enemy->health has been altered.
 *ERROR:enemy or player are invalid (-1 returned.)
 */
int dealDamage(Player* player, Enemy* enemy);


/**
 *delEnemy
 *This function deletes the enemy struct, freeing all of its memory.
 *IN:   enemy, the pointer to be deallocated.
 *OUT:  N/A
 *POST: enemy has now been deallocated.  It is invalid.
 *ERROR:N/A
 */
void delEnemy(Enemy* enemy);


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
 *getSpeed
 *This function returns a player's random speed value.
 *IN:   N/A
 *OUT:  a random number between 1 and 10
 *POST: N/A
 *ERROR:N/A
 */
int getSpeed();


/**
 *initEnemy
 *this function creates an enemy pointer that can be used.
 *IN:   a character reperesenting the type, and the coordinates, x and y
 *OUT:  a generated enemy structure or NULL.
 *POST: enemy has been allocate. Free before end of program
 *ERROR:enemy cannot be allocated.
 */
Enemy* initEnemy(char type, int y, int x);


/**
 *initItem
 *This function allocates and initializes an item struct.
 *IN:   a character representing the type of item.
 *OUT:  a new item pointer or null
 *POST: new memory allocated.
 *ERROR:malloc fails
 */
Item* initItem(char type);


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


/**
 *takeDamage
 *this function causes player->health to be decreased by enemy->attack
 *IN:   player and enemy, the two structs that are fighting
 *OUT:  the damage dealth (enemy->attack) or -1 on error
 *POST: player->health has been decreased.
 *ERROR:player or enemy are null (-1 returned)
 */
int takeDamage(Player* player, Enemy* enemy);


#endif
