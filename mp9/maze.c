//author: yuxuan42
//partners: sj57, zhiboz3
/*
DESCRIPTION:
Given the structure containing the maze and a current row and column recursively solve the maze from this position using the DFS algorithm. 
This function should be the recursive function, but if you feel it is necessary you can write a recursive helper function that's called by this. 
This function should change the contents of the 2D array (cells) by indicating which cells are along the solution path and which cells were visited during the search. 
If the maze is unsolvable this function should return 0 otherwise it should return 1. You should NOT overwrite spaces that contain walls, the start, or the end.
ACTIONS:
Implement 
maze_t * createMaze(char * fileName);
void destroyMaze(maze_t * maze);
int solveMazeDFS(maze_t * maze, int col, int row);
void printMaze(maze_t * maze);
*/


#include <stdio.h>
#include <stdlib.h>
#include "maze.h"


/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None 
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
    // Your code here. Make sure to replace following line with your own code.
    FILE *file;
    file = fopen(fileName, "r"); 
    int width, height;
    fscanf(file, "%d %d", &width, &height); // Read width and height

    int i, j;
    char grid; // Store element in every grid
    maze_t *maze = (maze_t*)malloc(sizeof(maze_t));
    maze->cells = (char**)malloc(height * sizeof(char*)); // malloc arrays of rows
    for (i = 0; i < height; i++){
       maze->cells[i] = (char *)malloc(width * sizeof(char)); // malloc arrays of each column
    }
    maze->width = width;
    maze->height = height; // Store w and h

    for (i = 0; i < height; i++){
        for (j = 0; j < width; j++){
            grid = fgetc(file);
            if (grid != '\n'){
                maze->cells[i][j] = grid;
            } else{
                j--; // ignore new line
            }
            if (j >= 0){
                if (grid == 'S'){ // Start
                    maze->startRow = i;
                    maze->startColumn = j;
                }
                if (grid == 'E'){ // Ending
                    maze->endRow = i;
                    maze->endColumn = j;
                }
            }
        }
    }

    fclose(file); // Close file
    return maze;
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information 
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    // Your code here.
    for (int i = 0; i < maze->height; i++){
        free(maze->cells[i]); // free cols
    }
    free(maze->cells); // free rows
    free(maze);
    return;
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information 
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
    // Your code here.
    for (int i = 0; i < maze->height; i++){
        for (int j = 0; j < maze->width; j++){
            printf("%c", maze->cells[i][j]);
        }
        printf("\n");
    }
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */ 
int solveMazeDFS(maze_t * maze, int col, int row)
{
    // Your code here. Make sure to replace following line with your own code.
    if (maze->cells[row][col] == END)
        return 1; // finish

    if (maze->cells[row][col] != EMPTY && maze->cells[row][col] != START)
        return 0; // continue

    if (maze->cells[row][col] == EMPTY)
        maze->cells[row][col] = PATH; 

    if (row < maze->height-1 && solveMazeDFS(maze, col, row + 1)) // go down first
        return 1; 
    if (col > 0 && solveMazeDFS(maze, col - 1, row)) // at bottom, go left
        return 1;
    if (col < maze->width - 1 && solveMazeDFS(maze, col + 1, row)) // else go right
        return 1;
    if (row > 0 && solveMazeDFS(maze, col, row - 1)) // go up
        return 1;
        
    if (maze->cells[row][col] == '*') // go back
        maze->cells[row][col] = '~'; 

    return 0;
}