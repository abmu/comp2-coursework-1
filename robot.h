#ifndef ROBOT_H
#define ROBOT_H

// must include grid.h before robot.h in source (.c) files

typedef struct robot {
    int x;
    int y;
    char dir;
    int hasMarker;
    int numMoves;
    char* prevMoves;
} robot;

void drawRobot(robot* botPtr, grid* gridPtr);
int canMoveForward(robot* botPtr, grid* gridPtr);
int isObstacle(robot* botPtr, grid* gridPtr);
void forward(robot* botPtr, grid* gridPtr);
void left(robot* botPtr, grid* gridPtr);
void right(robot* botPtr, grid* gridPtr);
int atMarker(robot* botPtr, grid* gridPtr);
void pickUpMarker(robot* botPtr, grid* gridPtr);
void dropMarker(robot* botPtr, grid* gridPtr);
void reverseMoves(robot* botPtr, grid* gridPtr);

#endif // ROBOT_H