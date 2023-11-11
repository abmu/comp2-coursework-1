#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include "grid.h"

int getScreenX(int xCoordinate, grid* gridPtr) {
    return (xCoordinate * gridPtr->tileSize) + gridPtr->offset;
}

int getScreenY(int yCoordinate, grid* gridPtr) {
    return (yCoordinate * gridPtr->tileSize) + gridPtr->offset;
}

void setGrid(grid* gridPtr) {
    FILE* fp = fopen("./grid.txt","r");
    if (fp == NULL) {
        fprintf(stderr,"Couldn't find the file ./grid.txt\n");
        exit(2);
    }
    for (int y = 0; y < gridPtr->height; y ++) {
        for (int x = 0; x < gridPtr->width; x++) {
            // read an element from the file and place it into the 2d array
            fscanf(fp, "%d,", &gridPtr->tiles[y][x]);
        }
    }
    fclose(fp);
}

void setHome(int x, int y, grid* gridPtr) {
    if (gridPtr->tiles[y][x] != 0) {
        fprintf(stderr,"This tile is already occupied\n");
        exit(3);
    }
    gridPtr->tiles[y][x] = 3;
}

void drawGrid(grid* gridPtr) {
    background();
    clear();
    gridPtr->markersLeft = 0;
    int squareSize = gridPtr->tileSize;
    for (int y = 0; y < gridPtr->height; y++) {
        int posY = getScreenY(y, gridPtr);
        for (int x = 0; x < gridPtr->width; x++) {
            int posX = getScreenX(x, gridPtr);
            if (gridPtr->tiles[y][x] == 0) { // empty tile
                setColour(black);
                drawRect(posX, posY, squareSize, squareSize);
                continue;
            } else if (gridPtr->tiles[y][x] == 1) { // marker tile
                setColour(lightgray);
                gridPtr->markersLeft++;
            } else if (gridPtr->tiles[y][x] == 2) { // obstacle tile
                setColour(black);
            } else { // home tile
                setColour(blue);
            }
            fillRect(posX, posY, squareSize, squareSize);
        }
    }
}
