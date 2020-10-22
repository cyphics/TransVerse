/* ========================================================================
   $File: $
   $Date: $B
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

#include "game.h"

GameState BuildGameConfig(const upgrade up_list[], int number_upgrades, const resource res_list[], int number_resources)
{
    GameState config;
    memcpy(config.upgrades_list, up_list, sizeof(upgrade) * number_upgrades);
    memcpy(config.resources_list, res_list, sizeof(resource) * number_resources);
    return config;
}
