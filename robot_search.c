#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "./graphics.h"

const int winSize = 800;
const int tileSize = 60;
const int gridOffset = 100;
const int gridHeight = 10;
const int gridWidth = 10;
int grid[10][10];
const int waitTime = 25;

typedef struct robot {
    int x;
    int y;
    char dir;
} robot;

typedef struct trianglePolygon {
    int xPoints[3];
    int yPoints[3];
} triangle;

// create triangles facing different directions
triangle northTriangle = {{0.5*tileSize, tileSize, 0}, {0, tileSize, tileSize}};
triangle eastTriangle = {{tileSize, 0, 0}, {0.5*tileSize, tileSize, 0}};
triangle southTriangle = {{0.5*tileSize, 0, tileSize},{tileSize, 0, 0}};
triangle westTriangle = {{0, tileSize, tileSize},{0.5*tileSize, 0, tileSize}};

int getScreenPos(int coordinatePart) { // convert either grid x or y coordinate into an x or y position on screen
    return (coordinatePart * tileSize) + gridOffset;
}

void getGrid(void) {
    FILE* fp = fopen("./grid.txt","r");
    if (fp == NULL) {
        fprintf(stderr,"Couldn't find the file ./grid.txt\n");
        exit(2);
    }
    for (int y = 0; y < gridHeight; y ++) {
        for (int x = 0; x < gridWidth; x++) {
            // read an element from the file and place it into the 2d array
            fscanf(fp, "%d,", &grid[y][x]);
        }
    }
    fclose(fp);
}

void drawGrid(void) {
    getGrid();

    background();
    for (int y = 0; y < gridHeight; y++) {
        int posY = getScreenPos(y);
        for (int x = 0; x < gridWidth; x++) {
            int posX = getScreenPos(x);
            if (grid[y][x] == 0) { // empty tile
                setColour(black);
                drawRect(posX,posY,tileSize,tileSize);
            } else if (grid[y][x] == 1) { // home tile
                setColour(blue);
                fillRect(posX,posY,tileSize,tileSize);
            } else { // obstacle tile
                setColour(red);
                fillRect(posX,posY,tileSize,tileSize);
            }
        }
    }
}

void triangleDuplication(triangle* dest, triangle* src) {
    size_t memSize = 3 * sizeof(int);
    memcpy(dest->xPoints, src->xPoints, memSize);
    memcpy(dest->yPoints, src->yPoints, memSize);
}

void drawRobot(robot* bot) {
    foreground();
    setColour(green);
    int posX = getScreenPos(bot->x);
    int posY = getScreenPos(bot->y);
    triangle screenTriangle;
    if (bot->dir == 'N') {
        triangleDuplication(&screenTriangle, &northTriangle);
    } else if (bot->dir == 'E') {
        triangleDuplication(&screenTriangle, &eastTriangle);
    } else if (bot->dir == 'S') {
        triangleDuplication(&screenTriangle, &southTriangle);
    } else {
        triangleDuplication(&screenTriangle, &westTriangle);
    }
    // offset the points so that the triangle is shown within the correct tile
    for (int i = 0; i < 3; i++) {
        screenTriangle.xPoints[i] += posX;
        screenTriangle.yPoints[i] += posY;
    }
    fillPolygon(3,screenTriangle.xPoints,screenTriangle.yPoints);
}

void moveRobot(robot* bot) {
    drawRobot(bot);
}

int getPosition(char* positionArg, int boundary) {
    int pos = strtol(positionArg, NULL, 10);
    if (pos >= 0 && pos < boundary) {
        return pos;
    }
    return 0; // default position if argument is invalid
}

char getDirection(char* directionArg) {
    char dir = toupper(directionArg[0]);
    if (dir == 'N' || dir == 'E' || dir == 'S' || dir == 'W') {
        return dir;
    }
    return 'N'; // default direction if argument is invalid
}

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr,"Invalid number of arguments\n");
        return 1;
    }

    int startX = getPosition(argv[1], gridWidth);
    int startY = getPosition(argv[2], gridHeight);
    char startDir = getDirection(argv[3]);
    robot robot1 = {startX, startY, startDir};
    
    setWindowSize(winSize,winSize);
    drawGrid();
    moveRobot(&robot1);
    return 0;
}