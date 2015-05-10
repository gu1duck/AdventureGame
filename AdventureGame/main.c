//
//  main.c
//  AdventureGame
//
//  Created by Jeremy Petter on 2015-05-07.
//  Copyright (c) 2015 Jeremy Petter. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct Room Room;

struct Room {
    Room* north;
    Room* south;
    Room* east;
    Room* west;
    Room* up;
    Room* down;
    char* nChalk;
    char* sChalk;
    char* eChalk;
    char* wChalk;
    char* uChalk;
    char* dChalk;
};

typedef struct Player Player;

struct Player {
    Room* position;
    int health;
    char* name;
    int gem;
};

Room* cube = NULL;
Room* goal = NULL;
Room* gem  = NULL;
Room* builder = NULL;
int dungeonX = 4;
int dungeonY = 4;
int dungeonZ = 4;
int gameState = 0;
int roomNo;

int randomNumber(int max);
char* getInput();
char* getName();

void placeEncounters (Player* player);
Room* placeObject(int sizeX, int sizeY);

void promptPlayer(Player* player);
Room* movePlayer(Player* player);
void checkCollisions (Player* player);
void takeDamage (Player* player);
void findGem(Player* player);
void win();
void lose();
void encounterCube(Player* player);

void buildAllRooms(int sizeX, int sizeY, int sizeZ);
int numberOfRoomsWestOfBuilder();
int numberOfRoomsNorthOfBuilder();
void buildRoomEastOfBuilder();
int numberOfRoomsBelowBuilder();
void startNewRowSouthOfPreviousRooms();
void startNewFloorAboveNorthWestCorner();
void buildRow(int sizeX);
void buildFloor(int sizeX, int sizeY);
Room* newRoom ();

int main(int argc, const char * argv[]) {
    
    /* define RNG seed based on time */
    srand((unsigned)time(NULL));
    
    //BuildDungeon based on X and Y dimensions
    buildAllRooms(dungeonX, dungeonY, dungeonZ);
    
    //Create Player, cube and goal in different locations
    Player *player = (Player*)malloc(sizeof(Player));
    player->position = placeObject(dungeonX, dungeonY);
    player->health  = 100;
    player->name = getName();
    player->gem = 0;
    
    placeEncounters(player);
    
    gameState = 1;
    do {
        //Prompt
        promptPlayer(player);
        player->position = movePlayer(player);
        //Check Collisions
        checkCollisions(player);
        //Win
        //Take Damage
        //Die
    } while (gameState);
    
    
    
    return 0;
}

void placeEncounters(Player* player){
    do {
        cube = placeObject(dungeonX, dungeonY);
    } while (cube == gem || cube == goal || cube == player->position);
    
    do {
        goal = placeObject(dungeonX, dungeonY);
    } while (goal == gem || goal == cube || goal == player->position);
    
    do {
        gem = placeObject(dungeonX, dungeonY);
    } while (gem == goal ||gem == cube || gem == player->position);
}

char* getName(){
    printf("What is your name, adventurer?\n>");
    char* input = getInput();
    return input;
}

void win(){
    printf("****You found the treasure! Congratulations, you win!****\n");
    gameState = 0;
}

void lose(){
    printf("****You died to the deadly gelatinous cube!****\n");
    gameState = 0;
}

void findGem(Player* player){
    printf("****You found the a shiny gem! You stick it in your pocket****\n");
    player->gem = 1;
    gem = NULL;
}

void encounterCube(Player* player){
    printf("**** You encounter the deadly gelatinous cube!****\n");
    if(player->gem == 1){
        printf("****The glowing gem flashes and destroys the gelatinous cube!****\n");
        cube = NULL;
    } else {
        takeDamage(player);
        //if player isn't dead, move the cube
        if (gameState) {
            do{
                cube = placeObject(dungeonX, dungeonY);
            } while (cube == goal || cube == player->position);
        }
    }
}

