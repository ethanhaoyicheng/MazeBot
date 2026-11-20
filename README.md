MazeBot is a program which generates semi-random mazes using a DFS generation approach, to be solved by the titular mazebot- a simple right-hugging automaton. The generation and solving all happens automatically, with java drawapp handling the UI. Users can pause, rewind, and play the program.

To compile:
gcc -Wall -Wextra -Wpedantic -std=c11 main.c graphics.c -o maze

To run:
./maze | java -jar drawapp-5.0.jar