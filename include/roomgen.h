/**
 *Author: Giuliano Sovernigo 0948924
 *
 *DUE: 2/14/2016
 *
 *Project A2, CIS*2500_W16
 *
 *  This header file contains functions and structs that are used
 *in a rogue-like terminal game.  The struct defined represents a
 *room in the dungeon, and the function defined here will create
 *and initialize all of the rooms and then return them in an array.
 *
 *
 */
#ifndef __GSOVERNIGO_ROOMGEN_H__
#define __GSOVERNIGO_ROOMGEN_H__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct room
{
    int length;
    int width;
    int roomNum;
    char **contents;
};

typedef struct room Room;

struct map
{
    int roomc;
    Room* rooms[6];
};

typedef struct map Map;


/**
 *addDoor
 *This function adds a door to room, specified by arg.
 *IN:   room, the room pointer, argl, the arg length,
 *      and arg, the string specification of the door.
 *OUT:  0 on success, 1 on failure.
 *POST: room->contents has been modified.
 *ERROR:coordinates specified by arg are out of bounds.
 */
int addDoor(Room* room, int argl, char *arg);

/**
 *buildRoom
 *this function creates the room's contentes, and fills
 *  them with floor tiles.
 *IN:   room, the room to be filled.
 *OUT:  0 on success, -1 if room is invalid.
 *POST: room->contents have been filled.
 *ERROR:room is invalid.
 */
int buildRoom(Room* room);


/**
 *delMap
 *This function is the deconstructor for the map struct.
 *IN:   Map* map, the map pointer to be deallocated.
 *OUT:  N/A
 *POST: The passed in pointer has been invalidated, as
 *      has ALL OF THE ITEMS INSIDE OF IT.
 *ERROR:N/A
 */
void delMap(Map* map);


/**
 *delRoom
 *This function is the deconstructor for the Room struct.
 *IN:   room, the room pointer to be deallocated.
 *OUT:  N/A
 *POST: room has been dealocated, and so can be ignored.
 *ERROR:N/A
 */
void delRoom(Room* room);


/**
 *initMap
 *This function is the constructor for the map struct.
 *IN:   rooms, an array of room pointers.
 *OUT:  an initialized (on the heap) Map pointer.
 *POST: memory has been allocated on the heap.
 *ERROR:allocation with malloc fails.
 */
Map* initMap(Room* rooms[], int roomc);


/**
 *initRoom
 *This function is the constructor for a Room struct.
 *IN:   int length, the length of the room, int width, the width of
 *      the room, argc, the argument count, and argv, a string array
 *      of populant arguments.
 *OUT:  a single new room struct pointer, or NULL
 *      upon allocation failure.
 *POST: a new Room pointer has been allocated on the heap.
 *ERROR:System is out of memory, and so allocation fails.
 */
Room* initRoom(int length, int width, int paramc, char *params[]);



#endif
