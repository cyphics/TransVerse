#include "gui.cpp"
#include "game.cpp"
#include "game_handler.cpp"
#include "game_debug.cpp"
#include "helper.h"
#include "physics.cpp"
#include "storage.cpp"

extern char *CONFIG_FILE;
extern HandleList AvailableUpgrades;

int main(int argc, char *argv[])
{

    // (*b.function)("test");

    // CreateFirstXmlFile();
    if (LoadGame()) {
        SartGui();
    }
}
