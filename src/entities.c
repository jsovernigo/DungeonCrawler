/**
 *Author: Giuliano Sovernigo 0948924
 *
 *DUE: 2/17/2016
 *
 *Project A2, CIS*2500_W16
 *
 *  This file contains the function code for the fucntion
 *definitions in entities.h.  These functions manipulate the
 *structures also defined in that header file.  They are all
 *used in the rogue-like project as game structures.
 *
 *
 */
#include "entities.h"

int addGold(Player* player, char type)
{
    int gold;

    gold = 0;

    if(player == NULL) // the player is invalid
    {
        return -1;
    }

    if(type == '8') // found big gold
    {
        gold = (rand() % 200) + 50;
    }
    else if(type == '*') // found small gold
    {
        gold = (rand() % 50);
    }
    else // some other character was passed in.
    {
        gold = 0;
    }

    player->gold += gold; // add the gold to the player.

    return gold;
}

void delPlayer(Player* player)
{
    free(player->name);
    free(player);
}

Player* initPlayer(char* name)
{
    Player* player;

    player = malloc(sizeof(Player)); // create the player.
    if(player == NULL) // malloc failed.
    {
        free(player);
        return NULL;
    }   

    player->name = malloc(sizeof(char)*32);
    if(player->name == NULL)
    {
        free(player);
        return NULL;
    }

    strncpy(player->name, name, 32); // set name, health and gold
    player->health  = 25;
    player->gold    = 0;

    player->standingOn = '.';
    
    player->x = 0;
    player->y = 0;
    player->room = 1;
    
    return player;
}



