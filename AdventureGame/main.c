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
};

typedef struct Player Player;

struct Player {
    Room* position;
    int health;
    char* name;
};

Room* cube = NULL;
Room* goal = NULL;
Room* gem  = NULL;
Room* builder = NULL;
int dungeonX = 4;
int dungeonY = 4;
int gameState = 0;

int randomNumber(int max);
char* getInput();
char* getName();

void placeEncounters (Player* player);
Room* placeObject(int sizeX, int sizeY);

void promptPlayer(Player* player);
Room* movePlayer(Player* player);
void checkCollisions (Player* player);
void takeDamage (Player* player);

void buildAllRooms(int sizeX, int sizeY);
int numberOfRoomsWestOfBuilder();
int numberOfRoomsNorthOfBuilder();
void buildRoomEastOfBuilder();
void startNewRowSouthOfPreviousRooms();
Room* newRoom ();

int main(int argc, const char * argv[]) {
    
    /* define RNG seed based on time */
    srand((unsigned)time(NULL));
    
    //BuildDungeon based on X and Y dimensions
    buildAllRooms(dungeonX, dungeonY);
    
    //Create Player, cube and goal in different locations
    Player *player = (Player*)malloc(sizeof(Player));
    player->position = placeObject(dungeonX, dungeonY);
    player->health  = 100;
    player->name = getName();
    
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
    } while (gameState == 1);
    
    
    
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

void takeDamage (Player* player){
    printf("**** You encounter the deadly gelatinous cube!****\n");
    player->health -= 50;
    printf("Health reduced to %d\n\n", player->health);
    
    //check death
    if (player->health < 1) {
        lose();
    } else {
        //if player isn't dead, move the cube
        do {
            cube = placeObject(dungeonX, dungeonY);
        } while (cube == goal || cube == player->position);
    }
}

void checkCollisions (Player* player){
    if (player->position == cube){
        takeDamage(player);
    }
    if (player->position == goal){
    //    win();
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
    void buildAllRooms(int sizeX, int sizeY){
//        int count = 0;                      //DEBUG
        
        if (builder == NULL){
            builder = newRoom();        //count++; printf("%d  ", count); //DEBUG
        }
        
        while(numberOfRoomsWestOfBuilder() < sizeX-1){
            buildRoomEastOfBuilder();
//            count++; printf("%d  ", count); //DEBUG
        }
        while (numberOfRoomsNorthOfBuilder() < sizeY-1){
            startNewRowSouthOfPreviousRooms();
//            count++; printf("\n%d  ", count); //DEBUG

            while(numberOfRoomsWestOfBuilder() < sizeX-1){
                buildRoomEastOfBuilder();
//                count++; printf("%d  ", count); //DEBUG
            }
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
    }

    void startNewRowSouthOfPreviousRooms(){
        // move builder to far west room
        while (builder->west != NULL){
            builder = builder->west;
        }
        builder->south = newRoom();
        
        // create two-way link between rooms
        builder->south->north = builder;
        builder = builder->south;
        
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

    Room* newRoom (){
        Room* room = (Room*)malloc(sizeof(Room));
        room-> north = NULL;
        room-> south = NULL;
        room-> east = NULL;
        room-> west = NULL;

        return room;
    }


