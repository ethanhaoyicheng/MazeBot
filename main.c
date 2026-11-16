//bot able to traverse 2D maze
//main program

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "graphics.h"

//maze constants
#define PASSABLE 0
#define IMPASSABLE 1
#define END_MARKER 9

#define HEIGHT 20
#define WIDTH 20

//bot constants
#define START_X 0
#define START_Y 4

#define EAST  0
#define SOUTH 1
#define WEST  2
#define NORTH 3

//UI constants
#define WINDOW_WIDTH 780
#define WINDOW_HEIGHT 800

#define CELL_SIZE 35
#define LEFT 40
#define TOP 40

#define LONG_SLEEP 1000
#define SHORT_SLEEP 80

/*int maze[HEIGHT][WIDTH] = {
    {1,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,9},
    {1,0,1,0,1,1,0,1},
    {1,0,1,0,0,0,0,1},
    {0,0,1,0,1,1,0,1},
    {1,0,1,0,1,0,1,1},
    {1,0,1,1,0,0,0,1},
    {1,0,0,0,0,1,0,1},
    {1,1,1,1,1,1,1,1},
    
};*/

int maze[HEIGHT][WIDTH];

typedef struct {
    int x;
    int y;
    int direction;
} Bot;


void outMaze(Bot *bot)
//prints out the maze to terminal
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (bot != NULL && bot->x==x && bot->y==y){
                printf("B");
            }
            else if (maze[y][x] == END_MARKER)
                printf("!");
            else if (maze[y][x] == IMPASSABLE)
                printf("X");
            else
                printf("O");
        }
        printf("\n");
    }
}


void left(Bot *bot)
//turn bot left
{
    if (bot == NULL)
    return;
    bot->direction = (bot->direction + 3) % 4;
}

void right(Bot *bot)
//turn bot right
{
    if (bot == NULL)
    return;
    bot->direction = (bot->direction+1)%4;
}

bool checkSpace(Bot *bot)
//checks if space corresponding to bot position is within grid bounds and passable
//implemented instead of canMoveForward(); the latter could be implemented simply by including the axis increment temporarily
{
    if (bot == NULL)
        return false; 
    if (bot->x < 0 || bot->x >= WIDTH || bot->y < 0 || bot->y >= HEIGHT || maze[bot->y][bot->x] == IMPASSABLE){
        return false;
    }   return true;
}

bool forward(Bot *bot){
    if (bot == NULL)
    return false;
    
    int *axis;
    int bd = bot->direction;
    if (bd % 2 == 0){
        axis = &bot->x;
    } else {
        axis = &bot->y;
    }
    if (bd < 2){
        (*axis)++;
        if (!checkSpace(bot)){
            (*axis)--;
            return false;
        }
    }   else{
        (*axis)--;
        if (!checkSpace(bot)){
            (*axis)++;
            return false;
        }
    } return true;
}

bool atMarker(Bot *bot)
// checks if bot is at end marker
{
    if (bot == NULL)
    return false;
    
    if (maze[bot->y][bot->x] == END_MARKER){
        return true;
    }   return false;
}

void drawMaze(int maze[HEIGHT][WIDTH], Bot *bot){
    setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setLineWidth(2);
    setColour(black);
    for (int y = 0; y < HEIGHT; y++){
        for (int x = 0; x < WIDTH; x++){
            if (maze[y][x] == IMPASSABLE){
                fillRect(LEFT + x*CELL_SIZE, TOP + y*CELL_SIZE, CELL_SIZE, CELL_SIZE);
            } else if (maze[y][x] == END_MARKER){
                setColour(yellow);
                fillRect(LEFT + x*CELL_SIZE, TOP + y*CELL_SIZE, CELL_SIZE, CELL_SIZE);
                setColour(black);
            }
        }
    }
    if (bot != NULL){
        int bx = LEFT + bot->x*CELL_SIZE;
        int by = TOP + bot->y*CELL_SIZE;
        setColour(blue);
        fillOval(bx+5, by+5, CELL_SIZE-10, CELL_SIZE-10);

        setColour(black);
        int centerX = bx + CELL_SIZE/2;
        int centerY = by + CELL_SIZE/2;
        fillRect(centerX, centerY, 1,1);

        setColour(yellow);
        int lineLength = CELL_SIZE/2 - 5;
        int endX = centerX;
        int endY = centerY;
        switch(bot->direction){
            case EAST:
                endX += lineLength;
                break;
            case SOUTH:
                endY += lineLength;
                break;
            case WEST:
                endX -= lineLength;
                break;
            case NORTH:
                endY -= lineLength;
                break;
        
        drawLine(centerX, centerY, endX, endY);
    }
}

void discoverBlock(int maze[HEIGHT][WIDTH]){
    for (int y = 0; y < HEIGHT; y++){
        for (int x = 0; x < WIDTH; x++){
            maze[y][x] = IMPASSABLE;
        }
    }
    maze[1][WIDTH-2] = END_MARKER;
} 

/*void ellerGenerateMaze(int maze[HEIGHT][WIDTH]){

}*/

void dfsGenerateMaze(int maze[HEIGHT][WIDTH]){
    //initialize solid impassable maze block 
    discoverBlock(maze);
    //dfs maze generation algorithm
    int stack[HEIGHT*WIDTH];
    int top = 0;
    int visited[HEIGHT][WIDTH] = {0};
    stack[top++] = START_Y * WIDTH + START_X;
    visited[START_Y][START_X] = 1;
    
    while (top >= 0){
        int current = stack[--top];
        int x = current % WIDTH;
        int y = current / WIDTH;
        maze[y][x] = PASSABLE;
        
        // pick random direction for next visited space
        int directions[4] = {0, 1, 2, 3};
        for (int i = 0; i < 4; i++){
            int j = rand() % 4;
            int temp = directions[i];
            directions[i] = directions[j];
            directions[j] = temp;
        }
        
        for (int i = 0; i < 4; i++){
            int nx = x;
            int ny = y;
            switch(directions[i]){
                case EAST: nx += 2; break;
                case SOUTH: ny += 2; break; 
                case WEST: nx -= 2; break; 
                case NORTH: ny -= 2; break; 
            }
            if (nx > 0 && nx < WIDTH-1 && ny > 0 && ny < HEIGHT-1 && !visited[ny][nx]){
                visited[ny][nx] = 1; // prevents space from being carved to in future (no loops in maze)
                maze[(y+ny)/2][(x+nx)/2] = PASSABLE; // remove wall between cells
                stack[top++] = ny * WIDTH + nx;
            }
        }
    }
}


int main(void){
    while (1){
        //rest after and before each maze run
        sleep(LONG_SLEEP);
        dfsGenerateMaze(maze);

        Bot bot = {0,4,EAST};
        outMaze(&bot);
        drawMaze(maze, &bot);
        sleep(LONG_SLEEP);

        //short interval between each robot step; wall hugging is implemented by first attempting to move right, then forward, then left, then back
        while (!atMarker(&bot)){
            sleep(SHORT_SLEEP);
            right(&bot);
            if (!forward(&bot)){
                left(&bot);
                if (!forward(&bot)){
                    right(&bot);
                    if (!forward(&bot)){
                        left(&bot);
                        left(&bot);
                        if (!forward(&bot)){
                            left(&bot);
                        }
                    }
                }
            }  
            outMaze(&bot);
            drawMaze(maze, &bot);
        }
    }

    return 0; 
}

//compile:
//gcc -Wall -Wextra -Wpedantic -std=c11 main.c graphics.c -o maze

//run:
//./maze | java -jar drawapp-5.0.jar