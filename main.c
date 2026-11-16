//bot able to traverse 2D maze
//main program

#include <stdio.h>
#include <stdbool.h>
#include "graphics.h"

//maze constants
#define PASSABLE 0
#define IMPASSABLE 1
#define END_MARKER 9

#define HEIGHT 9
#define WIDTH 8

//bot constants
#define EAST  0
#define SOUTH 1
#define WEST  2
#define NORTH 3

//drawing constants
#define CELL_SIZE 45
#define LEFT 60
#define TOP 60

int maze[HEIGHT][WIDTH] = {
    {1,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,9},
    {1,0,1,0,1,1,0,1},
    {1,0,1,0,0,0,0,1},
    {0,0,1,0,1,1,0,1},
    {1,0,1,0,1,0,1,1},
    {1,0,1,1,0,0,0,1},
    {1,0,0,0,0,1,0,1},
    {1,1,1,1,1,1,1,1},
};

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
{
    if (bot == NULL)
    return;
    bot->direction = (bot->direction + 3) % 4;
}

void right(Bot *bot){
    if (bot == NULL)
    return;
    bot->direction = (bot->direction+1)%4;
}

bool checkSpace(Bot *bot){
//implemented instead of canMoveForward(); the latter could be implemented simply by including the axis increment temporarily
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

bool atMarker(Bot *bot){
    if (bot == NULL)
    return false;
    
    if (maze[bot->y][bot->x] == END_MARKER){
        return true;
    }   return false;
}

void drawMaze(int maze[HEIGHT][WIDTH], Bot *bot){
    setWindowSize(600,600);
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
        }
        drawLine(centerX, centerY, endX, endY);
    }
}

int main(void){
    Bot bot = {0,4,EAST};
    outMaze(&bot);
    drawMaze(maze, &bot);

    while (!atMarker(&bot)){
        sleep(300);
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

    return 0; 
}

//compile:
//gcc -Wall -Wextra -Wpedantic -std=c11 main.c graphics.c -o maze

//run:
//./maze | java -jar drawapp-5.0.jar