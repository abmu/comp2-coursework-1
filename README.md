# COMP0002 Coursework 1

The code provided here completes all six stages of the coursework. The objective was to show a robot which traverses a grid containing obstacles until it finds a marker, which is then returned to the robot's home.

## Running the Program

To run the program, follow these steps:

1. Ensure you have the `graphics.c`, `graphics.h`, and `drawapp-3.0.jar` within the coursework files directory.
2. Compile the source code using the following commands:
   ```bash
   gcc robot_search.c graphics.c
   ```
3. Execute the compiled program with the following command:
   ```bash
   ./a.out (0-9) (0-9) (N E S W) | java -jar "drawapp-3.0.jar"
   ```
   Replace (0-9) with the X and Y positions on the grid, and (N E S W) with the initial direction (however due to the nature of the algorithm this does not matter much). If the chosen grid position is already occupied, the program will not run, so select a different location if needed.

The grid can be adjusted by editing the 'grid.txt' file.

## Known issues

There are some obstacle positions which may potentially cause the algorithm to fail to find the marker. These are listed in more detail within the `algorithmStage6` procedure in `robot_search.c`.