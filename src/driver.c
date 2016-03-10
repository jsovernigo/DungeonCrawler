/**
 *Author: Giuliano Sovernigo 0948924
 *
 *DUE: 3/13/2016
 *
 *Project A3, CIS*2500_W16
 *
 *  This file contains the implementation of the functions
 *defined in the driver.h file.  These functions are used to
 *setup ncurses, initialize the in-game elements, and simply
 *run the game itself.
 *
 */


#include "driver.h"


int cleanup(Map* map, Player* player, Enemy* enemies[], int* numEnemies)
{
    int i;

    endwin();

    if(map)
    {
        delMap(map);
    }
    if(player)
    {
        delPlayer(player);
    }

    for(i = 1; i < *numEnemies; i++) // loop through the enemy array
    {
        delEnemy(enemies[i]);
    }
    free(numEnemies);

    return 0;
}


int combat(Player* player, Enemy* enemy)
{
    char enemyName[15];

    switch(enemy->symbol)
    {
        case('A'):
            strcpy(enemyName, "Aquator");
            break;
        case('B'):
            strcpy(enemyName, "Bat");
            break;
        case('S'):
            strcpy(enemyName, "Snake");
            break;
        case('Z'):
            strcpy(enemyName, "Zombie");
            break;
        case('T'):
            strcpy(enemyName, "Troll");
            break;
        default:
            strcpy(enemyName, "Unknown");
            break;
    }

    if(getSpeed() > enemy->speed)
    {
        int damage;
        char message[50];
        memset(message, '\0', 50);
        damage = dealDamage(player, enemy);
        sprintf(message, "Player deals %d damage to the %s.", damage, enemyName);
        printMessage(message);

    }
    else // enemy gets to attack
    {
        if(enemy->symbol == 'A') // that means that the enemy is an aquator
        {
            player->attack -= enemy->attack;
            if(player->attack < 1) // make sure that the player has at least SOME attack.
            {
                player->attack = 1;
            }
            printMessage("The Aquator sprays the player, decreasing his attack by 1.");
        }
        else // not an aquator
        {
            int damage;
            char message[50];
            memset(message, '\0', 50);
            damage = takeDamage(player, enemy);
            sprintf(message, "The %s deals %d damage to the Player", enemyName, damage);
            printMessage(message);
        }
    }

    if(enemy->health > 0)
    {
        return 0;
    }

    return 1;
}


int drawHalls(Map* map)
{
    int room;
    int i;


    if(map == NULL)
    {
        return -1;
    }

    room = 0;
    i = 0;

    for(i = 0; i < 79; i++) // this creates the centre hallway.
    {
        mvaddch(22, i, '#');
    }

    for(room = 0; room < map->roomc; room++) // loop through the rooms.
    {
        int doorX;
        int doorY;


        doorX = 0;
        doorY = 0;


        doorY = map->rooms[room]->yOrg;
        for (doorX = map->rooms[room]->xOrg; doorX < map->rooms[room]->xOrg + map->rooms[room]->width; doorX++) // North Face + South Face
        {
            if (mvinch(doorY, doorX) == '+')
            {
                spawnHall(doorY, doorX, 'n');
            }
            if (mvinch(doorY + map->rooms[room]->length - 1, doorX) == '+')
            {
                spawnHall(doorY + map->rooms[room]->length - 1, doorX, 's');
            }
        }

        doorX = map->rooms[room]->xOrg;

        for(doorY = map->rooms[room]->yOrg; doorY < map->rooms[room]->yOrg + map->rooms[room]->width; doorY++) // east and west faces.
        {
            if(mvinch(doorY, doorX) == '+')
            {
                spawnHall(doorY, doorX, 'w');
            }
            if(mvinch(doorY, doorX + map->rooms[room]->width - 1) == '+')
            {
                spawnHall(doorY, doorX + map->rooms[room]->width - 1, 'e');
            }
        }
    }
    return 0;
}


