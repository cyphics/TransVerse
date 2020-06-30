#include "gui.cpp"
#include "game.cpp"
#include "game_handler.cpp"
#include "game_debug.cpp"
#include "helper.cpp"
#include "physics.cpp"
#include "storage.cpp"

extern char *CONFIG_FILE;

typedef struct Button{
    void *function;

}Button;

void Print()
{
    printf("Success!%s\n");
}
int main(int argc, char *argv[])
{

    // CreateFirstXmlFile();
    if (LoadGame()) {
        SartGui();
    }
    // Button b = {&Print};

}
