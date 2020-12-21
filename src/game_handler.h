/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber $
   ======================================================================== */

#if !defined(GAME_HANDLER_H)
#define GAME_HANDLER_H

#include <stdbool.h>
#include "game.h"

typedef void *UPGRADE;
typedef struct UpgradeInfo
{
    char name[30];
    char desc[100];
    char type[40];
    float increase_factor;
    price initial_price;
    u_time time_until_available;
    int amount_bought;
    char **dependencies[MAX_DEPENDENCIES][30];
    void *handle;
} UpgradeInfo;

typedef struct UpgradesHandleList
{
    void* list[MAX_UPGRADES_AMOUNT];
    int size;
} UpgradesHandleList;

extern UpgradesHandleList available_upgrades;

void UpdateAffordableUpgrades();
void UpdateAvailableUpgrades();
bool PurchaseUpgrade(UPGRADE handle, int amount);
void GetInfo(UPGRADE handle, UpgradeInfo *info);
bool LoadGame();
void SaveGame();
void Wait(double elapsed_time);
void StartGame();

#endif
