/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

#include "gui.h"



char some_text[100];
void StartGui()
{

    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Transverse");
    Image grad = GenImageGradientH(gradWidth, gradHeight, hovered, unhovered);
    gradientTexture = LoadTextureFromImage(grad);
    UnloadImage(grad);
    InitConfig();
    while (!WindowShouldClose())
    {
        // Wait((u_time)GetFrameTime()); // TODO make sure cast is ok
        BeginDrawing();
        {
            // Store input data
            // keyPressed = GetKeyPressed();
            // mousePosition = GetMousePosition();
            // mouseGesture = GetGestureDetected();
            // mouseLeftPressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
            // mouseLeftReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
            // mouseRightPressed = IsMouseButtonPressed(MOUSE_RIGHT_BUTTON);
            // mouseRightReleased = IsMouseButtonReleased(MOUSE_RIGHT_BUTTON);

            ClearBackground(RAYWHITE);
            DrawLineEx((Vector2){800, 0}, (Vector2){800, 600}, 1, LIGHTGRAY);
            // DrawConfigPanel();
            TestRayGui(some_text);
            // DrawGame();
        }
        EndDrawing();
    }
    CloseWindow();
}

void DrawHoverAnimLayer(Interact *i)
{
    if (i->isHoverable) {
        float dist = (i->rect.x + i->rect.width) - (i->rect.x - gradWidth);
        float offset = dist / hoverLength * i->hoverIdx;

        Rectangle colorIn = {i->rect.x - i->rect.width - gradWidth + offset, i->rect.y,
                             i->rect.width + gradWidth, i->rect.height};
        Rectangle colorInDrawn = GetCollisionRec(colorIn, i->rect);


        Rectangle colorOut = {i->rect.x + offset, i->rect.y,
                              i->rect.width, i->rect.height};
        Rectangle colorOutDrawn = GetCollisionRec(colorOut, i->rect);

        DrawRectangleRec(colorInDrawn, hovered);
        DrawRectangleRec(colorOutDrawn, unhovered);

        float rectX, rectWidth;
        float rectXBase = i->rect.x + 50;

        Rectangle movingRectangle = {i->rect.x - gradWidth + offset, i->rect.y,
                                     gradWidth, gradHeight};
        Rectangle intersect = GetCollisionRec(i->rect, movingRectangle);

        // DrawRectangleLinesEx(movingRectangle, 1, BLACK);

        Rectangle sourceRect = {rectX, i->rect.y + gradHeight + 20, rectWidth, gradHeight};

        DrawTexturePro(gradientTexture, staticRect, intersect, (Vector2){}, 0, WHITE); // TODO check if the vector2 is correct
        // DrawRectangleLinesEx(movingRectangle, 1, BLACK);
    }
    if (i->isEditable && i->isHovered) {
        framesCounter++;


        if (((framesCounter/20)%2) == 0)
            DrawText("_", i->rect.x + 6 + MeasureText(i->text, fontSize),
                     i->rect.y + 8, fontSize, MAROON);
        if (framesCounter > 200) framesCounter = 0;


    }

}

void DrawInteract(Interact *i)
{
    if (i->isHovered) {
        i->hoverIdx++;
        if (i->hoverIdx > hoverLength) {
            i->hoverIdx = hoverLength;
        }
        // DrawRectangleRec(i->rect, LIGHTGRAY);
    }
    if (!i->isHovered) {
        i->hoverIdx--;
        if (i->hoverIdx < 0) {
            i->hoverIdx = 0;
        }
    }
    DrawHoverAnimLayer(i);
    DrawRectangleLinesEx(i->rect, 1, BLACK);
    DrawText(i->title, i->rect.x + padding, i->rect.y, i->fontSize / 2, GRAY);
    DrawText(i->text, i->rect.x + padding, i->rect.y + i->rect.height / 4, i->fontSize, MAROON);
}

