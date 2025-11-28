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
    wormPos[0] = (Vector2) { 0, 3 };
    wormPos[1] = (Vector2) { 0, 2 };
    wormPos[2] = (Vector2) { 0, 1 };
    wormPos[3] = (Vector2) { 0, 0 };
    int currentWormLength = 4; // this must be anchored to how many times i did the one i did in line above this

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

            // clear previous moves
            //map[(int)wormPos[currentWormLength].x][(int)wormPos[currentWormLength].y] = 0;

            // remove previous worm segment if we ran out of worm
            map[(int) wormPos[1].x][(int) wormPos[1].y] = 0;
            map[(int) wormPos[2].x][(int) wormPos[2].y] = 0;
            map[(int) wormPos[3].x][(int) wormPos[3].y] = 0;
            // for (int i = 0; i < currentWormLength; i++) {
            //     map[(int)wormPos[i].x][(int)wormPos[i].y] = 1;
            // }

            // inherit head pos to body 1
            wormPos[3] = wormPos[2];
            wormPos[2] = wormPos[1];
            wormPos[1] = wormPos[0];
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
            for (int i = 0; i < currentWormLength; i++) {
                if (i == 0)
                map[(int)wormPos[i].x][(int)wormPos[i].y] = 2;
                else
                map[(int)wormPos[i].x][(int)wormPos[i].y] = 1;
            }
            
            inputDelay = 15;
        }

        BeginDrawing();

        ClearBackground(BLACK);
        
        // render anything above 0 to be green (worm) for now, objects and blocks might be stored as negatives
        for (int i = 0; i < pixelHeight; i++)
        for (int j = 0; j < pixelWidth; j++)
        if (map[i][j] > 0) {
            if (map[i][j] == 2) 
            DrawRectangle(j * squareRes, i * squareRes, size.x, size.y, DARKGREEN);
            else
            DrawRectangle(j * squareRes, i * squareRes, size.x, size.y, GREEN);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}