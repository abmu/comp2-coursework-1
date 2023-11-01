#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "./graphics.h"

const int winSize = 800;
const int tileSize = 60;
const int gridOffset = 100;
const int gridHeight = 10;
const int gridWidth = 10;
int grid[10][10];
const int waitTime = 25;

void drawRobot(int topleftX, int topleftY) {
    foreground();
    setColour(green);
    int triangleX[3] = {topleftX + 0.5*tileSize, topleftX + tileSize, topleftX};
    int triangleY[3] = {topleftY, topleftY + tileSize, topleftY + tileSize};
    fillPolygon(3, triangleX, triangleY);
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
    for (int i = 0; i < gridHeight; i++) {
        int y = (i * tileSize) + gridOffset;
        for (int j = 0; j < gridWidth; j++) {
            int x = (j * tileSize) + gridOffset;
            if (grid[i][j] == 0) {
                setColour(black);
                drawRect(x,y,tileSize,tileSize);
            } else if (grid[i][j] == 1) {
                setColour(blue);
                fillRect(x,y,tileSize,tileSize);
            } else if (grid[i][j] == 2) {
                setColour(red);
                fillRect(x,y,tileSize,tileSize);
            }
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr,"Invalid number of arguments\n");
        return 1;
    }

    setWindowSize(winSize,winSize);
    drawGrid();
    return 0;
}