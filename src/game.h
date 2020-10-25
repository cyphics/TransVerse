//
// Created by cyphics on 7/19/20.
//

#ifndef TRANSVERSE_GAME_H
#define TRANSVERSE_GAME_H

#include "types.h"

#define MAX_UPGRADES_AMOUNT 30
#define MAX_RESOURCES_AMOUNT 5
#define MAX_RESOURCES_PER_PRICE 4
#define MAX_DEPENDENCIES 4
#define UPGRADE_ID_MAX_LENGTH 25

typedef void* UPGRADE;

typedef struct resource
{
    char type[15];
    int amount;
} resource;

typedef struct price { resource resources[MAX_RESOURCES_PER_PRICE]; } price;

typedef struct upgrade
{
    char id[UPGRADE_ID_MAX_LENGTH];
    char type[15];
    char description[100];
    price initial_price;
    float  increase_factor;
    int amount_bought;
    char dependencies[MAX_DEPENDENCIES][30];
    char filename[50]; // software file
    int amount_typed; // software
} upgrade;

typedef struct GameState{
    u_speed current_speed;
    u_distance traveled_distance;
    u_time elapsed_time;
    u_acceleration current_acceleration;
    upgrade upgrades_list[MAX_UPGRADES_AMOUNT];
    resource resources_list[MAX_RESOURCES_AMOUNT];
} GameState;

extern GameState game_state;
GameState BuildGameConfig(const upgrade up_list[], int number_upgrades, const resource res_list[], int number_resources);

u_speed GetCurrentSpeed();
u_distance GetTraveledDistance();
u_acceleration GetCurrentAcceleration();
#endif //TRANSVERSE_GAME_H
