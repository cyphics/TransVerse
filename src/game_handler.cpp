/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

#include "storage.h"
#include "game_debug.h"

extern GameState state;
extern char *CONFIG_FILE;
UPGRADE available_upgrades[MAX_UPGRADES_AMOUNT] = {};
UPGRADE affordable_upgrades[MAX_UPGRADES_AMOUNT] = {};

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
        if (!AreStrEquals(dep_str, "")) {
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

    for (int i = 0; i < MAX_UPGRADES_AMOUNT; i++) {
        upgrade *up = &state.upgrades_list[i];
        if (IsEmpty(up->id) || !(IsBuyable(up) && AreDependenciesMet(up)))
            available_upgrades[i] = NULL;
        else available_upgrades[i] = up;
    }


    // for (int i = 0; i < MAX_UPGRADES_AMOUNT; i++) {
    //     UPGRADE cur_handle = available_upgrades[i];
    //     if (cur_handle != NULL) {
    //         UpgradeInfo up_info = GetInfo(cur_handle);
    //         PrintUpgradeInfo(up_info);
    //     }
    // }

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

UpgradeInfo GetInfo(UPGRADE handle)
{
    UpgradeInfo info;
    if (handle != NULL) {
        upgrade *up = (upgrade*)handle;
        memcpy(info.name, up->id, sizeof(up->id));
        memcpy(info.desc, up->description, sizeof(up->description));
        memcpy(info.type, up->type, sizeof(up->type));
        info.amount_bought = up->amount_bought;
        info.increase_factor = up->increase_factor;
    }

    return info;
}

bool LoadGame()
{
    bool result = LoadGameFromFile(CONFIG_FILE, &state);
    UpdateAvailableUpgrades();
    UpdateAffordableUpgrades();
    // PrintGameState(state);
    return result;
}

void SaveGame()
{
    SaveGameToDisk(CONFIG_FILE, &state);
    printf("Game saved!\n");
}