int GuiListViewExDrag(Rectangle bounds, const char **text, int count, int *focus, int *scrollIndex, int active, void (DragGallBack(int)))
{
    GuiControlState state = guiState;
    int itemFocused = (focus == NULL)? -1 : *focus;
    int itemSelected = active;

    // Check if we need a scroll bar
    bool useScrollBar = false;
    if ((GuiGetStyle(LISTVIEW, LIST_ITEMS_HEIGHT) + GuiGetStyle(LISTVIEW, LIST_ITEMS_PADDING))*count > bounds.height) useScrollBar = true;

    // Define base item rectangle [0]
    Rectangle itemBounds = { 0 };
    itemBounds.x = bounds.x + GuiGetStyle(LISTVIEW, LIST_ITEMS_PADDING);
    itemBounds.y = bounds.y + GuiGetStyle(LISTVIEW, LIST_ITEMS_PADDING) + GuiGetStyle(DEFAULT, BORDER_WIDTH);
    itemBounds.width = bounds.width - 2*GuiGetStyle(LISTVIEW, LIST_ITEMS_PADDING) - GuiGetStyle(DEFAULT, BORDER_WIDTH);
    itemBounds.height = GuiGetStyle(LISTVIEW, LIST_ITEMS_HEIGHT);
    if (useScrollBar) itemBounds.width -= GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH);

    // Get items on the list
    int visibleItems = bounds.height/(GuiGetStyle(LISTVIEW, LIST_ITEMS_HEIGHT) + GuiGetStyle(LISTVIEW, LIST_ITEMS_PADDING));
    if (visibleItems > count) visibleItems = count;

    int startIndex = (scrollIndex == NULL)? 0 : *scrollIndex;
    if ((startIndex < 0) || (startIndex > (count - visibleItems))) startIndex = 0;
    int endIndex = startIndex + visibleItems;

    // Update control
    //--------------------------------------------------------------------
    static bool isDragging;
    static bool isAboutToSelectItem;
    static bool isAboutToRemoveDependency;
    static int draggedItemId;

    Vector2 mousePoint = GetMousePosition();

    if ((state != GUI_STATE_DISABLED) && !guiLocked)
    {
        int gesture = GetGestureDetected();

        if (isDragging && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            isDragging = false;
            DragGallBack(draggedItemId);
            draggedItemId = 0;
        }

        // Check mouse inside list view
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            state = GUI_STATE_FOCUSED;

            // Check focused and selected item
            for (int i = 0; i < visibleItems; i++)
            {
                if (CheckCollisionPointRec(mousePoint, itemBounds))
                {
                    itemFocused = startIndex + i;
                    if (gesture == GESTURE_DRAG) {
                        if (!isDragging) {
                            isDragging = true;
                            draggedItemId = itemFocused;
                        }


                    }
                    else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        isAboutToSelectItem = true;
                    }
                    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                    {
                        if (itemSelected == (startIndex + i)) itemSelected = -1;
                        else itemSelected = startIndex + i;
                    }
                    break;
                }

                // Update item rectangle y position for next item
                itemBounds.y += (GuiGetStyle(LISTVIEW, LIST_ITEMS_HEIGHT) + GuiGetStyle(LISTVIEW, LIST_ITEMS_PADDING));
            }

            if (useScrollBar)
            {
                int wheelMove = GetMouseWheelMove();
                startIndex -= wheelMove;

                if (startIndex < 0) startIndex = 0;
                else if (startIndex > (count - visibleItems)) startIndex = count - visibleItems;

                endIndex = startIndex + visibleItems;
                if (endIndex > count) endIndex = count;
            }
        }
        else itemFocused = -1;

        // Reset item rectangle y to [0]
        itemBounds.y = bounds.y + GuiGetStyle(LISTVIEW, LIST_ITEMS_PADDING) + GuiGetStyle(DEFAULT, BORDER_WIDTH);
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(DEFAULT, BORDER_WIDTH), Fade(GetColor(GuiGetStyle(LISTVIEW, BORDER + state*3)), guiAlpha), GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));     // Draw background

    // Draw visible items
    for (int i = 0; ((i < visibleItems) && (text != NULL)); i++)
    {
        if (state == GUI_STATE_DISABLED)
        {
            if ((startIndex + i) == itemSelected) GuiDrawRectangle(itemBounds, GuiGetStyle(LISTVIEW, BORDER_WIDTH), Fade(GetColor(GuiGetStyle(LISTVIEW, BORDER_COLOR_DISABLED)), guiAlpha), Fade(GetColor(GuiGetStyle(LISTVIEW, BASE_COLOR_DISABLED)), guiAlpha));

            GuiDrawText(text[startIndex + i], GetTextBounds(DEFAULT, itemBounds), GuiGetStyle(LISTVIEW, TEXT_ALIGNMENT), Fade(GetColor(GuiGetStyle(LISTVIEW, TEXT_COLOR_DISABLED)), guiAlpha));
        }
        else
        {
            if ((startIndex + i) == itemSelected)
            {
                // Draw item selected
                GuiDrawRectangle(itemBounds, GuiGetStyle(LISTVIEW, BORDER_WIDTH), Fade(GetColor(GuiGetStyle(LISTVIEW, BORDER_COLOR_PRESSED)), guiAlpha), Fade(GetColor(GuiGetStyle(LISTVIEW, BASE_COLOR_PRESSED)), guiAlpha));
                GuiDrawText(text[startIndex + i], GetTextBounds(DEFAULT, itemBounds), GuiGetStyle(LISTVIEW, TEXT_ALIGNMENT), Fade(GetColor(GuiGetStyle(LISTVIEW, TEXT_COLOR_PRESSED)), guiAlpha));
            }
            else if ((startIndex + i) == itemFocused)
            {
                // Draw item focused
                GuiDrawRectangle(itemBounds, GuiGetStyle(LISTVIEW, BORDER_WIDTH), Fade(GetColor(GuiGetStyle(LISTVIEW, BORDER_COLOR_FOCUSED)), guiAlpha), Fade(GetColor(GuiGetStyle(LISTVIEW, BASE_COLOR_FOCUSED)), guiAlpha));
                GuiDrawText(text[startIndex + i], GetTextBounds(DEFAULT, itemBounds), GuiGetStyle(LISTVIEW, TEXT_ALIGNMENT), Fade(GetColor(GuiGetStyle(LISTVIEW, TEXT_COLOR_FOCUSED)), guiAlpha));
            }
            else
            {
                // Draw item normal
                GuiDrawText(text[startIndex + i], GetTextBounds(DEFAULT, itemBounds), GuiGetStyle(LISTVIEW, TEXT_ALIGNMENT), Fade(GetColor(GuiGetStyle(LISTVIEW, TEXT_COLOR_NORMAL)), guiAlpha));
            }
        }

        // Update item rectangle y position for next item
        itemBounds.y += (GuiGetStyle(LISTVIEW, LIST_ITEMS_HEIGHT) + GuiGetStyle(LISTVIEW, LIST_ITEMS_PADDING));
    }

    if (isDragging) {
        // GuiDrawText(text[draggedItemId], GetTextBounds(DEFAULT, itemBounds), GuiGetStyle(LISTVIEW, TEXT_ALIGNMENT), Fade(GetColor(GuiGetStyle(LISTVIEW, TEXT_COLOR_NORMAL)), guiAlpha));
        DrawRectangle(mousePoint.x - padding - bounds.width/2, mousePoint.y - padding - 15, bounds.width, 30, GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        DrawRectangleLines(mousePoint.x - padding  - bounds.width/2, mousePoint.y - padding - 15, bounds.width, 30, DARKGRAY);
        DrawText(text[draggedItemId], mousePoint.x  - bounds.width/2, mousePoint.y - 15, 10, DARKGRAY);
    }

    if (useScrollBar)
    {
        Rectangle scrollBarBounds = {
            bounds.x + bounds.width - GuiGetStyle(LISTVIEW, BORDER_WIDTH) - GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH),
            bounds.y + GuiGetStyle(LISTVIEW, BORDER_WIDTH), (float)GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH),
            bounds.height - 2*GuiGetStyle(DEFAULT, BORDER_WIDTH)
        };

        // Calculate percentage of visible items and apply same percentage to scrollbar
        float percentVisible = (float)(endIndex - startIndex)/count;
        float sliderSize = bounds.height*percentVisible;

        int prevSliderSize = GuiGetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE);   // Save default slider size
        int prevScrollSpeed = GuiGetStyle(SCROLLBAR, SCROLL_SPEED); // Save default scroll speed
        GuiSetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE, sliderSize);            // Change slider size
        GuiSetStyle(SCROLLBAR, SCROLL_SPEED, count - visibleItems); // Change scroll speed

        startIndex = GuiScrollBar(scrollBarBounds, startIndex, 0, count - visibleItems);

        GuiSetStyle(SCROLLBAR, SCROLL_SPEED, prevScrollSpeed); // Reset scroll speed to default
        GuiSetStyle(SCROLLBAR, SCROLL_SLIDER_SIZE, prevSliderSize); // Reset slider size to default
    }
    //--------------------------------------------------------------------

    if (focus != NULL) *focus = itemFocused;
    if (scrollIndex != NULL) *scrollIndex = startIndex;

    return itemSelected;
}
