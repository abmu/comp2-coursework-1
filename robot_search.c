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
const int waitTime = 500; // milliseconds

typedef struct robot {
    int x;
    int y;
    char dir;
} robot;

int getScreenPos(int coordinatePart) { // convert coordinate part (either x or y value) into an x or y position on screen
    return (coordinatePart * tileSize) + gridOffset;
}

void setGrid(void) {
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

void setHome(int x, int y) {
    if (grid[y][x] != 0) {
        fprintf(stderr,"This tile is already occupied\n");
        exit(3);
    }
    grid[y][x] = 3;
}

void drawGrid(void) {
    background();
    clear();
    for (int y = 0; y < gridHeight; y++) {
        int posY = getScreenPos(y);
        for (int x = 0; x < gridWidth; x++) {
            int posX = getScreenPos(x);
            if (grid[y][x] == 0) { // empty tile
                setColour(black);
                drawRect(posX,posY,tileSize,tileSize);
                continue;
            } else if (grid[y][x] == 1) { // marker tile
                setColour(gray);
            } else if (grid[y][x] == 2) { // obstacle tile
                setColour(black);
            } else { // home tile
                setColour(blue);
            }
            fillRect(posX,posY,tileSize,tileSize);
        }
    }
}

void drawRobot(robot* bot) {
    foreground();
    clear();
    setColour(green);
    int posX = getScreenPos(bot->x);
    int posY = getScreenPos(bot->y);
    if (bot->dir == 'N') {
        int xPoints[3] = {posX + 0.5*tileSize, posX + tileSize, posX};
        int yPoints[3] = {posY, posY + tileSize, posY + tileSize};
        fillPolygon(3,xPoints,yPoints);
    } else if (bot->dir == 'E') {
        int xPoints[3] = {posX + tileSize, posX, posX};
        int yPoints[3] = {posY + 0.5*tileSize, posY + tileSize, posY};
        fillPolygon(3,xPoints,yPoints);
    } else if (bot->dir == 'S') {
        int xPoints[3] = {posX + 0.5*tileSize, posX, posX + tileSize};
        int yPoints[3] = {posY + tileSize, posY, posY};
        fillPolygon(3,xPoints,yPoints);
    } else { // robot facing west
        int xPoints[3] = {posX, posX + tileSize, posX + tileSize};
        int yPoints[3] = {posY + 0.5*tileSize, posY, posY + tileSize};
        fillPolygon(3,xPoints,yPoints);
    }
    sleep(waitTime);
}

int canMoveForward(robot* bot) {
    if (bot->dir == 'N') {
        int forwardY = bot->y - 1;
        if (forwardY >= 0 && grid[forwardY][bot->x] != 2) { // 2 is obstacle tile
            return 1;
        }
    } else if (bot->dir == 'E') {
        int forwardX = bot->x + 1;
        if (forwardX < gridWidth && grid[bot->y][forwardX] != 2) {
            return 1;
        }
    } else if (bot->dir == 'S') {
        int forwardY = bot->y + 1;
        if (forwardY < gridHeight && grid[forwardY][bot->x] != 2) {
            return 1;
        }
    } else { // robot facing west
        int forwardX = bot->x - 1;
        if (forwardX >= 0 && grid[bot->y][forwardX] != 2) {
            return 1;
        }
    }
    return 0;
}

void forward(robot* bot) {
    if (canMoveForward(bot)) {
        if (bot->dir == 'N') {
            bot->y--;
        } else if (bot->dir == 'E') {
            bot->x++;
        } else if (bot->dir == 'S') {
            bot->y++;
        } else { // robot facing west
            bot->x--;
        }
    }
    drawRobot(bot);
}

void left(robot* bot) {
    if (bot->dir == 'N') {
        bot->dir = 'W';
    } else if (bot->dir == 'E') {
        bot->dir = 'N';
    } else if (bot->dir == 'S') {
        bot->dir = 'E';
    } else { // robot facing west
        bot->dir = 'S';
    }
    drawRobot(bot);
}

void right(robot* bot) {
    if (bot->dir == 'N') {
        bot->dir = 'E';
    } else if (bot->dir == 'E') {
        bot->dir = 'S';
    } else if (bot->dir == 'S') {
        bot->dir = 'W';
    } else { // robot facing west
        bot->dir = 'N';
    }
    drawRobot(bot);
}

int atMarker(robot* bot) {
    if (grid[bot->y][bot->x] == 1) {
        return 1;
    }
    return 0;
}

int atHome(robot* bot) {
    if (grid[bot->y][bot->x] == 3) {
        return 1;
    }
    return 0;
}

void moveRobot(robot* bot) {
    drawRobot(bot);
    while (!atMarker(bot)) {
        right(bot);
        while (!atMarker(bot) && canMoveForward(bot)) {
            forward(bot);
        }
    }
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
    setGrid();
    setHome(startX,startY);
    drawGrid();

    moveRobot(&robot1);
    return 0;
}