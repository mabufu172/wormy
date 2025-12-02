#include "include/raylib.h"
#include <stdio.h>
#include <stdlib.h>

const char gameName[] = "wormy";
const int squareReso = 60;
const int screenWidth = 1080;
const int screenHeight = 1080;
const int pixelReso = screenWidth / squareReso;
const int voidYLevel = 17;
int map[18][18]; //1080 / square reso
int currentAction = 0; // 0 = stationary, 1 = left, 2 = right, 3 = up, 4 = down
int currentWormLength = 0;
int currentLowestSegment; // int of the index of wormPos
bool headCollisionState[4]; // true == will collide, false == free to go
bool shouldFall;
Vector2 wormPos[30]; // max worm length is 30
Vector2 tailPos;

bool checkCollision(Vector2 positionInMap, int direction, bool ignoreWorm) {
    
    // setting num to two causes it to ignore worm as worm IDs are 2 and 3 (head and body)
    int num = 1;
    if (ignoreWorm) num = 3;

    switch (direction) {
        case 1: return map[(int) positionInMap.x][(int) positionInMap.y - 1] > num;
        case 2: return map[(int) positionInMap.x][(int) positionInMap.y + 1] > num;
        case 3: return map[(int) positionInMap.x - 1][(int) positionInMap.y] > num;
        case 4: return map[(int) positionInMap.x + 1][(int) positionInMap.y] > num;
    }

    return false;

}

void updateHeadCollisionState() {
    for (int i = 0; i < 4; i++)
    headCollisionState[i] = checkCollision(wormPos[0], (i + 1), false);
}

void checkShouldFall() {
    bool onGround = false;
    for (int i = 0; i < currentWormLength; i++) 
    if (checkCollision(wormPos[i], 4, true)) onGround = true;
    shouldFall = !onGround;
}

void updateLowestSegment() {
    int lowest = 0;
    int lowestSegment;
    for (int i = 0; i < currentWormLength; i++)
    if (wormPos[i].x > lowest) {
        lowest = wormPos[i].x;
        lowestSegment = i;
    }
    currentLowestSegment = lowestSegment;
}

void initMap() {
    // fill everything with zeros (default; blanks)
    for (int i = 0; i < pixelReso; i++)
    for (int j = 0; j < pixelReso; j++)
    map[i][j] = 0;
}

void drawSnakeToMap() {
    for (int i = 0; i < currentWormLength; i++)
    if (i== 0) map[(int)wormPos[i].x][(int)wormPos[i].y] = 3;
    else map[(int)wormPos[i].x][(int)wormPos[i].y] = 2;
}

Color getColorFromId(int id) {
    switch(id) {
        case 1: return RED;
        case 2: return GREEN;
        case 3: return DARKGREEN;
        case 4: return GRAY;
        case 5: return BROWN;
    }
    return PURPLE; // fallback
}

void drawVisual() {
    BeginDrawing();
    ClearBackground(BLACK);
    for (int i = 0; i < pixelReso; i++)
    for (int j = 0; j < pixelReso; j++)
    if (map[i][j] > 1) DrawRectangle(j * squareReso, i * squareReso, squareReso, squareReso, getColorFromId(map[i][j]));
    else if (map[i][j] == 1) DrawCircle(j * squareReso + squareReso / 2, i * squareReso + squareReso / 2, squareReso / 2, RED);
    //DrawCircle()
    EndDrawing();
}

void gameLoop() {
    // printf("X->%d ", (int) wormPos[0].x);
    // printf("Y->%d\n", (int) wormPos[0].y);

    // game over when worm touches void
    if (wormPos[currentLowestSegment].x >= voidYLevel) CloseWindow();
    
    currentAction = 0;

    if (shouldFall) {

        for (int i = 0; i < currentWormLength; i++) {
            map[(int) wormPos[i].x][(int) wormPos[i].y] = 0;
            wormPos[i].x += 1;
        }

        drawSnakeToMap();
        updateHeadCollisionState();
        checkShouldFall();

    }

    //else if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D)) && inputCooldown == 0) {
    else if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D))) {
        if (IsKeyPressed(KEY_A) && !headCollisionState[0]) currentAction = 1;
        if (IsKeyPressed(KEY_D) && !headCollisionState[1]) currentAction = 2;
        if (IsKeyPressed(KEY_W) && !headCollisionState[2]) currentAction = 3;
        if (IsKeyPressed(KEY_S) && !headCollisionState[3]) currentAction = 4;
        
        // make sure it will actually move
        if (currentAction != 0) {
            // tailPos gets updated to the last worm segment
            tailPos = wormPos[currentWormLength - 1];
            for (int i = 0; i < currentWormLength; i++) {
                // remove previous worm segment from the map
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
            
            // apple test
            if (wormPos[0].x == 6 && wormPos[0].y == 6) 
            wormPos[currentWormLength++] = tailPos;

            updateLowestSegment();
            drawSnakeToMap();
            updateHeadCollisionState();
            checkShouldFall();
        }
    }
    drawVisual();
}

int main() {
    initMap();
    InitWindow(screenWidth, screenHeight, gameName);
    SetTargetFPS(60);
    HideCursor();
    //ToggleFullscreen();

    // make this non hard code
    wormPos[currentWormLength++] = (Vector2) { 8, 7 };
    wormPos[currentWormLength++] = (Vector2) { 8, 6 };
    wormPos[currentWormLength++] = (Vector2) { 9, 6 };
    //wormPos[currentWormLength++] = (Vector2) { 6, 4 };

    // always draw intial worm state in the very beginning
    drawSnakeToMap();
    updateHeadCollisionState();

    // apple
    map[6][6] = 1;
    // test showing where block is
    map[6][13] = 5;
    map[6][12] = 5;
    map[6][11] = 5; 
    map[6][10] = 5;
    map[7][10] = 5;
    map[8][10] = 5;
    map[9][10] = 5;
    map[9][9] = 5;
    map[9][8] = 5;
    map[9][7] = 5;

    while (!WindowShouldClose()) gameLoop();

    CloseWindow();

    return 0;
}