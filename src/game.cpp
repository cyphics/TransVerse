/* ========================================================================
   $File: $
   $Date: $B
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

GameState
BuildGameConfig(upgrade up_list[], int number_upgrades, resource res_list[], int number_resources)
{
    GameState config;
    memcpy(config.upgrades_list, up_list, sizeof(upgrade) * number_upgrades);
    memcpy(config.resources_list, res_list, sizeof(resource) * number_resources);
    return config;
}
