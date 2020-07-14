#include "gui.c"
#include "game.c"
#include "game_handler.c"
#include "game_debug.c"
#include "helper.h"
#include "physics.c"
#include "storage.c"

// #define STB_DS_IMPLEMENTATION
// #include "../lib/stb/stb_ds.h"

int main(int argc, char *argv[])
{

    // CreateFirstXmlFile();
    if (LoadGame()) {
        SartGui();
    }
}
