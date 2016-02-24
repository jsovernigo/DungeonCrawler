/**
 *  Giuliano Sovernigo (2016)
 *  gsoverni@mail.uoguelph.ca
 *
 *  This file contains the main function for assignment 2 in
 *  UoGuelph's CIS*2500_W16 course.  The program itself is a
 *  game, similar to bsdGames' Rogue.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "roomgen.h"
#include "entities.h"
#include "driver.h"

/**
 *main
 *entry point for the program, calls all the rest 
 *of the functions defined in the header files
 *IN:   argc, the argument count, and argv, the argument list
 *OUT:  literal 0 on success, 1 on error.
 *POST: terminal will have gone through curses mode, and game
 *      will have been played.
 *ERROR:argv[1] is an invalid filepath.
 */
int main(int argc, char *argv[])
{
    int result;
    char message[50];

    Map* map;
    Player* player;

    if(argc < 2) // check for argument requirements
    {
        printf("Incorrect usage.  Please use as:\n%s <levelFile>\n", argv[0]);
        return 1;
    }

    srand(time(NULL)); // seed the rand

    // init player and map
    map = readMap(argv[1]);
    player = initPlayer("Hero");

    if(map == NULL && player == NULL) // both setups have gone wrong
    {
        printf("Uh oh! Something must have gone wrong.\nPlease make sure you entered a valid file path for the level file!\n");
        perror("Map and player setup failed with condition");
        return 1;
    }
    else if(player == NULL) // player malloc has probably failed
    {
        perror("Something really weird happened.\nPlayer setup failed with condition");
        delMap(map);
        return 1;
    }
    else if(map == NULL) // something went wrong with the definition in the file/malloc.
    {
        printf("Uh oh! Something must have gone wrong.\nPlease make sure you entered a valid file path for the level file!\n");
        perror("Map setup failed with condition");
        delPlayer(player);
        return 1;
    }

    result = setup(map, player); // setup the game structures and initscr

    if(result == -1) // great, that failed too.
    {
        cleanup(map, player);
        printf("Uh oh! Something must have gone wrong.\nThe setup for the game failed.\nTry resizing your window.\n");
        perror("System setup failed with condition");
        return 1;
    }

    play(map, player); // game loop

    strcpy(message, "Congratulations, you found ");
    sprintf(message + strlen(message), "%d gold in the dungeon.", player->gold);
    printMessage(message); //print the win message
    
    getch(); // wait for the input to end the game

    cleanup(map, player);
    
    return 0;
}
