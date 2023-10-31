#include "./graphics.h"
#include <stdio.h>

const int winSize = 800;
const int tileSize = 60;
const int gridOffset = 100;
const int gridHeight = 10;
const int gridWidth = 10;
const int grid[][10] = {
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0}
};

void drawGrid(void) {
    foreground();
    setColour(black);
    for (int i = 0; i < gridHeight; i++) {
        int y = (i * tileSize) + gridOffset;
        for (int j = 0; j < gridWidth; j++) {
            int x = (j * tileSize) + gridOffset;
            if (grid[i][j] == 0) {
                drawRect(x,y,tileSize,tileSize);
            }
        }
    }
}

int main(void) {
    setWindowSize(winSize,winSize);
    drawGrid();
    return 0;
}