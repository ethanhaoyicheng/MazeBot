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

void forward(Bot *bot){
    if (bot == NULL)
    return;
    
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
        }
    }   else{
        (*axis)--;
        if (!checkSpace(bot)){
            (*axis)++;
        }
    }
}

bool atMarker(Bot *bot){
    if (bot == NULL)
    return false;
    
    if (maze[bot->y][bot->x] == END_MARKER){
        return true;
    }   return false;
}

void drawMaze(void){

}

int main(void){
    Bot bot = {0,4,EAST};
    outMaze(&bot);
    return 0;
}

//compile:
//gcc -Wall -Wextra -Wpedantic -std=c11 main.c graphics.c -o maze

//run:
//./maze | java -jar drawapp-5.0.jar