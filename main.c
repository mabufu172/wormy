#include "include/raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const char gameName[] = "wormy";
const int squareReso = 60;
const int screenWidth = 1080;
const int screenHeight = 1080;
const int pixelReso = screenWidth / squareReso;
int map[18][18]; //1080 / square reso
int inputDelay = 0;
int currentAction = 0; // 0 = stationary, 1 = left, 2 = right, 3 = up, 4 = down
int currentWormLength = 0;
int headCollisionState[4]; // true == will collide, false == free to go
Vector2 wormPos[30]; // max worm length is 30
Vector2 tailPos;

bool checkCollision(Vector2 positionInMap, int direction) {
    switch (direction) {
        case 1:
        return map[(int) positionInMap.x][(int) positionInMap.y - 1] != 0;
        case 2:
        return map[(int) positionInMap.x][(int) positionInMap.y + 1] != 0;
        case 3:
        return map[(int) positionInMap.x - 1][(int) positionInMap.y] != 0;
        case 4:
        return map[(int) positionInMap.x + 1][(int) positionInMap.y] != 0;
    }
    return false;
}

void updateHeadCollisionState() {
    headCollisionState[0] = checkCollision(wormPos[0], 1);
    headCollisionState[1] = checkCollision(wormPos[0], 2);
    headCollisionState[2] = checkCollision(wormPos[0], 3);
    headCollisionState[3] = checkCollision(wormPos[0], 4);
}

void initMap() {
    // fill everything with zeros (default; blanks)
    for (int i = 0; i < pixelReso; i++)
    for (int j = 0; j < pixelReso; j++)
    map[i][j] = 0;
}

void drawSnakeToMap() {
    for (int i = 0; i < currentWormLength; i++)
    if ((i % 2) == 0) map[(int)wormPos[i].x][(int)wormPos[i].y] = 1;
    else map[(int)wormPos[i].x][(int)wormPos[i].y] = 2;
}

int main() {
    
    initMap();
    InitWindow(screenWidth, screenHeight, gameName);
    SetTargetFPS(60);
    HideCursor();
    //ToggleFullscreen();

    // make this non hard code
    wormPos[currentWormLength++] = (Vector2) { 0, 3 };
    wormPos[currentWormLength++] = (Vector2) { 0, 2 };
    wormPos[currentWormLength++] = (Vector2) { 0, 1 };
    wormPos[currentWormLength++] = (Vector2) { 0, 0 };

    // always draw intial worm state in the very beginning
    drawSnakeToMap();
    updateHeadCollisionState();

    // test showing where apple is
    // map[5][5] = 1;

    while (!WindowShouldClose()) {
        if (inputDelay > 0) inputDelay--;

        currentAction = 0;

        if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D)) && inputDelay == 0) {

            if (IsKeyDown(KEY_A) && !headCollisionState[0]) currentAction = 1;
            if (IsKeyDown(KEY_D) && !headCollisionState[1]) currentAction = 2;
            if (IsKeyDown(KEY_W) && !headCollisionState[2]) currentAction = 3;
            if (IsKeyDown(KEY_S) && !headCollisionState[3]) currentAction = 4;

            // make sure it will actually move
            if (currentAction != 0) {
            
                // tailPos gets updated to the last worm segment
                tailPos = wormPos[currentWormLength - 1];
                for (int i = 0; i < currentWormLength; i++) {
                    // remove previous worm segment
                    map[(int) wormPos[i + 1].x][(int) wormPos[i + 1].y] = 0;
                    
                    // inherit n worm segment to n - 1 worm segment
                    wormPos[currentWormLength - i] = wormPos[(currentWormLength - i) - 1];
                }

                // update worm head position, in this state, wormPos[0] (head) actually collides with wormPos[1]
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

                drawSnakeToMap();

                // update collision state
                updateHeadCollisionState();

                // printf("X->%d ", (int) wormPos[0].x);
                // printf("Y->%d\n", (int) wormPos[0].y);
            
                inputDelay = 15;
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);
        
        // render anything above 0 to be green (worm) for now, objects and blocks might be stored as negatives
        for (int i = 0; i < pixelReso; i++)
        for (int j = 0; j < pixelReso; j++)
        if (map[i][j] > 0) {
            if (map[i][j] == 1) DrawRectangle(j * squareReso, i * squareReso, squareReso, squareReso, DARKGREEN);
            else if (map[i][j] == 2) DrawRectangle(j * squareReso, i * squareReso, squareReso, squareReso, GREEN);
        }
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}