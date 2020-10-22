/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber $
   ======================================================================== */
#if !defined(GAME_DEBUG_H)
#define GAME_DEBUG_H

#include "game_handler.h"

void CreateFirstXmlFile();
void PrintUpgrade(upgrade u);
void PrintResource(resource res);
void PrintGameState(GameState state);
void PrintUpgradeInfo(struct UpgradeInfo info);

#endif
