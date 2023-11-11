#ifndef GRID_H
#define GRID_H

typedef struct grid {
    int height;
    int width;
    int tileSize;
    int offset;
    int markersLeft;
    int tiles[10][10];
} grid;

int getScreenX(int xCoordinate, grid* gridPtr);
int getScreenY(int yCoordinate, grid* gridPtr);
void setGrid(grid* gridPtr);
void setHome(int x, int y, grid* gridPtr);
void drawGrid(grid* gridPtr);

#endif // GRID_H