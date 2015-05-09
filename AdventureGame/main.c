//
//  main.c
//  AdventureGame
//
//  Created by Jeremy Petter on 2015-05-07.
//  Copyright (c) 2015 Jeremy Petter. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
typedef struct Room Room;

struct Room {
    Room* north;
    Room* south;
    Room* east;
    Room* west;
};

Room* builder;

void buildAllRooms();

int numberOfRoomsWestOfBuilder();
int numberOfRoomsNorthOfBuilder();
void buildRoomEastOfBuilder();
void startNewRowSouthOfPreviousRooms();
Room* newRoom ();


int main(int argc, const char * argv[]) {
    
    buildAllRooms();
    
    return 0;
}

void buildAllRooms(){
    int count = 0;
    
    if (builder == NULL){
        builder = newRoom();        count++; printf("%d  ", count); //DEBUG
    }
    
    while(numberOfRoomsWestOfBuilder() < 3){
        buildRoomEastOfBuilder();
        count++; printf("%d  ", count); //DEBUG
    }
    while (numberOfRoomsNorthOfBuilder() <3){
        startNewRowSouthOfPreviousRooms();
        count++; printf("\n%d  ", count); //DEBUG

        while(numberOfRoomsWestOfBuilder() < 3){
            buildRoomEastOfBuilder();
            count++; printf("%d  ", count); //DEBUG
        }
    }
    
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


