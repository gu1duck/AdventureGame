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
};

Room* cube = NULL;
Room* goal = NULL;
Room* builder = NULL;
int dungeonX = 4;
int dungeonY = 4;

void promptPlayer(Room* pointer);

int randomNumber(int max);
void placeObject(int sizeX, int sizeY, Room* pointer);

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
    placeObject(dungeonX, dungeonY, player->position);
    while (cube == NULL || cube == goal || cube == player->position){
        placeObject(dungeonX, dungeonY, cube);
    }
    while (goal == NULL || goal == cube || goal == player->position){
        placeObject(dungeonX, dungeonY, goal);
    }
    
    while (1) {
        //Prompt
        promptPlayer(player->position);
        //Get Input
        //MovePlayer
        //Check Collisions
        //Win
        //Take Damage
        //Die
    }
    
    
    
    return 0;
}

//void promptPlayer(Room* pointer){
//    printf("You are in a dark room.\n There are avai");
//    
//    
//}

int randomNumber(int max){
    return (1+rand() % max);
}

void placeObject(int sizeX, int sizeY, Room* pointer){
    int x = randomNumber(sizeX);
    int y = randomNumber(sizeY);
    pointer = builder;
    
    for (int i = 0 ; i < x; i++){
        pointer = pointer->east;
    }
    for (int i = 0 ; i < y; i++){
        pointer = pointer->south;
    }
}

//Dungeon Construction Methods
    void buildAllRooms(int sizeX, int sizeY){
        int count = 0;
        
        if (builder == NULL){
            builder = newRoom();        count++; printf("%d  ", count); //DEBUG
        }
        
        while(numberOfRoomsWestOfBuilder() < sizeX-1){
            buildRoomEastOfBuilder();
            count++; printf("%d  ", count); //DEBUG
        }
        while (numberOfRoomsNorthOfBuilder() < sizeY-1){
            startNewRowSouthOfPreviousRooms();
            count++; printf("\n%d  ", count); //DEBUG

            while(numberOfRoomsWestOfBuilder() < sizeX-1){
                buildRoomEastOfBuilder();
                count++; printf("%d  ", count); //DEBUG
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


