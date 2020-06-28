/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber $
   ======================================================================== */

#if !defined(GAME_HANDLER_H)
#define GAME_HANDLER_H

#include "physics.h"
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
} UpgradeInfo;

void UpdateAffordableUpgrades();
void UpdateAvailableUpgrades();
bool PurchaseUpgrade(UPGRADE handle, int amount = 1);
UpgradeInfo GetInfo(UPGRADE handle);
bool LoadGame();
void SaveGame();

#endif
