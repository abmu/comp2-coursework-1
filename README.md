# COMP0002 Coursework 1

The code provided here completes all six stages of the coursework. The objective was to display a robot traversing a grid containing obstacles until it finds a marker(s), at which point it picks it up and returns it home.

## Running the program

To run the program, follow these steps:

1. Ensure you have the `graphics.c`, `graphics.h`, and `drawapp-3.0.jar` files within the same directory as the coursework files.
2. Compile the source code using the following command:
   ```bash
   gcc robot_search.c graphics.c
   ```
3. Execute the compiled program with the following command:
   ```bash
   ./a.out (0-9) (0-9) (N E S W) | java -jar "drawapp-3.0.jar"
   ```
   In this command, replace (0-9) (0-9) with an X and Y position on the grid, and (N E S W) with a cardinal direction (due to the nature of the algorithm this initial direction won't really make a difference).

   For example:
   ```bash
   ./a.out 3 5 N | java -jar "drawapp-3.0.jar"
   ```

If the chosen grid position is already occupied, the program will not run, so select a different location if needed. The grid can also be adjusted by editing the `grid.txt` file.

## Known issues

There are some obstacle positions which may potentially cause the robot to fail to find all of the markers. These are listed in more detail within the `algorithmStage6` procedure in `robot_search.c`.