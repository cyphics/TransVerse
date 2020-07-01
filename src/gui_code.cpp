/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

extern const int screenWidth;
extern const int screenHeight;
void TypeCode()
{
    // Initialization
    //--------------------------------------------------------------------------------------

    const char message[128] = "This sample illustrates a text writing\nanimation effect! Check it out! ;)";
    int lines = 0;
    // Color backgroundColor = {50, 48, 47, 255};
    Color backgroundColor = DARKBROWN;
    // Color textColor = {179, 182, 38, 255};
    Color textColor = BEIGE;
    Color textColorBack = {136, 137, 41, 255};
    char symbol;
    Font font = GetFontDefault();
    Rectangle codeContainer = { 25, 25, (float)screenWidth - 50, (float)screenHeight - 50};
    int codeFontSize = 20;
    int codeFontPadding = 2;
    char buffer[200000];
    FILE *fp = fopen("../assets/3dfirmware.txt", "r");
    if (!fp) {
        printf("Error, file not loaded\n");
    }
    if (fp) {
        while ((symbol = getc(fp)) != EOF) {
            strcat(buffer, &symbol);
        }
        fclose(fp);
    }

    int codePosition = 0;
    int textStep = 1;

    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        int key = GetKeyPressed();
        if ((key >= 32) && (key <= 125)) {
            codePosition+= textStep;
        }
        if (IsKeyPressed(KEY_ENTER)) codePosition = 0;
        if (IsKeyPressed(KEY_SPACE)) textStep++;

        const char *textToDraw = TextSubtext(buffer, 0, codePosition);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(backgroundColor);

        // DrawText(textToDraw, 21, 16, codeFontSize, textColorBack);

        DrawTextRec(font, textToDraw, codeContainer, codeFontSize, codeFontPadding, true, textColor);


        DrawText("PRESS [ENTER] to RESTART!", 240, 500, 20, LIGHTGRAY);
        DrawText("PRESS [SPACE] to SPEED UP!", 239, 540, 20, LIGHTGRAY);


        EndDrawing();
        //----------------------------------------------------------------------------------
    }
}
