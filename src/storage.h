/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber $
   ======================================================================== */

#if !defined(STORAGE_H)

#define STORAGE_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "game.h"

char *CONFIG_FILE = "../../saves/config.xml";

char *GetXmlSegment(char receiver[], char xml_code[], char *entry);
void SaveUpgrades(char *output, upgrade *up_list);
void SaveResources(char *output, resource *res_list);
void LoadUpgrades(char *xml_code, upgrade *upgrades_list);
void LoadResources(char *xml_code, resource *resources_list);
bool LoadGameFromFile(char *file_path, GameState *state);
void SaveGameToDisk(char *file_name, GameState *state);
void PriceToStr(char *ptr, price p); // TODO remove
#endif
