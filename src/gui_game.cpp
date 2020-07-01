/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */
#include <raylib.h>
#include <time.h>

#include "gui.h"
#include "helper.cpp"
#include "gui_code.cpp"

float interfaceAnchorX = 800.0f;
float interfaceAnchorY = 0;

float isInit;

Rectangle mainPanelRect = {interfaceAnchorX + padding, interfaceAnchorY + padding,
                           800 - padding * 2, 600 - padding * 2};

Interact terminalButton {"termTab", "", (Rectangle){mainPanelRect.x + padding, mainPanelRect.y + padding, 100, 30},
                         "Terminal"};
Interact editorButton {"editor", "", (Rectangle){terminalButton.rect.x + terminalButton.rect.width + padding, mainPanelRect.y + padding, 100, 30},
                       "Editor"};

void HandleMouseHoveredGame()
{
    int gesture = GetGestureDetected();
    Vector2 mouse = GetMousePosition();

    terminalButton.isHovered = (CheckCollisionPointRec(mouse, terminalButton.rect));

}
void HandleMouseClickGame()
{


}



void DrawEditor()
{

}

void DrawTerminal()
{

}


void DrawStaticInterface()
{
    DrawRectangleLinesEx(mainPanelRect, 2, BLACK);
    DrawInteract(&terminalButton);
    DrawInteract(&editorButton);
}


void DrawGame()
{

    HandleMouseHoveredGame();
    HandleMouseClickGame();
    DrawStaticInterface();
    // DrawAnimation();
    // TypeCode();
    // DrawConfigMenu();
}
