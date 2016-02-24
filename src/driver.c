/**
 *Author: Giuliano Sovernigo 0948924
 *
 *DUE: 2/14/2016
 *
 *Project A2, CIS*2500_W16
 *
 *  This file contains the implementation of the functions
 *defined in the driver.h file.  These functions are used to
 *setup ncurses, initialize the in-game elements, and simply
 *run the game itself.
 *
 */


#include "driver.h"


int changeRoom(Map* map, Player* player)
{
    int newRoom;

    int oldX;
    int oldY;

    int nRoomOriginX;
    int nRoomOriginY;

    int i;
    int j;

    char message[50];
    memset(message, '\0', 50);


    if(map == NULL || player == NULL)
    {       
        return -1;
    }   


    nRoomOriginX = 1;
    nRoomOriginY = 2;

    getyx(stdscr, oldY, oldX);

    if(player->room == 6) // we have to loop back to the beginning
    {
        newRoom = 1;
        player->room = newRoom;
    }
    else
    {
        newRoom = player->room + 1;
        player->room = newRoom;
    }
    if(newRoom > 3) // we are on the second row of rooms.
    {
        nRoomOriginY += 21;
        nRoomOriginX += 26 * (newRoom - 4);
    }
    else // we are on the first row of rooms, so the y origin of the rooms will be the same
    {
        nRoomOriginX += 26 * (newRoom - 1);
    }

    
    strcpy(message, "New Room: ");
    sprintf(message + strlen(message), "%d", newRoom);

    printMessage(message);


    for(i = 0; i < map->rooms[player->room - 1]->width + 2; i++) // loop the cols as defined
    {
        for(j = 0; j < map->rooms[player->room - 1]->length + 2; j++) // loop along the rows
        {
            char tile;
            tile = mvinch(nRoomOriginY + j, nRoomOriginX + i);
            if(tile == '+')
            {
                mvaddch(oldY, oldX, player->standingOn);
                mvaddch(nRoomOriginY + j, nRoomOriginX + i, '@');
                move(nRoomOriginY + j, nRoomOriginX + i);
                player->standingOn = '+';
                return 1;
            }
        }
    }
    return 0;
}


int cleanup(Map* map, Player* player)
{
    if(map)
    {
        delMap(map);
    }
    if(player)
    {
        delPlayer(player);
    }
    endwin();

    return 0;
}


int drawMap(Map* map, Player* player)
{
    int artOriginX;
    int artOriginY;

    int room;

    int i;
    int j;

    if(!map || !player)
    {
        return -1;
    }

    artOriginX = 1;
    artOriginY = 2;

    move(artOriginY, artOriginX);

    for(room = 0; room < map->roomc; room++) // loop through the rooms
    {
        for(i = 0; i < map->rooms[room]->length; i++) // then the rows
        {
            for(j = 0; j < map->rooms[room]->width; j++) // then the columns...
            {
                if(map->rooms[room]->contents[i][j] == '@')
                {
                    mvaddch(artOriginY + i, artOriginX + j, '@'); // place the player
                    map->rooms[room]->contents[i][j] = '.'; // de-room him/her
                    player->x = artOriginX;
                    player->y = artOriginY;
                    player->room = room + 1; // this captures 1-6 instead of 0-5
                }
                else
                {
                    mvaddch(artOriginY + i, artOriginX + j, map->rooms[room]->contents[i][j]); // draw a piece of the map.
                }
            }
        }
        if(room < 2 || room > 2) // this just helps with the spacing basically.
        {
            artOriginX += 26;
        }
        else 
        {
            artOriginY += 21;
            artOriginX = 1;
        }
    }

    return 0;
}


void findPlayer()
{
    int i;
    int j;

    int x;
    int y;

    getmaxyx(stdscr, y, x);

    for(i = 0; i < y; i++) // loop through the y's
    {
        for(j = 0; j < x; j++) // loop through the x's
        {
            if(mvinch(i, j) == '@') // if we found the player...
            {
                return; // this return keeps the cursor at the player's position.
            }
        }
    }
    return;
}