Room* markWall(Player* player){
    printf("Mark which wall?\n%s>", player->name);
    char* wall = getInput();
    printf("What word would you like to write?\n%s>", player->name);
    char* message = getInput();
    switch (wall[0]) {
        case 'n':
        case 'N':
            player->position->nChalk = message;
            printf("You marked the NORTH wall: \"%s\"\n\n", message);
            promptPlayer(player);
            return movePlayer(player);
            break;
        case 's':
        case 'S':
            player->position->sChalk = message;
            printf("You marked the SOUTH wall: \"%s\"\n\n", message);
            promptPlayer(player);
            return movePlayer(player);
            break;
        case 'e':
        case 'E':
            player->position->eChalk = message;
            printf("You marked the EAST wall: \"%s\"\n\n", message);
            promptPlayer(player);
            return movePlayer(player);
            break;
        case 'w':
        case 'W':
            player->position->wChalk = message;
            printf("You marked the NORTH wall: \"%s\"\n\n", message);
            promptPlayer(player);
            return movePlayer(player);
            break;
        case 'u':
        case 'U':
            player->position->uChalk = message;
            printf("You marked the CEILING: \"%s\"\n\n", message);
            promptPlayer(player);
            return movePlayer(player);
            break;
        case 'd':
        case 'D':
            player->position->dChalk = message;
            printf("You marked the FLOOR: \"%s\"\n\n", message);
            promptPlayer(player);
            return movePlayer(player);
            break;

        default:
            printf("What do you mean? ");
            return markWall(player);
            break;
    }
}

void takeDamage (Player* player){
    player->health -= 50;
    printf("Health reduced to %d\n\n", player->health);
    
    //check death
    if (player->health < 1) {
        lose();
    }
}

void checkCollisions (Player* player){
    if (player->position == cube){
        encounterCube(player);
    }
    if (player->position == goal){
        win();
    }
    if (player->position == gem){
        findGem(player);
    }
}

char* getInput(){
    char* input = malloc(sizeof(char) *255);
    scanf("%s", input);
    return input;
}

Room* movePlayer(Player* player){
    char* input = getInput();
    switch (input[0]) {
        case 'n':
        case 'N':
            if (player->position->north == NULL){
                printf("You can't move that way\n>");
                return movePlayer(player);
            } else {
                player->position = player->position->north;
                printf("MOVED NORTH\n\n");
            }
            break;
        case 's':
        case 'S':
            if (player->position->south == NULL){
                printf("You can't move that way\n>");
                return movePlayer(player);
            } else {
                player->position = player->position->south;
                printf("MOVED SOUTH\n\n");
            }
            break;
        case 'e':
        case 'E':
            if (player->position->east == NULL){
                printf("You can't move that way\n>");
                return movePlayer(player);
            } else {
                player->position = player->position->east;
                printf("MOVED EAST\n\n");
            }
            break;
        case 'w':
        case 'W':
            if (player->position->west == NULL){
                printf("You can't move that way\n>");
                return movePlayer(player);
            } else {
                player->position = player->position->west;
                printf("MOVED WEST\n\n");
            }
            break;
        case 'u':
        case 'U':
            if (player->position->up == NULL){
                printf("You can't move that way\n>");
                return movePlayer(player);
            } else {
                player->position = player->position->up;
                printf("MOVED UP\n\n");
            }
            break;
        case 'd':
        case 'D':
            if (player->position->down == NULL){
                printf("You can't move that way\n>");
                return movePlayer(player);
            } else {
                player->position = player->position->down;
                printf("MOVED WEST\n\n");
            }
            break;
        case 'm':
        case 'M':
            return markWall(player);
            break;
        default:
            printf("What do you mean? Which direction?\n%s>", player->name);
            return movePlayer(player);
            break;
    }
    return player->position;
}

void promptPlayer(Player* player){
    printf("You are in a dark room. There are available exits in the following directions:\n");
    if (player->position->north != NULL)
        printf("NORTH ");
    if (player->position->south != NULL)
        printf("SOUTH ");
    if (player->position->east != NULL)
        printf("EAST ");
    if (player->position->west != NULL)
        printf("WEST ");
    if (player->position->up != NULL)
        printf("UP ");
    if (player->position->down != NULL)
        printf("DOWN ");
    printf("\n");
    if (player->position->nChalk != NULL)
        printf("\"%s\" is written on the NORTH wall.\n", player->position->nChalk);
    if (player->position->sChalk != NULL)
        printf("\"%s\" is written on the SOUTH wall.\n", player->position->sChalk);
    if (player->position->eChalk != NULL)
        printf("\"%s\" is written on the EAST wall.\n", player->position->eChalk);
    if (player->position->wChalk != NULL)
        printf("\"%s\" is written on the WEST wall.\n", player->position->wChalk);
    if (player->position->uChalk != NULL)
        printf("\"%s\" is written on the CEILING.\n", player->position->uChalk);
    if (player->position->dChalk != NULL)
        printf("\"%s\" is written on the FLOOR.\n", player->position->dChalk);

    printf("\nEnter the direction in which you would like to move.\n%s>", player->name);
}

