#include "gui.cpp"
#include "game.cpp"
#include "game_handler.cpp"
#include "game_debug.cpp"
#include "helper.h"
#include "physics.cpp"
#include "storage.cpp"
#include <stb/stb.h>

int main(int argc, char *argv[])
{

    // CreateFirstXmlFile();
    if (LoadGame()) {
        SartGui();
    }
}
