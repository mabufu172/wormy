#include "include/raylib.h"
#include <stdio.h>

int inputDelay = 0;
int currentAction = 0;
// 0 = stationary, 1 = left, 2 = right, 3 = up, 4 = down

int main(void) {
    const char GAME_NAME[] = "wormy";
    const float squareRes = 60.0f;
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    const int pixelWidth = screenWidth / squareRes;
    const int pixelHeight = screenHeight / squareRes;
    const int maxWormLength = 30;
    
    int map[pixelHeight][pixelWidth];
    
    // fill everything with zeros (default; blanks)
    for (int i = 0; i < pixelHeight; i++)
    for (int j = 0; j < pixelWidth; j++)
    map[i][j] = 0;
    
    InitWindow(screenWidth, screenHeight, GAME_NAME);
    SetTargetFPS(60);
    ToggleFullscreen();
    HideCursor();

    Vector2 wormPos[(int) maxWormLength];
    Vector2 size = {squareRes, squareRes};

    
    int currentWormLength = 0;
    // make this non hard code
    wormPos[currentWormLength++] = (Vector2) { 0, 3 };
    wormPos[currentWormLength++] = (Vector2) { 0, 2 };
    wormPos[currentWormLength++] = (Vector2) { 0, 1 };
    wormPos[currentWormLength++] = (Vector2) { 0, 0 };

    // always draw intial worm state in the very beginning
    for (int i = 0; i < currentWormLength; i++)
    map[(int)wormPos[i].x][(int)wormPos[i].y] = 1;

    while (!WindowShouldClose()) {
        if (inputDelay > 0) inputDelay--;

        currentAction = 0;

        if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D)) && inputDelay == 0) {

            if (IsKeyDown(KEY_W)) currentAction = 3;
            if (IsKeyDown(KEY_A)) currentAction = 1;
            if (IsKeyDown(KEY_S)) currentAction = 4;
            if (IsKeyDown(KEY_D)) currentAction = 2;

            for (int i = 0; i < currentWormLength; i++) {
                // remove previous worm segment
                map[(int) wormPos[i + 1].x][(int) wormPos[i + 1].y] = 0;
                
                // inherit n worm segment to n - 1 worm segment
                wormPos[currentWormLength - i] = wormPos[(currentWormLength - i) - 1];
            }

            // prevent diagonal movements
            switch (currentAction) {
            case 1:
                wormPos[0].y -= 1;
                break;
            case 2:
                wormPos[0].y += 1;
                break;
            case 3:
                wormPos[0].x -= 1;
                break;
            case 4:
                wormPos[0].x += 1;
                break;
            }

            // loop thru all worm segments and make it 1 in map (green in render)
            for (int i = 0; i < currentWormLength; i++)
            map[(int)wormPos[i].x][(int)wormPos[i].y] = 1;
            
            inputDelay = 15;
        }

        BeginDrawing();

        ClearBackground(BLACK);
        
        // render anything above 0 to be green (worm) for now, objects and blocks might be stored as negatives
        for (int i = 0; i < pixelHeight; i++)
        for (int j = 0; j < pixelWidth; j++)
        if (map[i][j] > 0) DrawRectangle(j * squareRes, i * squareRes, size.x, size.y, GREEN);
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}