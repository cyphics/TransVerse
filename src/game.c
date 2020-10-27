/* ========================================================================
   $File: $
   $Date: $B
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

#include <string.h>
#include "game.h"
#include "physics.h"

GameState game_state = {};

GameState BuildGameConfig(const upgrade up_list[], int number_upgrades, const resource res_list[], int number_resources)
{
    GameState config;
    memcpy(config.upgrades_list, up_list, sizeof(upgrade) * number_upgrades);
    memcpy(config.resources_list, res_list, sizeof(resource) * number_resources);
    return config;
}

u_speed GetCurrentSpeed() {
    return game_state.current_speed;
}
u_distance GetTraveledDistance(){
    return game_state.traveled_distance;
}
u_acceleration GetCurrentAcceleration(){
    return game_state.current_acceleration;
}

u_time GetRemainingTime() {
    return RemainingTime(dist_mstone.id.universe_diamete.value, GetCurrentSpeed(), GetCurrentAcceleration());
}