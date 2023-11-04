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
int markersLeft = 0;
const int waitTime = 50; // milliseconds

typedef struct robot {
    int x;
    int y;
    char dir;
    int hasMarker;
    int numMoves;
    char* prevMoves;
} robot;

int getScreenX(int xCoordinate) {
    return (xCoordinate * tileSize) + gridOffset;
}

int getScreenY(int yCoordinate) {
    return (yCoordinate * tileSize) + gridOffset;
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
    markersLeft = 0;
    for (int y = 0; y < gridHeight; y++) {
        int posY = getScreenY(y);
        for (int x = 0; x < gridWidth; x++) {
            int posX = getScreenX(x);
            if (grid[y][x] == 0) { // empty tile
                setColour(black);
                drawRect(posX,posY,tileSize,tileSize);
                continue;
            } else if (grid[y][x] == 1) { // marker tile
                setColour(lightgray);
                markersLeft++;
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
    if (bot->hasMarker) {
        setColour(lightgray);
    } else {
        setColour(green);
    }
    int posX = getScreenX(bot->x);
    int posY = getScreenY(bot->y);
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
    // check if there is a wall or obstacle
    if (bot->dir == 'N') {
        int forwardY = bot->y - 1;
        return (forwardY >= 0 && grid[forwardY][bot->x] != 2); // 2 is obstacle tile
    } else if (bot->dir == 'E') {
        int forwardX = bot->x + 1;
        return (forwardX < gridWidth && grid[bot->y][forwardX] != 2);
    } else if (bot->dir == 'S') {
        int forwardY = bot->y + 1;
        return (forwardY < gridHeight && grid[forwardY][bot->x] != 2);
    } else { // robot facing west
        int forwardX = bot->x - 1;
        return (forwardX >= 0 && grid[bot->y][forwardX] != 2);
    }
}

int isObstacle(robot* bot) {
    if (bot->dir == 'N') {
        int forwardY = bot->y - 1;
        return (forwardY >= 0 && grid[forwardY][bot->x] == 2); // 2 is obstacle tile
    } else if (bot->dir == 'E') {
        int forwardX = bot->x + 1;
        return (forwardX < gridWidth && grid[bot->y][forwardX] == 2);
    } else if (bot->dir == 'S') {
        int forwardY = bot->y + 1;
        return (forwardY < gridHeight && grid[forwardY][bot->x] == 2);
    } else { // robot facing west
        int forwardX = bot->x - 1;
        return (forwardX >= 0 && grid[bot->y][forwardX] == 2);
    }
}

void updateMoves(robot* bot, char newMove) {
    if (newMove == '-') { // delete last move from arr
        bot->numMoves--;
        bot->prevMoves = realloc(bot->prevMoves, bot->numMoves);
        return;
    }
    bot->numMoves++;
    bot->prevMoves = realloc(bot->prevMoves, bot->numMoves);
    bot->prevMoves[bot->numMoves - 1] = newMove; // access last index and add new move
}

void forward(robot* bot) {
    if (canMoveForward(bot)) {
        updateMoves(bot, 'F');
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
    updateMoves(bot, 'L');
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
    updateMoves(bot, 'R');
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

int atHome(robot* bot) {
    if (grid[bot->y][bot->x] == 3) {
        return 1;
    }
    return 0;
}

int atMarker(robot* bot) {
    if (grid[bot->y][bot->x] == 1) {
        return 1;
    }
    return 0;
}

int isCarryingAMarker(robot* bot) {
    return bot->hasMarker;
}

void pickUpMarker(robot* bot) {
    if (atMarker(bot) && !isCarryingAMarker(bot)) {
        grid[bot->y][bot->x] = 0;
        bot->hasMarker = 1;
    }
    drawGrid();
    drawRobot(bot);
}

void dropMarker(robot* bot) {
    if (isCarryingAMarker(bot) && grid[bot->y][bot->x] != 2) { // 2 is obstacle tile
        if (grid[bot->y][bot->x] == 0) { // empty tile
            grid[bot->y][bot->x] = 1;
            bot->hasMarker = 0;
        } else if (grid[bot->y][bot->x] == 3) { // home tile
            bot->hasMarker = 0;
        } else { // marker tile
            // TO DO - allow multiple markers to be dropped on one tile
        }
    }
    drawGrid();
    drawRobot(bot);
}

void reverseMoves(robot* bot) {
    // face opposite direction
    right(bot);
    right(bot);
    // remove additional reversing moves from prevMoves
    updateMoves(bot, '-');
    updateMoves(bot, '-');
    while (!atHome(bot) && bot->numMoves > 0) {
        char lastMove = bot->prevMoves[bot->numMoves-1];
        updateMoves(bot, '-'); // remove last robot move
        if (lastMove == 'F') {
            forward(bot);
        } else if (lastMove == 'L') {
            right(bot);
        } else { // lastMove is right
            left(bot);
        }
        updateMoves(bot, '-');
    }
    while (bot->numMoves > 0) {
        updateMoves(bot, '-');
    }
}

void algorithmStage4(robot* bot) {
    // may get in a loop if 4 blocks are placed in a specific way
    while (markersLeft > 0) {
        while (!atMarker(bot)) {
            right(bot);
            while (!atMarker(bot) && canMoveForward(bot)) {
                forward(bot);
            }
        }
        pickUpMarker(bot);
        reverseMoves(bot);
        dropMarker(bot);
    }
}

char getOppositeDir(char overallDir) {
    if (overallDir == 'E') {
        return 'W';
    }
    return 'E';
}

void algorithmStage6(robot* bot) {
    // may not work if more than one obstacle is placed within a given 2x2 section of the grid
    char overallDir = 'W';
    while (markersLeft > 0) {
        while (bot->dir != 'S') {
            left(bot);
        }
        while (!atMarker(bot)) {
            if (isObstacle(bot)) {
                // move around obstacle
                right(bot);
                if (canMoveForward(bot)) {
                    forward(bot);
                    if (atMarker(bot)) {
                        break;
                    }
                    left(bot);
                    forward(bot);
                    if (atMarker(bot)) {
                        break;
                    }
                    forward(bot);
                    if (atMarker(bot)) {
                        break;
                    }
                    left(bot);
                    forward(bot);
                    right(bot);
                } else { // go around the other side
                    left(bot);
                    left(bot);
                    forward(bot);
                    if (atMarker(bot)) {
                        break;
                    }
                    right(bot);
                    forward(bot);
                    if (atMarker(bot)) {
                        break;
                    }
                    forward(bot);
                    if (atMarker(bot)) {
                        break;
                    }
                    right(bot);
                    forward(bot);
                    left(bot);
                }
            } else {
                if ((bot->dir == 'N' && overallDir == 'E') || (bot->dir == 'S' && overallDir == 'W')) {
                    right(bot);
                    if (canMoveForward(bot)) {
                        forward(bot);
                    } else {
                        if (isObstacle(bot)) {
                            right(bot);
                            forward(bot);
                            if (atMarker(bot)) {
                                break;
                            }
                            left(bot);
                            forward(bot);
                        } else { // a wall has been reached
                            overallDir = getOppositeDir(overallDir);
                        }
                    }
                    right(bot);
                } else {
                    left(bot);
                    if (canMoveForward(bot)) {
                        forward(bot);
                    } else {
                        if (isObstacle(bot)) {
                            left(bot);
                            forward(bot);
                            if (atMarker(bot)) {
                                break;
                            }
                            right(bot);
                            forward(bot);
                        } else { // a wall has been reached
                            overallDir = getOppositeDir(overallDir);
                        }
                    }
                    left(bot);
                }
            }
            while (!atMarker(bot) && canMoveForward(bot)) {
                forward(bot);
            }
        }
        pickUpMarker(bot);
        reverseMoves(bot);
        dropMarker(bot);
    }
}

void moveRobot(robot* bot) {
    drawRobot(bot);
    //algorithmStage4(bot);
    algorithmStage6(bot);
}

int getStartPos(char* positionArg, int boundary) {
    int pos = strtol(positionArg, NULL, 10);
    if (pos >= 0 && pos < boundary) {
        return pos;
    }
    return 0; // default position if argument is invalid
}

char getStartDir(char* directionArg) {
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
    int startX = getStartPos(argv[1], gridWidth);
    int startY = getStartPos(argv[2], gridHeight);
    char startDir = getStartDir(argv[3]);
    robot robot1 = {startX, startY, startDir, 0, 0};
    
    setWindowSize(winSize,winSize);
    setGrid();
    setHome(startX,startY);
    drawGrid();

    moveRobot(&robot1);
    free(robot1.prevMoves);
    return 0;
}