int moveTo(Map* map, Player* player, char command)
{
    int oldX;
    int oldY;

    int newX;
    int newY;

    char tile;

    newX = 0;
    newY = 0;

    if(player == NULL || map == NULL) // we got a problem here.
    {
        return -1;
    }

    getyx(stdscr, oldY, oldX);

    if(command == 'w') // move negative y
    {
        newX = oldX;
        newY = oldY - 1;
    }
    else if(command == 'a') // move negative x
    {
        newX = oldX - 1;
        newY = oldY;
    }
    else if(command == 's') // move positive y
    {
        newX = oldX;
        newY = oldY + 1;
    }
    else if(command == 'd') // move positive x
    {
        newX = oldX + 1;
        newY = oldY;
    }

    tile = mvinch(newY, newX);
    /*
       This is unholy abuse of the string function,
       but checks if the tile character is in the
       allowable subset of room points.
       But it looks like crappy Regex.
       */
    if(strchr(".*8()]!", tile) != NULL) 
    {        
        char message[50];
        memset(message, '\0', 50);

        mvaddch(oldY, oldX, player->standingOn); // replace the old tile
        mvaddch(newY, newX, '@'); // move the player
        move(newY, newX);
        player->standingOn = '.';

        if(tile == '*' || tile == '8') // we have stumbled upon a gold stash
        {
            int gold;
            gold = addGold(player, tile);

            strcpy(message, "Found a gold stash with ");
            sprintf(message + strlen(message), "%d gold in it.", gold);
            printMessage(message);
        }
        else
        {
            printMessage("");
        }
        return 1;
    }
    else if(tile == '>' || tile == '<') // the stairs
    {
        mvaddch(oldY, oldX, player->standingOn);
        mvaddch(newY, newX, '@');
        move(newY, newX);
        player->standingOn = tile;
        return 0;
    }
    else if(tile == '+') // a door.  this one is gonna suck.
    {
        mvaddch(oldY, oldX, player->standingOn);
        mvaddch(newY, newX, '@');
        move(newY, newX);
        player->standingOn = '+';
        changeRoom(map, player);
        return 1;
    }
    else // we are hitting something else... (a wall or monster, basically.
    {
        move(oldY, oldX);
        printMessage("You bump or stumble into something in the darkness.  You really hope it is a wall.");
        return -1;
    }

    return -1; // assume the move failed if all else did for whatever reason.
}


Map* readMap(char* fname)
{
    int i;
    char line[256];

    Map* map;

    FILE* roomsFile;

    Room* rooms[6];

    roomsFile = fopen(fname, "r");
    if(roomsFile == NULL) // make sure it exists, first off.
    {
        return NULL;
    }

    for (i = 0; i < 6; i++) // loop through the rooms.
    {
        char *tokens[15]; 
        int pos;
        int length;
        int width;
        int tStart;
        int j;
        int l;
        int lineLen;
        int linePos;

        j = 0;

        if(fgets(line, 256, roomsFile) == NULL) // get a line
        {
            fclose(roomsFile);
            return NULL;
        }

        lineLen = strlen(line);

        while (j < lineLen && line[j] != '\n') // while we have not read to the end
        {
            if(line[j] == ' ') // if we found the first space.
            {
                tStart = j+1; // this saves the position of where we want to start reading for arguments.
                break;
            }
            j++; // next char.
        }   


        sscanf(line, "%2dX%2d ", &width, &length); // snag the dimensions of the room

        pos = 0;
        linePos = tStart;

        for(j = 0; j < 15; j++) // loop through the parameters
        {
            int k;
            int stop;
            tokens[j] = malloc(sizeof(char) * 6);
            /*
               Even though I manually add null-terminators
               below, I like to keep this memset here for 
               good luck, and just generally 
               */
            memset(tokens[j], '\0', 6);

            stop = 0;
            for(k = 0; k < 6; k++)
            {
                if(line[linePos] == '\n')
                {
                    stop = 1;
                    break;
                }
                else if( line[linePos] == ' ') // separating character.
                {
                    tokens[j][k] = '\0';
                    linePos++;
                    break;
                }
                else // valid character
                {
                    tokens[j][k] = line[linePos];
                    linePos++;
                }
            }
            if(stop == 1) // we have reached the end of the line (no new params)
            {
                //free(tokens[j]);
                break;
            }
            pos++;
        }
        rooms[i] = initRoom(length, width, pos+1, tokens); // setup the rooms

        for(l = 0; l < pos + 1; l++)
        {
            free(tokens[l]);
        }
    }

    map = initMap(rooms, 6); // create the room itself (returned below)

    fclose(roomsFile);

    return map;
}


int play(Map* map, Player* player)
{
    char input;

    input = ' ';

    do // loop while we don't quit.
    {
        int result;

        result = 0;

        input = getch();

        if(input != 'q')
        {
            result = moveTo(map, player, input); // move, and see if it worked.
        }
        if(result == 0) // if the moveTo returned the win condition basically
        {
            break;
        }
        refresh(); // draw the screen.

    }while(input != 'q');

    return player->gold;
}


int printMessage(char* msg)
{
    int result;
    int oldX;
    int oldY;

    getyx(stdscr, oldY, oldX); // record current position

    move(0,0);
    clrtoeol();

    result = printw(msg); // print at a new location

    move(oldY, oldX); // restore cursor

    return result;
}


int setup(Map* map, Player* player)
{
    int x;
    int y;

    x = 0;
    y = 0;

    initscr();

    getmaxyx(stdscr, y, x);

    if(x < MIN_COLS) // we have a too thin window
    {
        return -1;
    }
    if(y < MIN_ROWS) // or a too short one...
    {
        return -1;
    }

    drawMap(map, player); // draw the map to the screen and then record the player's position.

    noecho();
    curs_set(0); // this disables the cursor blinking.
    cbreak();

    findPlayer(); // move the cursor to the player's position.

    return 0;
}
