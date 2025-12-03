#include "include/raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char gameName[] = "wormy";
const int squareReso = 60;
const int screenWidth = 1080;
const int screenHeight = 1080;
const int mapReso = 18; // screenWidth / squareReso;
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

Image appleImage;
Image portalImage;
Image blockImage;
Image headImageDown;
Image headImageLeft;
Image headImageUp;
Image headImageRight;
Texture2D headTexture;
Texture2D appleTexture;
Texture2D portalTexture;
Texture2D blockTexture;

Vector2 getPortal(int map) {
    switch (map) {
        case 1: return (Vector2) { 3, 13 };
        case 2: return (Vector2) { 7, 13 };
        case 3: return (Vector2) { 6, 12 };
        case 4: return (Vector2) { 10, 14 };
        case 5: return (Vector2) { 9, 16 };
    }
}

Texture2D getTextureFromId(int id) {
    switch(id) {
        case 2: return headTexture;
        case 3: return appleTexture;
        case 4: return blockTexture;
        default: return appleTexture;
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

void updateFacing() {
    if (IsKeyPressed(KEY_A)) headTexture = LoadTextureFromImage(headImageLeft);
    else if (IsKeyPressed(KEY_D)) headTexture = LoadTextureFromImage(headImageRight);
    else if (IsKeyPressed(KEY_W)) headTexture = LoadTextureFromImage(headImageUp);
    else if (IsKeyPressed(KEY_S)) headTexture = LoadTextureFromImage(headImageDown);
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
            map[7][6] = 3;
            map[7][13] = 4;
            map[7][12] = 4;
            map[7][11] = 4; 
            map[7][10] = 4;
            map[8][10] = 4;
            map[9][10] = 4;
            map[10][10] = 4;
            map[10][9] = 4;
            map[10][8] = 4;
            map[10][7] = 4;
            wormPos[currentWormLength++] = (Vector2) { 9, 7 };
            wormPos[currentWormLength++] = (Vector2) { 9, 6 };
            wormPos[currentWormLength++] = (Vector2) { 10, 6 };
            headTexture = LoadTextureFromImage(headImageRight);
            break;
        case 2:
            map[7][9] = 3;
            map[6][8] = 4;
            map[6][9] = 4;
            map[7][8] = 4;
            map[10][11] = 4;
            map[10][10] = 4;
            map[10][9] = 4;
            map[9][9] = 4;
            map[9][8] = 4;
            map[9][7] = 4;
            map[9][6] = 4;
            map[9][5] = 4;
            map[9][4] = 4;
            map[9][11] = 4;
            map[8][11] = 4;
            map[7][11] = 4;
            map[6][11] = 4;
            map[6][12] = 4;
            map[6][13] = 4;
            wormPos[currentWormLength++] = (Vector2) { 8, 6 };
            wormPos[currentWormLength++] = (Vector2) { 8, 5 };
            wormPos[currentWormLength++] = (Vector2) { 8, 4 };
            headTexture = LoadTextureFromImage(headImageRight);
            break;
        case 3:
            map[7][7] = 4;
            map[7][6] = 4;
            map[7][5] = 4;
            map[8][6] = 4;
            map[9][6] = 4;
            map[10][6] = 4;
            map[11][6] = 4;
            map[11][7] = 4;
            map[11][9] = 4;
            map[7][10] = 4;
            map[6][10] = 4;
            map[5][10] = 4;
            map[4][10] = 4;
            map[8][9] = 4;
            map[7][9] = 4;
            map[6][9] = 4;
            map[5][9] = 4;
            map[4][9] = 4;
            map[10][9] = 4;
            map[10][10] = 4;
            map[10][11] = 4;
            map[10][12] = 4;
            map[9][12] = 4;
            map[8][12] = 4;
            map[7][12] = 4;
            map[10][8] = 3;
            wormPos[currentWormLength++] = (Vector2) { 6, 7 };
            wormPos[currentWormLength++] = (Vector2) { 6, 6 };
            wormPos[currentWormLength++] = (Vector2) { 6, 5 };
            headTexture = LoadTextureFromImage(headImageRight);
            break;
        case 4:
            map[7][8] = 3;
            map[7][10] = 4;
            map[5][10] = 4;
            map[5][9] = 4;
            map[9][11] = 4;
            map[9][12] = 4;
            map[9][13] = 4;
            map[9][14] = 4;
            map[9][7] = 4;
            map[9][7] = 4;
            map[9][7] = 4;
            map[7][6] = 4;
            map[6][6] = 4;
            map[5][6] = 4;
            map[9][7] = 4;
            map[9][6] = 4;
            map[9][5] = 4;
            map[9][4] = 4;
            map[9][3] = 4;
            wormPos[currentWormLength++] = (Vector2) { 8, 6 };
            wormPos[currentWormLength++] = (Vector2) { 8, 5 };
            wormPos[currentWormLength++] = (Vector2) { 8, 4 };
            headTexture = LoadTextureFromImage(headImageRight);
            break;
        case 5:
            map[7][8] = 4;
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
            headTexture = LoadTextureFromImage(headImageUp);
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

void drawVisual() {
    Vector2 portal = getPortal(currentLevel);
    BeginDrawing();
    ClearBackground(BLACK);
    if (currentLevel != 6) {
        DrawText(TextFormat("Level %d", currentLevel), 25, 25, 30, WHITE);
        DrawText(TextFormat("Press \"R\" to reset", currentLevel), 380, 1000, 30, WHITE);
    } else {
        DrawText(TextFormat("The End!"), 350, 470, 80, WHITE);
    }
    for (int i = 0; i < mapReso; i++)
    for (int j = 0; j < mapReso; j++) {
        if (map[i][j] == 1) DrawRectangle(j * squareReso, i * squareReso, squareReso, squareReso, WHITE);
        if (map[i][j] != 0) DrawTexture(getTextureFromId(map[i][j]), j * squareReso, i * squareReso, WHITE);
        // render portal (above map)
        if (i == portal.x && j == portal.y) DrawTexture(portalTexture, j * squareReso, i * squareReso, WHITE); 
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

        updateFacing();

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

void loadSprites() {
    appleImage = LoadImage("sprites/apple.png");
    portalImage = LoadImage("sprites/portal.png");
    blockImage = LoadImage("sprites/block.png");
    headImageRight = LoadImage("sprites/head.png");

    ImageResizeNN(&headImageRight, 60, 60);
    ImageResizeNN(&blockImage, 60, 60);
    ImageResizeNN(&portalImage, 60, 60);
    ImageResizeNN(&appleImage, 60, 60);

    headImageDown = ImageCopy(headImageRight);
    headImageLeft = ImageCopy(headImageRight);
    headImageUp = ImageCopy(headImageRight);

    ImageRotateCW(&headImageDown);
    ImageRotateCW(&headImageLeft);
    ImageRotateCW(&headImageLeft);
    ImageRotateCCW(&headImageUp);

    headTexture = LoadTextureFromImage(headImageLeft);
    blockTexture = LoadTextureFromImage(blockImage);
    appleTexture = LoadTextureFromImage(appleImage);
    portalTexture = LoadTextureFromImage(portalImage);

    UnloadImage(portalImage);
    UnloadImage(appleImage);
    UnloadImage(blockImage);
}

void unloadSprites() {
    UnloadTexture(headTexture);
    UnloadTexture(blockTexture);
    UnloadTexture(portalTexture);
    UnloadTexture(appleTexture);
    UnloadImage(headImageRight);
    UnloadImage(headImageLeft);
    UnloadImage(headImageUp);
    UnloadImage(headImageDown);
}

int main() {
    InitWindow(screenWidth, screenHeight, gameName);
    SetTargetFPS(60);
    HideCursor();
    loadSprites();

    // always draw intial state in the very beginning
    loadMap(currentLevel);
    drawSnakeToMap();
    updateHeadCollisionState();

    while (!WindowShouldClose()) gameLoop();

    unloadSprites();
    CloseWindow();

    return 0;
 }
