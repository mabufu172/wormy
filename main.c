#include "include/raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char gameName[] = "wormy";
const int squareReso = 60;
const int screenWidth = 1080;
const int screenHeight = 1080;
const int mapReso = screenWidth / squareReso;
const int voidYLevel = 19;

int map[20][18]; //1080 / square reso, 20 is to allow worm to fall beyond map
int currentAction = 0; // 0 = stationary, 1 = left, 2 = right, 3 = up, 4 = down
int currentWormLength = 0;
int currentLowestSegment; // int of the index of wormPos
int currentLevel = 1;

bool headCollisionState[4]; // true == will collide, false == free to go
bool shouldFall;
Vector2 wormPos[30]; // max worm length is 30
Vector2 tailPos;

Vector2 getPortal(int map) {
    switch (map) {
        case 1: return (Vector2) { 3, 13 };
        case 2: return (Vector2) { 7, 13 };
        case 3: return (Vector2) { 6, 12 };
        case 4: return (Vector2) { 10, 14 };
        case 5: return (Vector2) { 9, 16 };
    }
}

bool checkCollision(Vector2 positionInMap, int direction, bool ignoreWorm) {
    
    // setting num to two causes it to ignore worm as worm IDs are 2 and 3 (head and body)
    int num = 0;
    if (ignoreWorm) num = 2;

    switch (direction) {
        case 1: return map[(int) positionInMap.x][(int) positionInMap.y - 1] > num;
        case 2: return map[(int) positionInMap.x][(int) positionInMap.y + 1] > num;
        case 3: return map[(int) positionInMap.x - 1][(int) positionInMap.y] > num;
        case 4: return map[(int) positionInMap.x + 1][(int) positionInMap.y] > num;
    }

    return false;

}

bool canMove() {
    if (IsKeyPressed(KEY_A)) return !headCollisionState[0];
    if (IsKeyPressed(KEY_D)) return !headCollisionState[1];
    if (IsKeyPressed(KEY_W)) return !headCollisionState[2];
    if (IsKeyPressed(KEY_S)) return !headCollisionState[3];
}

