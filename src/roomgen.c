/**
 *Author: Giuliano Sovernigo 0948924
 *
 *DUE 3/13/2016
 *
 *Project A3, CIS*2500_W16
 *
 *  This c file contains the implementation for the functions
 *defined in the roomgen.h file.  These functions are used to
 *initialize the rooms to be used in the map, and the map itself.
 *
 *
 */

#include "roomgen.h"


int addDoor(Room* room, int argl, char *arg)
{
    int j;

    int sidel;
    int sidew;            
    char side;
    char pos[2];

    sidel = 0;
    sidew = 0;

    if (argl < 3 || arg[0] != 'd') // even though these are already checked, verify.
    {
        return 1;
    }

    side = arg[1]; // get the wall.
    /*
       This loop advances from the second character
       to the end of the string, capturing the door
       position number.
       */
    for (j = 2; j < argl; j++) // loop through the argument, going from the side to the end.
    {
        pos[j - 2] = arg[j];
    }


    if (side == 'n') // north wall
    {
        sidel = 0;
        sidew = atoi(pos);
    }
    else if (side == 's') // south wall
    {
        sidel = room->length - 1;
        sidew = atoi(pos);
    }
    else if (side == 'e') // east wall.
    {
        sidel = atoi(pos);
        sidew = room->width - 1;
    }
    else if (side == 'w') // west wall.
    {
        sidel = atoi(pos);
        sidew = 0;
    }
    if(sidel < room->length && sidew < room->width)
    {
        room->contents[sidel][sidew] = '+'; // set the door at our location.
    }
    return 0;
}


int buildRoom(Room* room)
{
    int i;
    int j;

    if(!room)
    {
        return -1;
    }

    room->contents = malloc(sizeof(char*)*room->length);

    for (i = 0; i < room->length; i++) // loop through the rows of the room
    {
        room->contents[i] = malloc(sizeof(char) * room->width);

        for (j = 0; j < room->width; j++) // loop through the columns of the room
        {
            if (i == 0 || i == room->length - 1) // if we are at the top or bottom of the room
            {
                if (j == 0 || j == room->width - 1) // if we are at a corner, basically.
                {
                    room->contents[i][j] = '-';
                }
                else // if we are on a wall
                {
                    room->contents[i][j] = '-';
                }
            }
            else // if we are in the middle of the room, or the east/west wall
            {
                if (j == 0 || j == room->width - 1) // east/west wall...
                {
                    room->contents[i][j] = '|';
                }
                else // middle of the room.
                {
                    room->contents[i][j] = '.';
                }
            }
        }
    }


    return 0;
}

void delMap(Map* map)
{
    int i;

    for(i = 0; i < 6; i++)
    {
        delRoom(map->rooms[i]);
    }
    free(map);
}   


void delRoom(Room* room)
{
    int i;

    for(i = 0; i < room->length; i ++) // loop through the contents array
    {
        free(room->contents[i]); // free this line
    }
    free(room->contents); // free the whole 2d array

    free(room); // finally, free the struct itself.

    return;
}


Map* initMap(Room* rooms[], int roomc)
{
    int i;
    Map* map;

    map = malloc(sizeof(Map));
    for(i = 0; i < 6; i++)
    {
        map->rooms[i] = rooms[i];
    } 

    map->roomc = roomc;

    return map;
}


Room* initRoom(int length, int width, int paramc, char *params[])
{
    int i;

    Room* room;

    room = malloc(sizeof(Room));

    /*
       Below, length is assigned to length + 2.
       This is to account for the fact that the
       walls in the dungeon also are included in
       the room struct, so we need to increase
       the size of the room slightly.
       */
    room->length = length + 2;
    room->width = width + 2;


    buildRoom(room);

    for(i = 0; i < paramc; i++) // loop through the parameters.
    {
        int length;

        length = strlen(params[i]);

        if(length <= 0)
        {
            continue;
        }
        else if(params[i][0] == 'd' && length >= 3)
        {
            addDoor(room, length, params[i]);
        }
        else if(params[i][0] == 'g' && length >= 4)
        {
            int x, y;
            x = 0;
            y = 0;
            sscanf(params[i],"g%2d,%2d", &y, &x);
            if(y < room->length-2 && x < room->width-2)
            {
                room->contents[y][x] = '*';
            }
        }
        else if(params[i][0] == 'G' && length >= 4)
        {
            int x, y;
            x = 0;
            y = 0;
            sscanf(params[i],"G%2d,%2d", &y, &x);
            room->contents[y][x] = '8';
        }
        else if(params[i][0] == 'w' && length >= 4)
        {
            int x, y;
            x = 0;
            y = 0;
            sscanf(params[i],"w%2d,%2d", &y, &x);
            if(y < room->length-2 && x < room->width-2)
            {
                room->contents[y][x] = ')';
            }
        }
        else if(params[i][0] == 'W' && length >= 4)
        {
            int x, y;
            x = 0;
            y = 0;
            sscanf(params[i],"W%2d,%2d", &y, &x);
            if(y < room->length-2 && x < room->width-2)
            {
                room->contents[y][x] = '(';
            }
        }
        else if(params[i][0] == 'e' && length >= 4)
        {
            int x, y;
            x = 0;
            y = 0;
            sscanf(params[i],"e%2d,%2d", &y, &x);
            if(y < room->length-2 && x < room->width-2)
            {
                room->contents[y][x] = ']';
            }
        } 
        else if(params[i][0] == 'm' && length >= 4)
        {
            int x, y;
            char symb;

            int random;

            random = (rand() % 4) + 1;

            switch(random)
            {
                case 1:
                    symb = 'A';
                    break;
                case 2:
                    symb = 'B';
                    break;
                case 3:
                    symb = 'S';
                    break;
                case 4:
                    symb = 'Z';
                    break;
                default:
                    symb = 'U';
                    break;
            }

            x = 0;
            y = 0;
            sscanf(params[i],"m%2d,%2d", &y, &x);
            if(y < room->length-2 && x < room->width-2)
            {
                room->contents[y][x] = symb;
            }
        }
        else if(params[i][0] == 'M' && length >= 4)
        {
            int x, y;
            x = 0;
            y = 0;
            sscanf(params[i],"M%2d,%2d", &y, &x);
            if(y < room->length-2 && x < room->width-2)
            {
                room->contents[y][x] = 'T';
            }
        }
        else if(params[i][0] == 'p' && length >= 4)
        {
            int x, y;
            x = 0;
            y = 0;
            sscanf(params[i],"p%2d,%2d", &y, &x);
            if(y < room->length-2 && x < room->width-2)
            {
                room->contents[y][x] = '!';
            }
        }
        else if(params[i][0] == 'h' && length >= 4)
        {
            int x, y;
            x = 0;
            y = 0;
            sscanf(params[i],"h%2d,%2d", &y, &x);
            if(y < room->length-2 && x < room->width-2)
            {
                room->contents[y][x] = '@';
            }
        }
        else if(params[i][0] == 'z' && length >= 4)
        {
            int x, y;
            x = 0;
            y = 0;
            sscanf(params[i],"z%2d,%2d", &y, &x);
            if(y < room->length-2 && x < room->width-2)
            {
                room->contents[y][x] = '<';
            }
        }
        else if(params[i][0] == 'a' && length >= 4)
        {
            int x, y;
            x = 0;
            y = 0;
            sscanf(params[i],"a%2d,%2d", &y, &x);
            if(y < room->length-2 && x < room->width-2)
            {
                room->contents[y][x] = '>';
            }
        }
    }

    return room;
}