int randomNumber(int max){
    return (rand() % max);
}

Room* placeObject(int sizeX, int sizeY){
    int x = randomNumber(sizeX);
    int y = randomNumber(sizeY);
    Room* pointer = builder;
    
    for (int i = 0 ; i < x; i++){
        pointer = pointer->east;
    }
    for (int i = 0 ; i < y; i++){
        pointer = pointer->south;
    }
    return pointer;
}

//Dungeon Construction Methods
void buildAllRooms(int sizeX, int sizeY, int sizeZ){
    if (builder == NULL){
        builder = newRoom();
    }
    
    buildRow(sizeX);
    buildFloor(sizeX, sizeY);
    while(numberOfRoomsBelowBuilder() < sizeZ-1){
        startNewFloorAboveNorthWestCorner();
        buildRow(sizeX);
        buildFloor(sizeX, sizeY);
    }
    
}

void buildRow(int sizeX){
    while(numberOfRoomsWestOfBuilder() < sizeX-1){
        buildRoomEastOfBuilder();
    }
    // move builder to far west room
    while (builder->west != NULL){
        builder = builder->west;
    }
}

void buildFloor(int sizeX, int sizeY){
    while (numberOfRoomsNorthOfBuilder() < sizeY-1){
        startNewRowSouthOfPreviousRooms();
        buildRow(sizeX);
    }
    //return builder to northwest corner
    while (builder->west != NULL) builder = builder->west;
    while (builder->north != NULL) builder = builder->north;
}

void buildRoomEastOfBuilder(){
    builder->east = newRoom();
    
    // create two-way link between rooms
    builder->east->west = builder;
    builder = builder->east;
    
    //check if builder is on at least the second row
    if (builder->west->north != NULL){
        // create two-way link with room to the north
        builder->north = builder->west->north->east;
        builder->west->north->east->south = builder;
    }
    //check if builder is on at least the second floor
    if (builder->west->down != NULL){
        // create two-way link with room below
        builder->down = builder->west->down->east;
        builder->west->down->east->up = builder;
    }
}

void startNewRowSouthOfPreviousRooms(){
    builder->south = newRoom();
    
    // create two-way link between rooms
    builder->south->north = builder;
    builder = builder->south;
    
}

void startNewFloorAboveNorthWestCorner(){
    builder->up = newRoom();
    
    // create two-way link between rooms
    builder->up->down = builder;
    builder = builder->up;
}

int numberOfRoomsWestOfBuilder(){
    Room* query = builder;
    
    int count = 0;
    while (query->west != NULL){
        query = query->west;
        count ++;
    }
    return count;
}

int numberOfRoomsNorthOfBuilder(){
    Room* query = builder;
    
    int count = 0;
    while (query->north != NULL){
        query = query->north;
        count ++;
    }
    return count;
}

int numberOfRoomsBelowBuilder(){
    Room* query = builder;
    
    int count = 0;
    while (query->down != NULL){
        query = query->down;
        count ++;
    }
    return count;
}


Room* newRoom (){
    Room* room = (Room*)malloc(sizeof(Room));
    room-> north = NULL;
    room-> south = NULL;
    room-> east = NULL;
    room-> west = NULL;
    room-> up = NULL;
    room-> down = NULL;
    room->nChalk = NULL;
    room->sChalk = NULL;
    room->eChalk = NULL;
    room->wChalk = NULL;
    room->uChalk = NULL;
    room->dChalk = NULL;
    
    roomNo++;
    printf("%d ", roomNo);

    return room;
}


