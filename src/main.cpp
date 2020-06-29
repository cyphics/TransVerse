#include "gui.cpp"
#include "game.cpp"
#include "game_handler.cpp"
#include "game_debug.cpp"
#include "helper.cpp"
#include "physics.cpp"
#include "storage.cpp"

extern char *CONFIG_FILE;

int main(int argc, char *argv[])
{

    // CreateFirstXmlFile();
    if (LoadGame()) {
        SartGui();
    }
}
