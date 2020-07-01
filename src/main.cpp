#include "gui.cpp"
#include "game.cpp"
#include "game_handler.cpp"
#include "game_debug.cpp"
#include "helper.h"
#include "physics.cpp"
#include "storage.cpp"

extern char *CONFIG_FILE;

typedef struct Button{
    void (*function)(char*);
}Button;

void Print(char *message)
{
    printf("Here's my message: %s\n", message);
}
int main(int argc, char *argv[])
{

    Button b{Print};
    // (*b.function)("test");

    // CreateFirstXmlFile();
    if (LoadGame()) {
        SartGui();
    }

}
