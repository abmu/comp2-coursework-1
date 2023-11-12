#include <stdlib.h>
#include "graphics.h"
#include "grid.h"
#include "robot.h"

const int waitTime = 50; // milliseconds

void drawRobot(robot* botPtr, grid* gridPtr) {
    foreground();
    clear();
    if (botPtr->hasMarker) {
        setColour(lightgray);
    } else {
        setColour(green);
    }
    int squareSize = gridPtr->tileSize;
    int posX = getScreenX(botPtr->x, gridPtr);
    int posY = getScreenY(botPtr->y, gridPtr);
    if (botPtr->dir == 'N') {
        int xPoints[3] = {posX + 0.5*squareSize, posX + squareSize, posX};
        int yPoints[3] = {posY, posY + squareSize, posY + squareSize};
        fillPolygon(3, xPoints, yPoints);
    } else if (botPtr->dir == 'E') {
        int xPoints[3] = {posX + squareSize, posX, posX};
        int yPoints[3] = {posY + 0.5*squareSize, posY + squareSize, posY};
        fillPolygon(3, xPoints, yPoints);
    } else if (botPtr->dir == 'S') {
        int xPoints[3] = {posX + 0.5*squareSize, posX, posX + squareSize};
        int yPoints[3] = {posY + squareSize, posY, posY};
        fillPolygon(3, xPoints, yPoints);
    } else { // robot facing west
        int xPoints[3] = {posX, posX + squareSize, posX + squareSize};
        int yPoints[3] = {posY + 0.5*squareSize, posY, posY + squareSize};
        fillPolygon(3, xPoints, yPoints);
    }
    sleep(waitTime);
}

int canMoveForward(robot* botPtr, grid* gridPtr) {
    // check if there is a wall or obstacle
    if (botPtr->dir == 'N') {
        int forwardY = botPtr->y - 1;
        return (forwardY >= 0 && gridPtr->tiles[forwardY][botPtr->x] != 2); // 2 is obstacle tile
    } else if (botPtr->dir == 'E') {
        int forwardX = botPtr->x + 1;
        return (forwardX < gridPtr->width && gridPtr->tiles[botPtr->y][forwardX] != 2);
    } else if (botPtr->dir == 'S') {
        int forwardY = botPtr->y + 1;
        return (forwardY < gridPtr->height && gridPtr->tiles[forwardY][botPtr->x] != 2);
    } else { // robot facing west
        int forwardX = botPtr->x - 1;
        return (forwardX >= 0 && gridPtr->tiles[botPtr->y][forwardX] != 2);
    }
}

int isObstacle(robot* botPtr, grid* gridPtr) {
    if (botPtr->dir == 'N') {
        int forwardY = botPtr->y - 1;
        return (forwardY >= 0 && gridPtr->tiles[forwardY][botPtr->x] == 2); // 2 is obstacle tile
    } else if (botPtr->dir == 'E') {
        int forwardX = botPtr->x + 1;
        return (forwardX < gridPtr->width && gridPtr->tiles[botPtr->y][forwardX] == 2);
    } else if (botPtr->dir == 'S') {
        int forwardY = botPtr->y + 1;
        return (forwardY < gridPtr->height && gridPtr->tiles[forwardY][botPtr->x] == 2);
    } else { // robot facing west
        int forwardX = botPtr->x - 1;
        return (forwardX >= 0 && gridPtr->tiles[botPtr->y][forwardX] == 2);
    }
}

void updateMoves(robot* botPtr, char newMove) {
    if (newMove == '-') { // delete last move from arr
        botPtr->numMoves--;
        botPtr->prevMoves = realloc(botPtr->prevMoves, botPtr->numMoves);
        return;
    }
    botPtr->numMoves++;
    botPtr->prevMoves = realloc(botPtr->prevMoves, botPtr->numMoves);
    botPtr->prevMoves[botPtr->numMoves - 1] = newMove; // access last index and add new move
}

