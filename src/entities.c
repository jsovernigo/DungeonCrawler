/**
 *Author: Giuliano Sovernigo 0948924
 *
 *DUE: 3/13/2016
 *
 *Project A3, CIS*2500_W16
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


int addPotion(Player* player)
{

    if(player == NULL)
    {
        return -1;
    }
    if(player->potions < 5) // room to spare
    {
        player->potions++;
    }
    else // full inventory
    {
        return 1;
    }

    return 0;
}


int addItem(Player* player, Item* item)
{
    if(player == NULL || item == NULL)
    {
        return -1;
    }

    if(player->items >= 5)
    {
        return 0;
    }
    else
    {
        player->inv[player->items] = item;
        player->items = player->items + 1;
    }
    return 1;
}


int consumePotion(Player* player)
{

    if(player == NULL)
    {
        return -1;
    }
    if(player->potions > 0) // if we actually have a potion to drink.
    {
        player->health = PLAYER_MAX_HP;
        player->potions--;
    }
    else // no potions.
    {
        return 1;
    }

    return 0;
}


char cycle(Enemy* enemy)
{
    char move;

    if(enemy == NULL)
    {
        return 0;
    }

    if(enemy->symbol == 'A')
    {
        return '\0';
    }

    if(enemy->cycle > strlen(enemy->moveCycle) - 1) // check to see if our cycle is out of bounds
    {
            enemy->cycle = 0;
    }
    
    move = enemy->moveCycle[enemy->cycle];
    enemy->cycle ++; // increment the cycle.

    if(enemy->cycle > strlen(enemy->moveCycle) - 1) // check again, so that we can head off any errors next time.
    {
        enemy->cycle = 0;
    }

    return move;

}


int dealDamage(Player* player, Enemy* enemy)
{
    int damage;
    
    if(enemy == NULL || player == NULL)
    {
        return -1;
    }

    damage = player->attack; // store the damage

    enemy->health -= damage; // damage the enemy

    return damage;
}


void delEnemy(Enemy* enemy)
{
    if(enemy != NULL)
    {
        free(enemy);
    }
    return;
}


void delPlayer(Player* player)
{
    int i;
    free(player->name);
    for(i = 0; i < player->items; i++)
    {
        free(player->inv[i]);
    }
    free(player);
    return;
}


int getSpeed()
{
    int random;

    random = rand() % 10 + 1; // get the player's speed.

    return random;
}


Enemy* initEnemy(char type, int y, int x)
{
    Enemy* enemy;

    enemy = malloc(sizeof(Enemy)); // allocate the enemy struct pointer.
    if(enemy == NULL)
    {
        return NULL;
    }

    enemy->symbol = type;
    enemy->cycle = 0;
    enemy->y = y;
    enemy->x = x;

    switch(type)
    {
        case('A'): // aquator should be created
            strcpy(enemy->moveCycle, AQUATOR_MOVESET);
            enemy->health = 5;
            enemy->attack = 1;
            enemy->speed  = 2;
            break;
        case('B'): // bat should be created
            strcpy(enemy->moveCycle, BAT_MOVESET);
            enemy->health = 2;
            enemy->attack = 5;
            enemy->speed  = 4;
            break;
        case('S'): // snake should be created
            strcpy(enemy->moveCycle, SNAKE_MOVESET);
            enemy->health = 5;
            enemy->attack = 5;
            enemy->speed  = 4;
            break;
        case('Z'): // zombie should be created
            strcpy(enemy->moveCycle, ZOMBIE_MOVESET);
            enemy->health = 15;
            enemy->attack = 5;
            enemy->speed  = 2;
            break;
        case('T'): // troll should be created
            strcpy(enemy->moveCycle, TROLL_MOVESET);
            enemy->health = 50;
            enemy->attack = 5;
            enemy->speed  = 3;
            break;
        default: // this should never catch, but it will generate an unknown enemy
            strcpy(enemy->moveCycle, "");
            enemy->health = 1;
            enemy->attack = 1;
            enemy->speed  = 1;
            break;
    }

    return enemy;
}



Item* initItem(char type)
{
    int value;

    Item* item;

    value = 0;

    item = malloc(sizeof(Item));
    if(item == NULL)
    {
        return NULL;
    }

    if(type == ']') // equiptment
    {
        value = rand() % 21;
    }
    else if(type == ')') // common weapon
    {
        value = rand() % 9 + 1;
    }
    else if(type == '(') // rare weapon
    {
        value = rand() % 6 + 10;
    }

    item->type = type;
    item->value = value;

    return item;

}


Player* initPlayer(char* name)
{
    Player* player;

    player = malloc(sizeof(Player)); // create the player.
    if(player == NULL) // malloc failed.
    {
        return NULL;
    }   

    player->name = malloc(sizeof(char)*32);
    if(player->name == NULL)
    {
        free(player);
        return NULL;
    }

    strncpy(player->name, name, 32); // set name, health and gold
    player->health  = 50;
    player->gold    = 0;

    player->standingOn = '.';

    player->x = 0;
    player->y = 0;
    player->room = 1;
    player->potions = 1;
    player->attack = 5;
    player->items = 0;

    return player;
}


int takeDamage(Player* player, Enemy* enemy)
{
    if(player == NULL || enemy == NULL)
    {
        return -1;
    }
    player->health -= enemy->attack;
    return enemy->attack;
}


