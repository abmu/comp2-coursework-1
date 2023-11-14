#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "graphics.h"
#include "grid.h"
#include "robot.h"

const int winSize = 800;
const int gridHeight = 10;
const int gridWidth = 10;
const int tileSize = 60;
const int gridOffset = 100;
const int moveLimit = 500;

void algorithmStage4(robot* botPtr, grid* gridPtr) {
    // will get in a loop if 4 blocks are placed in a specific way
    while (gridPtr->markersLeft > 0) {
        while (!atMarker(botPtr, gridPtr)) {
            right(botPtr, gridPtr);
            while (!atMarker(botPtr, gridPtr) && canMoveForward(botPtr, gridPtr)) {
                forward(botPtr, gridPtr);
            }
        }
        pickUpMarker(botPtr, gridPtr);
        reverseMoves(botPtr, gridPtr);
        dropMarker(botPtr, gridPtr);
    }
}

char getOppositeDir(char overallDir) {
    if (overallDir == 'E') {
        return 'W';
    }
    return 'E';
}

void algorithmStage6(robot* botPtr, grid* gridPtr) {
    // may not work if more than one obstacle is placed within a given 2x2 section of the grid
    // will get in a loop if there is an obstacle in the top corner and another in the bottom corner on one of the sides
    // will skip specific positions if there are 2 obstacles on the very top with a gap in between, and an obstacle on the very bottom below the right obstacle
    // will skip specific positions if there are 2 obstacles on the very bottom with a gap in between, and an obstacle on the very top above the left obstacle
    char overallDir = 'W';
    while (gridPtr->markersLeft > 0) {
        while (botPtr->dir != 'S') {
            left(botPtr, gridPtr);
        }
        while (!atMarker(botPtr, gridPtr)) {
            if (botPtr->numMoves >= moveLimit) {
                fprintf(stderr,"Move limit will be reached and marker won't be found\n");
                exit(4);
            }
            if (isObstacle(botPtr, gridPtr)) {
                // move around obstacle
                right(botPtr, gridPtr);
                if (canMoveForward(botPtr, gridPtr)) {
                    forward(botPtr, gridPtr);
                    if (atMarker(botPtr, gridPtr)) {
                        break;
                    }
                    left(botPtr, gridPtr);
                    forward(botPtr, gridPtr);
                    if (atMarker(botPtr, gridPtr)) {
                        break;
                    }
                    forward(botPtr, gridPtr);
                    if (atMarker(botPtr, gridPtr)) {
                        break;
                    }
                    left(botPtr, gridPtr);
                    forward(botPtr, gridPtr);
                    right(botPtr, gridPtr);
                } else { // go around the other side
                    left(botPtr, gridPtr);
                    left(botPtr, gridPtr);
                    forward(botPtr, gridPtr);
                    if (atMarker(botPtr, gridPtr)) {
                        break;
                    }
                    right(botPtr, gridPtr);
                    forward(botPtr, gridPtr);
                    if (atMarker(botPtr, gridPtr)) {
                        break;
                    }
                    forward(botPtr, gridPtr);
                    if (atMarker(botPtr, gridPtr)) {
                        break;
                    }
                    right(botPtr, gridPtr);
                    forward(botPtr, gridPtr);
                    left(botPtr, gridPtr);
                }
            } else {
                if ((botPtr->dir == 'N' && overallDir == 'E') || (botPtr->dir == 'S' && overallDir == 'W')) {
                    right(botPtr, gridPtr);
                    if (canMoveForward(botPtr, gridPtr)) {
                        forward(botPtr, gridPtr);
                    } else {
                        if (isObstacle(botPtr, gridPtr)) {
                            right(botPtr, gridPtr);
                            forward(botPtr, gridPtr);
                            if (atMarker(botPtr, gridPtr)) {
                                break;
                            }
                            left(botPtr, gridPtr);
                            forward(botPtr, gridPtr);
                        } else { // a wall has been reached
                            overallDir = getOppositeDir(overallDir);
                        }
                    }
                    right(botPtr, gridPtr);
                } else {
                    left(botPtr, gridPtr);
                    if (canMoveForward(botPtr, gridPtr)) {
                        forward(botPtr, gridPtr);
                    } else {
                        if (isObstacle(botPtr, gridPtr)) {
                            left(botPtr, gridPtr);
                            forward(botPtr, gridPtr);
                            if (atMarker(botPtr, gridPtr)) {
                                break;
                            }
                            right(botPtr, gridPtr);
                            forward(botPtr, gridPtr);
                        } else { // a wall has been reached
                            overallDir = getOppositeDir(overallDir);
                        }
                    }
                    left(botPtr, gridPtr);
                }
            }
            while (!atMarker(botPtr, gridPtr) && canMoveForward(botPtr, gridPtr)) {
                forward(botPtr, gridPtr);
            }
        }
        pickUpMarker(botPtr, gridPtr);
        reverseMoves(botPtr, gridPtr);
        dropMarker(botPtr, gridPtr);
    }
}

void moveRobot(robot* botPtr, grid* gridPtr) {
    drawRobot(botPtr, gridPtr);
    //algorithmStage4(botPtr, gridPtr);
    algorithmStage6(botPtr, gridPtr);
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
    grid tileGrid = {gridHeight, gridWidth, tileSize, gridOffset};

    int startX = getStartPos(argv[1], gridWidth);
    int startY = getStartPos(argv[2], gridHeight);
    char startDir = getStartDir(argv[3]);
    int hasMarker = 0;
    int numMoves = 0;
    robot bot = {startX, startY, startDir, hasMarker, numMoves};
    
    setWindowSize(winSize, winSize);
    setGrid(&tileGrid);
    setHome(startX, startY, &tileGrid);
    drawGrid(&tileGrid);
    
    moveRobot(&bot, &tileGrid);
    free(bot.prevMoves);
    return 0;
}