void forward(robot* botPtr, grid* gridPtr) {
    if (canMoveForward(botPtr, gridPtr)) {
        updateMoves(botPtr, 'F');
        if (botPtr->dir == 'N') {
            botPtr->y--;
        } else if (botPtr->dir == 'E') {
            botPtr->x++;
        } else if (botPtr->dir == 'S') {
            botPtr->y++;
        } else { // robot facing west
            botPtr->x--;
        }
    }
    drawRobot(botPtr, gridPtr);
}

void left(robot* botPtr, grid* gridPtr) {
    updateMoves(botPtr, 'L');
    if (botPtr->dir == 'N') {
        botPtr->dir = 'W';
    } else if (botPtr->dir == 'E') {
        botPtr->dir = 'N';
    } else if (botPtr->dir == 'S') {
        botPtr->dir = 'E';
    } else { // robot facing west
        botPtr->dir = 'S';
    }
    drawRobot(botPtr, gridPtr);
}

void right(robot* botPtr, grid* gridPtr) {
    updateMoves(botPtr, 'R');
    if (botPtr->dir == 'N') {
        botPtr->dir = 'E';
    } else if (botPtr->dir == 'E') {
        botPtr->dir = 'S';
    } else if (botPtr->dir == 'S') {
        botPtr->dir = 'W';
    } else { // robot facing west
        botPtr->dir = 'N';
    }
    drawRobot(botPtr, gridPtr);
}

int atHome(robot* botPtr, grid* gridPtr) {
    if (gridPtr->tiles[botPtr->y][botPtr->x] == 3) {
        return 1;
    }
    return 0;
}

int atMarker(robot* botPtr, grid* gridPtr) {
    if (gridPtr->tiles[botPtr->y][botPtr->x] == 1) {
        return 1;
    }
    return 0;
}

int isCarryingAMarker(robot* botPtr) {
    return botPtr->hasMarker;
}

void pickUpMarker(robot* botPtr, grid* gridPtr) {
    if (atMarker(botPtr, gridPtr) && !isCarryingAMarker(botPtr)) {
        gridPtr->tiles[botPtr->y][botPtr->x] = 0;
        botPtr->hasMarker = 1;
    }
    drawGrid(gridPtr);
    drawRobot(botPtr, gridPtr);
}

void dropMarker(robot* botPtr, grid* gridPtr) {
    if (isCarryingAMarker(botPtr) && gridPtr->tiles[botPtr->y][botPtr->x] != 2) { // 2 is obstacle tile
        if (gridPtr->tiles[botPtr->y][botPtr->x] == 0) { // empty tile
            gridPtr->tiles[botPtr->y][botPtr->x] = 1;
            botPtr->hasMarker = 0;
        } else if (gridPtr->tiles[botPtr->y][botPtr->x] == 3) { // home tile
            botPtr->hasMarker = 0;
        } else { // marker tile
            // TO DO - allow multiple markers to be dropped on one tile as per coursework program specification
        }
    }
    drawGrid(gridPtr);
    drawRobot(botPtr, gridPtr);
}

void reverseMoves(robot* botPtr, grid* gridPtr) {
    // face opposite direction
    right(botPtr, gridPtr);
    right(botPtr, gridPtr);
    // remove additional reversing moves from prevMoves
    updateMoves(botPtr, '-');
    updateMoves(botPtr, '-');
    while (!atHome(botPtr, gridPtr) && botPtr->numMoves > 0) {
        char lastMove = botPtr->prevMoves[botPtr->numMoves-1];
        updateMoves(botPtr, '-'); // remove last robot move
        if (lastMove == 'F') {
            forward(botPtr, gridPtr);
        } else if (lastMove == 'L') {
            right(botPtr, gridPtr);
        } else { // lastMove is right
            left(botPtr, gridPtr);
        }
        updateMoves(botPtr, '-');
    }
    // clear remaining moves if home is reached before robot fully reverses
    while (botPtr->numMoves > 0) {
        updateMoves(botPtr, '-');
    }
}