int drawMap(Map* map, Player* player, Enemy* enemies[], int* numEnemies)
{
    int artOriginX;
    int artOriginY;

    int room;

    int i;
    int j;

    int enemy;

    if(!map || !player)
    {
        return -1;
    }

    artOriginX = 1;
    artOriginY = 2;

    enemy = 0;

    move(artOriginY, artOriginX);

    for(room = 0; room < map->roomc; room++) // loop through the rooms
    {
        map->rooms[room]->xOrg = artOriginX;
        map->rooms[room]->yOrg = artOriginY;

        for(i = 0; i < map->rooms[room]->length; i++) // then the rows
        {
            for(j = 0; j < map->rooms[room]->width; j++) // then the columns...
            {
                if(map->rooms[room]->contents[i][j] == '@')
                {
                    mvaddch(artOriginY + i, artOriginX + j, '@'); // place the player
                    map->rooms[room]->contents[i][j] = '.'; // de-room him/her
                    player->x = artOriginX + j;
                    player->y = artOriginY + i;
                    player->room = room + 1; // this captures 1-6 instead of 0-5
                }
                else
                {
                    if(strchr("ABSZT", map->rooms[room]->contents[i][j]) != NULL)
                    {
                        enemies[enemy] = initEnemy(map->rooms[room]->contents[i][j], artOriginY + i, artOriginX + j);                    
                        enemy++;
                    }
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


int enemyMove(Enemy* enemy)
{
    char move;
    int oldX;
    int oldY;

    if(enemy == NULL)
    {
        return -1;
    }

    getyx(stdscr, oldY, oldX);

    move = cycle(enemy); // get the enemies move

    if(move == 'R') // if we should move to a random location
    {
        int random;
        char moves[] = "NEWS";
        random = rand() % 4;
        move = moves[random];
    }

    switch(move) // move in the direction specified
    {
        case('N'):
            if(mvinch(enemy->y - 1, enemy->x) == '.')
            {
                mvaddch(enemy->y, enemy->x, '.');
                enemy->y --;
                mvaddch(enemy->y, enemy->x, enemy->symbol);
            }

            break;
        case('S'):
            if(mvinch(enemy->y + 1, enemy->x) == '.')
            {
                mvaddch(enemy->y, enemy->x, '.');
                enemy->y ++;
                mvaddch(enemy->y, enemy->x, enemy->symbol);
            }
            break;
        case('E'):
            if(mvinch(enemy->y, enemy->x + 1) == '.')
            {
                mvaddch(enemy->y, enemy->x, '.');
                enemy->x ++;
                mvaddch(enemy->y, enemy->x, enemy->symbol);
            }
            break;
        case('W'):
            if(mvinch(enemy->y, enemy->x - 1) == '.')
            {
                mvaddch(enemy->y, enemy->x, '.');
                enemy->x --;
                mvaddch(enemy->y, enemy->x, enemy->symbol);
            }
            break;
        default:
            break;
    }

    move(oldY, oldX); // restore cursor position

    return 0;
}


void gameOver(Player* player, int win)
{
    int defence;
    int attack;
    int i;

    if(player == NULL)
    {
        return;
    }

    attack = 0;
    defence = 0;

    for(i = 0; i < player->items; i++) // loop through the players items to sum their values
    {
        if(player->inv[i]->type == ']') // this item is equiptment
        {
            defence += player->inv[i]->value;
        }
        else // this item is a weapon
        {
            attack += player->inv[i]->value;
        }
    }
    
    clear();

    if(win == 0)
    {
        mvprintw(0,0,"Slowly, quietly, you die in the dungeon.");
    }
    else
    {
        mvprintw(0,0,"Gloriously, you leave the dungeon, seeing sunlight for the first time in hours.");
    }

    
    mvprintw(1,0,"You collected %d gold.\nYour equiptment totaled:\nDefence: %d\nAttack: %d\n", player->gold, defence, attack);
    return;
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



int kill(Enemy* enemies[], int* numEnemies, Enemy* enemy)
{
    int i;
    int j;
    char message[100];
    char enemyName[15];
    
    if(enemies == NULL || numEnemies == NULL || enemy == NULL)
    {
        return -1;
    }

    switch(enemy->symbol)
    {
        case('A'):
            strcpy(enemyName, "Aquator");
            break;
        case('B'):
            strcpy(enemyName, "Bat");
            break;
        case('S'):
            strcpy(enemyName, "Snake");
            break;
        case('Z'):
            strcpy(enemyName, "Zombie");
            break;
        case('T'):
            strcpy(enemyName, "Troll");
            break;
        default:
            strcpy(enemyName, "Unknown");
            break;
    }

    for(i = 0; i < *numEnemies; i ++)
    {
        if(enemy == enemies[i]) // compare their addresses.
        {
            *numEnemies = *numEnemies - 1;
            for(j = i; j < *numEnemies; j++) // loop from where we found the match
            {
                enemies[j] = enemies[j + 1]; // shift every element back one.
            } 
            free(enemy); // free our killed enemy.
            break;
        }
    }

    memset(message, '\0', 100);
    sprintf(message, "The %s dies, screaming.  You smile as it grows still", enemyName);
    printMessage(message);

    return 0;

}


int moveTo(Map* map, Player* player, Enemy* enemies[], int* numEnemies, char command)
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
    if(strchr(".*8()]#!", tile) != NULL) 
    {        
        char message[50];
        memset(message, '\0', 50);

        mvaddch(oldY, oldX, player->standingOn); // replace the old tile
        mvaddch(newY, newX, '@'); // move the player
        move(newY, newX);
        if(tile == '#')
        {
            player->standingOn = '#';
            printMessage("");
        }
        else // anything but a hallway (consumable)
        {
            if(tile == '!')
            {
                printMessage("Found a potion!  You clink it into your potion bag.");
                addPotion(player);
            }
            else if(tile == '*' || tile == '8') // we have stumbled upon a gold stash
            {
                int gold;
                gold = addGold(player, tile);

                strcpy(message, "Found a gold stash with ");
                sprintf(message + strlen(message), "%d gold in it.", gold);
                printMessage(message);
            }
            else if(strchr("()]", tile) != NULL)
            {
                int added;
                Item* item;
                item = initItem(tile);
                added = addItem(player, item);
                if(added == 0)
                {
                    printMessage("You attempt to pick up the item. You fumble with your full bag. You shatter the item. Tool.");
                }
                else if(added == 1)
                {
                    if(tile == ')')
                    {
                        printMessage("You pick up a common weapon from the ground.");
                    }
                    else if(tile == '(')
                    {
                        printMessage("You pick up a rare, shiny weapon from the ground.");
                    }
                    else if(tile == ']')
                    {
                        printMessage("You pick up a piece of equiptment from the ground."); 
                    }
                }
            }
            else // else, clear the message.
            {
                printMessage("");
            }
            player->standingOn = '.';
            return 1;
        }
    }
    else if(strchr("ABSZT", tile))
    {
        int i;
        for(i = 0; i < *numEnemies; i++)
        {
            if(enemies[i]->symbol == tile && enemies[i]->x == newX && enemies[i]->y == newY)
            {
                int result;
                result = combat(player, enemies[i]);
                if(result == 1)
                {
                    kill(enemies, numEnemies, enemies[i]);

                    mvaddch(oldY, oldX, player->standingOn); // replace the old tile
                    mvaddch(newY, newX, '@'); // move the player
                    move(newY, newX);

                    player->standingOn = '.';
                    return 1;
                }
                else if(result == -1)
                {
                    return -1;
                }
            }
        }
        move(oldY, oldX);
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
        printMessage("Opened a door.  It creaked loudly.");
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


Map* readMap(char* fname, int* numEnemies)
{
    int i;
    char line[256];

    Map* map;

    FILE* roomsFile;

    Room* rooms[6];

    *numEnemies = 0;

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
                tStart = j + 1; // this saves the position of where we want to start reading for arguments.
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
                if(strchr("mM",line[linePos]) != NULL)
                {
                    *numEnemies = *numEnemies + 1;
                }
                if(line[linePos] == '\n')
                {
                    stop = 1;
                    break;
                }
                else if(line[linePos] == ' ') // separating character.
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
                break;
            }
            pos++;
        }
        rooms[i] = initRoom(length, width, pos + 1, tokens); // setup the rooms

        for(l = 0; l < pos + 1; l++)
        {
            free(tokens[l]);
        }
    }

    map = initMap(rooms, 6); // create the room itself (returned below)

    fclose(roomsFile);

    if(*numEnemies >= 50)
    {
        printf("Too many monsters were found. Sending some to \"sleep with the fishes\"\n"); 
    }

    return map;
}


int play(Map* map, Player* player, Enemy* enemies[], int* numEnemies)
{
    char input;

    int win;

    input = ' ';

    do // loop while we don't quit.
    {
        int result;
        int i;

        result = 0;

        input = getch(); // get MOVE

        if(input != 'q')
        {
            if(strchr("wasd",input))
            {
                result = moveTo(map, player, enemies, numEnemies, input); // move, and see if it worked.
            }
            else if(input == 'p')
            {
                int success;
                success = consumePotion(player);
                if(success == -1) // the player was null
                {
                    return -1;
                }
                else if(success == 0)
                {
                    printMessage("You drink a potion and feel amazingly refreshed.");
                }
                else
                {
                    printMessage("You try to drink a potion.  You have none.  You are sad.");
                }
                result = 1; // we have NOT WON THE GAME.
            }
            else
            {
                result = 1;
            }
            printStats(player);
        }
        if(result == 0) // if the moveTo returned the win condition basically
        {
            break;
        }

        for(i = 0; i < *numEnemies; i++) // loop through all enemies
        {
            enemyMove(enemies[i]); // move specific enemy.
        }
        if(player->health < 1)
        {
            win = 0;

            return win;
        }
        refresh(); // draw the screen.

    }while(input != 'q');

    win = 1;

    return win;
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


int printStats(Player* player)
{
    int result;
    int oldX;
    int oldY;
    int maxX;
    int maxY;

    if(player == NULL)
    {
        return -1;
    }

    getmaxyx(stdscr, maxY, maxX); // result's value is thrown away later.

    maxX = 0;
    result = maxX;

    getyx(stdscr, oldY, oldX); // save old positions

    result = mvprintw(maxY - 1, 0, "Health: %d, Potions: %d, Attack: %d, Inv: %d/5", player->health, player->potions, player->attack, player->items);

    move(oldY, oldX); // restore the cursor.
    return result;
}   


int setup(Map* map, Player* player, Enemy* enemies[], int* numEnemies)
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

    drawMap(map, player, enemies, numEnemies); // draw the map to the screen and then record the player's position.

    drawHalls(map);

    noecho();
    curs_set(0); // this disables the cursor blinking.
    cbreak();

    findPlayer(); // move the cursor to the player's position.

    printStats(player);


    return 0;
}


int spawnHall(int doorY, int doorX, char face)
{
    int maxX;
    int maxY;

    int yDir;
    int xDir;
    int direction;


    if(doorY > 22) // we are below the centre hallway...
    {
        yDir = -1;
    }
    else
    {
        yDir = 1;
    }

    getmaxyx(stdscr, maxY, maxX);
    if(doorX < 0 || doorX > maxX || doorY < 0 || doorY > maxY) // check x,y bounds
    {
        return -1;
    } 
    if(strchr("nsew",face) == NULL) // if face is invalid
    {
        return -1;
    }

    direction = rand() % 2;
    if(direction == 0)
    {
        xDir = -1;
    }
    else
    {
        xDir = 1;
    }

    /*
       Basic summary of the following algorithm:
       1) search the doors on the north face, trail to the centre,
       then drop down to the centre hallway.
       2) search the south wall, and then drop straight down.
       3) search the west wall, go left (to next room, if there is one) then drop down.
       4) search the east wall, go right (to the next room, if there is one) then drop.

       direction chooses whether it goes left, right, or straight down.
       */

    if(face == 'n' && yDir == 1) // we do not have to worry about if the north face is on a south room.  The central hallway gets it.
    {
        int i;

        int tracerX;
        int tracerY;

        tracerX = doorX;
        tracerY = doorY - 1; // right above/below the room.

        for(i = tracerX; i < maxX && i >= 0; i += xDir)
        {
            if(mvinch(tracerY + 1, i) != ' ') // check if there is still a wall down below us.
            {
                mvaddch(tracerY, i, '#');
            }
            else
            {
                mvaddch(tracerY, i, '#');
                break;
            }
        }
        tracerX = i;
        i = tracerY;
        while (i < maxY && mvinch(i + 1, tracerX) != '#') // loop down until you hit another (the central) hallway.
        {
            i ++;
            mvaddch(i, tracerX, '#'); // draw a hall
        }
    }
    else if (face == 's')
    {
        if (yDir == 1) // i.e., if this is a north side room
        {
            int i;

            int tracerX;
            int tracerY;

            tracerX = doorX;
            tracerY = doorY + 1;

            for (i = tracerY; i < maxY; i++) // loop down until you hit a hallway
            {
                if (mvinch(i, tracerX) == ' ')
                {
                    mvaddch(i, tracerX, '#');
                }
                else // we hit something else
                {
                    break;
                }
            }
        }
        else if(yDir == -1) // we are in a south side room
        {
            int i;

            int tracerX;
            int tracerY;

            tracerX = doorX;
            tracerY = doorY + 1; // right above/below the room.

            for(i = tracerX; i < maxX && i >= 0; i += xDir)
            {
                if(mvinch(tracerY - 1, i) != ' ') // check if there is still a wall down below us.
                {
                    mvaddch(tracerY, i, '#');
                }
                else
                {
                    mvaddch(tracerY, i, '#');
                    break;
                }
            }
            tracerX = i;
            i = tracerY;
            while (i < maxY && mvinch(i - 1, tracerX) != '#') // loop down until you hit another (the central) hallway.
            {
                i --;
                mvaddch(i, tracerX, '#'); // draw a hall
            }

        }

    }
    else if(face == 'e')
    {
        int i;
        int tracerX;
        int tracerY;

        tracerX = doorX + 1;
        tracerY = doorY;

        mvaddch(tracerY, tracerX, '#');

        if(xDir == 1 && doorX < 50) 
        {

            for(i = tracerX + xDir; i < maxX && i > 0; i += xDir) // loop towards the room to the (xDir)
            {
                if(mvinch(tracerY, i) == ' ')
                {
                    addch('#');
                } 
                else
                {
                    break;
                }
            }
        }

        for(i = tracerY + yDir; i > 0 && i < maxY; i += yDir) // loop until we hit anything not blank.
        {
            if(mvinch(i, tracerX) == ' ')
            {
                addch('#');
            }
            else
            {
                break;
            }
        }
    }
    else if(face == 'w')
    {
        int i;
        int tracerX;
        int tracerY;

        tracerX = doorX - 1;
        tracerY = doorY;

        mvaddch(tracerY, tracerX, '#');

        if(xDir == -1 && doorX > 1) // if we are going to be moving away from the room (and we arent the first rooms)
        {

            for(i = tracerX + xDir; i < maxX && i > 0; i += xDir) // loop towards the room to the (xDir)
            {
                if(mvinch(tracerY, i) == ' ')
                {
                    addch('#');
                } 
                else
                {
                    break;
                }
            }
        }

        for(i = tracerY + yDir; i > 0 && i < maxY; i += yDir) // loop until we hit anything not blank.
        {
            if(mvinch(i, tracerX) == ' ')
            {
                addch('#');
            }
            else
            {
                break;
            }
        }

    }

    return 0;
}
