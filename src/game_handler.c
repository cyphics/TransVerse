/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

#include "includes.c"

extern GameState state;
extern char *CONFIG_FILE;
// UPGRADE available_upgrades[MAX_UPGRADES_AMOUNT] = {};
// UPGRADE affordable_upgrades[MAX_UPGRADES_AMOUNT] = {};


HandleList AvailableUpgrades;

upgrade *GetUpgradeFromName(char *name)
{
    for (int i = 0; i < MAX_UPGRADES_AMOUNT; i++) {
        if (AreStrEquals(state.upgrades_list[i].id, name)) {
            return &state.upgrades_list[i];
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


void UpdateAffordableUpgrades()
{
    // NOTE(cyphics) : This method is called every frame
    // It must therefore be economic

}

void UpdateAvailableUpgrades()
{
    // Return list of all upgrades with satisfied dependencies
    // NOTE(cyphics) : This method is ran after every purchase

    AvailableUpgrades.size = 0;
    for (int i = 0; i < MAX_UPGRADES_AMOUNT; i++) {
        upgrade *up = &state.upgrades_list[i];
        if (!IsEmpty(up->id) && IsBuyable(up) && AreDependenciesMet(up)){
            AvailableUpgrades.list[AvailableUpgrades.size++] = up;
        }
    }
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
    bool result = LoadGameFromFile(CONFIG_FILE, &state);
    if (result) {
        UpdateAvailableUpgrades();
        UpdateAffordableUpgrades();
        // PrintGameState(state);
    }

    return result;
}

void SaveGame()
{
    SaveGameToDisk(CONFIG_FILE, &state);
    printf("Game saved!\n");
}

void Wait(u_time elapsed_time)
{
    state.elapsed_time += elapsed_time;
    // compute and set new speed
    state.traveled_distance += TraveledDistance(state.current_speed,
                                                elapsed_time,
                                                state.current_acceleration);
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