bool isGoingToEatApple() {
    if (IsKeyPressed(KEY_A) && map[(int) wormPos[0].x][(int) wormPos[0].y - 1] == 3) {
        wormPos[currentWormLength++] = tailPos;
        return true;
    }
    if (IsKeyPressed(KEY_D) && map[(int) wormPos[0].x][(int) wormPos[0].y + 1] == 3) {
        wormPos[currentWormLength++] = tailPos;
        return true;
    }
    if (IsKeyPressed(KEY_W) && map[(int) wormPos[0].x - 1][(int) wormPos[0].y] == 3) {
        wormPos[currentWormLength++] = tailPos;
        return true;
    }
    if (IsKeyPressed(KEY_S) && map[(int) wormPos[0].x + 1][(int) wormPos[0].y] == 3) {
        wormPos[currentWormLength++] = tailPos;
        return true;
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

void loadMap(int level) {

    // fill everything with zeros (default; blanks)
    for (int i = 0; i < mapReso; i++)
    for (int j = 0; j < mapReso; j++)
    map[i][j] = 0;

    switch (level) {
        case 1:
            // apple
            map[7][6] = 3;

            // test showing where block is
            map[7][13] = 5;
            map[7][12] = 5;
            map[7][11] = 5; 
            map[7][10] = 5;
            map[8][10] = 5;
            map[9][10] = 5;
            map[10][10] = 5;
            map[10][9] = 5;
            map[10][8] = 5;
            map[10][7] = 5;

            wormPos[currentWormLength++] = (Vector2) { 9, 7 };
            wormPos[currentWormLength++] = (Vector2) { 9, 6 };
            wormPos[currentWormLength++] = (Vector2) { 10, 6 };
            break;
        case 2:
            // apple
            map[7][9] = 3;

            // map[y][x]
            map[6][8] = 5;
            map[6][9] = 5;
            map[7][8] = 5;

            map[10][11] = 5;
            map[10][10] = 5;
            map[10][9] = 5;
            map[9][9] = 5;
            map[9][8] = 5;
            map[9][7] = 5;
            map[9][6] = 5;
            map[9][5] = 5;
            map[9][4] = 5;

            map[9][11] = 5;
            map[8][11] = 5;
            map[7][11] = 5;
            map[6][11] = 5;
            map[6][12] = 5;
            map[6][13] = 5;

            wormPos[currentWormLength++] = (Vector2) { 8, 6 };
            wormPos[currentWormLength++] = (Vector2) { 8, 5 };
            wormPos[currentWormLength++] = (Vector2) { 8, 4 };
            break;
        case 3:

            // map[y][x]
            map[7][7] = 5;
            map[7][6] = 5;
            map[7][5] = 5;

            map[8][6] = 5;
            map[9][6] = 5;
            map[10][6] = 5;
            map[11][6] = 5;
            map[11][7] = 5;
            map[11][9] = 5;

            map[7][10] = 5;
            map[6][10] = 5;
            map[5][10] = 5;
            map[4][10] = 5;
            //
            map[8][9] = 5;
            map[7][9] = 5;
            map[6][9] = 5;
            map[5][9] = 5;
            map[4][9] = 5;

            map[10][9] = 5;
            map[10][10] = 5;
            map[10][11] = 5;
            map[10][12] = 5;
            map[9][12] = 5;
            map[8][12] = 5;
            map[7][12] = 5;

            //apple
            map[10][8] = 3;

            wormPos[currentWormLength++] = (Vector2) { 6, 7 };
            wormPos[currentWormLength++] = (Vector2) { 6, 6 };
            wormPos[currentWormLength++] = (Vector2) { 6, 5 };
            break;
        case 4:
            // apple
            map[7][8] = 3;

            map[7][10] = 5;
            map[5][10] = 5;
            map[5][9] = 5;

            map[9][11] = 5;
            map[9][12] = 5;
            map[9][13] = 5;
            map[9][14] = 5;

            // map[y][x]
            map[9][7] = 5;
            map[9][7] = 5;
            map[9][7] = 5;

            map[7][6] = 5;
            map[6][6] = 5;
            map[5][6] = 5;

            map[9][7] = 5;
            map[9][6] = 5;
            map[9][5] = 5;
            map[9][4] = 5;
            map[9][3] = 5;

            wormPos[currentWormLength++] = (Vector2) { 8, 6 };
            wormPos[currentWormLength++] = (Vector2) { 8, 5 };
            wormPos[currentWormLength++] = (Vector2) { 8, 4 };
            break;
        case 5:
            map[7][8] = 5;

            //apple
            map[8][9] = 3;
            map[8][7] = 3;
            map[9][10] = 3;
            map[9][8] = 3;
            map[9][6] = 3;
            map[10][9] = 3;
            map[10][7] = 3;
            map[11][8] = 3;

            wormPos[currentWormLength++] = (Vector2) { 4, 8 };
            wormPos[currentWormLength++] = (Vector2) { 5, 8 };
            wormPos[currentWormLength++] = (Vector2) { 6, 8 };
            break;
    }
}

void drawSnakeToMap() {
    for (int i = 0; i < currentWormLength; i++)
    if (i == 0) map[(int)wormPos[i].x][(int)wormPos[i].y] = 2;
    else map[(int)wormPos[i].x][(int)wormPos[i].y] = 1;
}

void resetState() {
    // clear out worm array
    currentWormLength = 0;
    memset(wormPos, 0, sizeof(wormPos));
    loadMap(currentLevel);
    drawSnakeToMap();
    updateHeadCollisionState();
    checkShouldFall();
}

void moveWorm() {
    if (IsKeyPressed(KEY_A)) wormPos[0].y -= 1;
    else if (IsKeyPressed(KEY_D)) wormPos[0].y += 1;
    else if (IsKeyPressed(KEY_W)) wormPos[0].x -= 1;
    else if (IsKeyPressed(KEY_S)) wormPos[0].x += 1;
}

Color getColorFromId(int id) {
    switch(id) {
        case 1: return GREEN;
        case 2: return DARKGREEN;
        case 3: return RED;
        case 4: return GRAY;
        case 5: return BROWN;
    }
    return PURPLE; // fallback
}

void drawVisual() {
    Vector2 portal = getPortal(currentLevel);
    BeginDrawing();
    ClearBackground(BLACK);
    if (currentLevel != 6) {
        (TextFormat("Level %d", currentLevel), 25, 25, 30, WHITE);
        DrawText(TextFormat("Press \"R\" to reset", currentLevel), 380, 1000, 30, WHITE);
    } else {
        DrawText(TextFormat("The End!"), 330, 460, 80, WHITE);
    }
    for (int i = 0; i < mapReso; i++)
    for (int j = 0; j < mapReso; j++) {
        if (map[i][j] == 3) DrawCircle(j * squareReso + squareReso / 2, i * squareReso + squareReso / 2, squareReso / 2, RED);
        else if (map[i][j] > 0) DrawRectangle(j * squareReso, i * squareReso, squareReso, squareReso, getColorFromId(map[i][j]));

        // render portal (above map)
        if (i == portal.x && j == portal.y) DrawCircle(j * squareReso + squareReso / 2, i * squareReso + squareReso / 2, squareReso / 2, PURPLE);
    }
    EndDrawing();
}

void gameLoop() {
    
    // game over when worm touches void
    if (wormPos[currentLowestSegment].x >= voidYLevel) resetState();

    // listening for R to reset level
    if (IsKeyPressed(KEY_R)) resetState();

    if (shouldFall){

        for (int i = 0; i < currentWormLength; i++) {
            map[(int) wormPos[i].x][(int) wormPos[i].y] = 0;
            wormPos[i].x += 1;
        }

        drawSnakeToMap();
        updateHeadCollisionState();
        checkShouldFall();

    }

    else if (canMove() || isGoingToEatApple()) {

        // tailPos gets updated to the last worm segment
        tailPos = wormPos[currentWormLength - 1];
        for (int i = 0; i < currentWormLength; i++) {
            // remove previous worm segment from the map
            map[(int) wormPos[i + 1].x][(int) wormPos[i + 1].y] = 0;
                
            // inherit n worm segment to n - 1 worm segment
            wormPos[currentWormLength - i] = wormPos[(currentWormLength - i) - 1];
        }

        // update worm head position, in this state, wormPos[0] (head) actually collides with wormPos[1]
        moveWorm();
            
        //check if it touches portal
        if (wormPos[0].x == getPortal(currentLevel).x && wormPos[0].y == getPortal(currentLevel).y) {
            currentLevel++;
            resetState();
        }
        
        updateLowestSegment();
        drawSnakeToMap();
        updateHeadCollisionState();
        checkShouldFall();

    }
    drawVisual();
}

int main() {
    InitWindow(screenWidth, screenHeight, gameName);
    SetTargetFPS(60);
    HideCursor();
    //ToggleFullscreen();

    // always draw intial worm state in the very beginning
    loadMap(currentLevel);
    drawSnakeToMap();
    updateHeadCollisionState();

    while (!WindowShouldClose()) gameLoop();

    CloseWindow();

    return 0;
 }