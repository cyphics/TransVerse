/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "utils.h"
#include "game_handler.h"
#include "helper.h"
#include "game_debug.h"
#include "storage.h"
#include "physics.h"
#include "gui/gui.h"
#include "gui/game/gui_game.h"

UpgradesHandleList available_upgrades = {};
const char *project_root_dir;
char config_file_path[PATH_MAX];

void LoadConfigFile(){
    project_root_dir = get_project_root_dir();
    printf("Project dir: %s\n", project_root_dir);

    /* char config_file[PATH_MAX] = project_root_dir; */
    strcat(config_file_path, project_root_dir);
    strcat(config_file_path, "/saves/config.xml");
    printf("Config file: %s\n", config_file_path);

}

upgrade *GetUpgradeFromName(char *name)
{
    for (int i = 0; i < MAX_UPGRADES_AMOUNT; i++) {
        if (AreStrEquals(game_state.upgrades_list[i].id, name)) {
            return &game_state.upgrades_list[i];
        }
    }
    return NULL;
}

bool IsBuyable(upgrade *up_ptr)
{
    /*
      An upgrade can be bought if it has not yet been bought,
      or if it's an incremental ugprade, which can be bought
      and infinite amount of times.
    */
    // if (up_ptr->amount_bought == 0 ||
    //     strcmp(up_ptr->type, "incremental") == 0) {
    //     printf("%s is buyable!\n", up_ptr->id);
    // }
    return
        up_ptr->amount_bought == 0 ||
        strcmp(up_ptr->type, "incremental") == 0;
}

bool AreDependenciesMet(upgrade *up)
{
    bool result = true;
    for (int i = 0; i < MAX_DEPENDENCIES; i++) {
        char *dep_str = up->dependencies[i];
        if (!IsEmpty(dep_str)) {
            upgrade *dependency = GetUpgradeFromName(dep_str);
            if (dependency == NULL) {
                printf("Error. Upgrade %s not found!\n", up->id);
                return false;
            }
            result = result && up->amount_bought > 0;
            if (!result) {
                // printf("Deps of %s are not met because of %s\n", up->id, dependency->id);
            }
        }
    }
    // if (result) {
    //     printf("%s met its dependencies!\n", up->id);
    // }
    return result;
}


/* void UpdateAffordableUpgrades() */
/* { */
/*     // NOTE(cyphics) : This method is called every frame */
/*     // It must therefore be economic */
/*     available_upgrades.size = 0; */
/*     for (int i = 0; i < MAX_UPGRADES_AMOUNT; i++) { */
/*         upgrade *up = &game_state.upgrades_list[i]; */
/*         if (!IsEmpty(up->id) && IsBuyable(up) && AreDependenciesMet(up)){ */
/*             available_upgrades.list[available_upgrades.size++] = up; */
/*         } */
/*     } */
/* } */

void UpdateAvailableUpgrades()
{
    // Stores list of all upgrades with satisfied dependencies
    // NOTE(cyphics) : This method is ran after every purchase

    available_upgrades.size = 0;
    for (int i = 0; i < MAX_UPGRADES_AMOUNT; i++) {
        upgrade *up = &game_state.upgrades_list[i];
        if (!IsEmpty(up->id) && IsBuyable(up) && AreDependenciesMet(up)){
            available_upgrades.list[available_upgrades.size++] = up;
        }
    }
}

static int GetAmountBought(UPGRADE handle){
    return 1;
}

static price MultiplyPrice(price p, float increase_factor){
    price new_price = p;
    for(int i = 0; i < MAX_RESOURCES_PER_PRICE; i++){
        new_price.resources[i].amount *= increase_factor;
    }
}

price GetPriceUpgrade(UPGRADE handle, int amount){
    upgrade *up = (upgrade *)handle;
    price current_price = up->initial_price;

    return current_price;
}

bool IsAffordable(UPGRADE handle){
    bool is_affordable = true;

}

bool PurchaseUpgrade(UPGRADE handle, int amount)
{
    for (int i = 0; i < amount; i++) {
        upgrade *u = (upgrade *)handle;
        PrintUpgrade(*u);
    }

    UpdateAvailableUpgrades();
    return true;
}

void GetInfo(UPGRADE handle, UpgradeInfo *info)
{
    if (handle)  {
        upgrade *up = (upgrade*)handle;
        memcpy(info->name, up->id, sizeof(up->id));
        memcpy(info->desc, up->description, sizeof(up->description));
        memcpy(info->type, up->type, sizeof(up->type));
        info->amount_bought = up->amount_bought;
        info->increase_factor = up->increase_factor;
        info->handle = handle;
    }
}

bool LoadGame()
{
    LoadConfigFile();

    bool result = LoadGameFromFile(config_file_path, &game_state);
    if (result) {
        UpdateAvailableUpgrades();
    }
    /* PrintGameState(state); */
    return result;
}

void SaveGame()
{
    SaveGameToDisk(config_file_path, &game_state);
    printf("Game saved!\n");
}

void Wait(u_time elapsed_time)
{
    game_state.elapsed_time += elapsed_time;
    // compute and set new speed
    game_state.traveled_distance += TraveledDistance(game_state.current_speed,
                                                     elapsed_time,
                                                     game_state.current_acceleration);
    game_state.current_speed += game_state.current_acceleration * elapsed_time;
    // gather resources
    // update state ?
}

void StartGame()
{
    // CreateFirstXmlFile();
    if (LoadGame()) {
        StartGui();
    }
}
