/* 
 * author: yuxuan42
 * partners: sj57, zhiboz3
 * INTRO:
 * In this MP, the game board will be represented as a 1-D array. 
 * The size of the array is Width*Height. In your functions the game board will be passed to you as pointer (for example int *game_board).
 * For each cell of the game board, a live cell is represented as 1, a dead cell is represented as 0. 
 * Each cell has eight surrounding neighbors. 
 * The game board is updated step by step (each step is a generation).
 * For each step, a live cell keeps alive if it has 2 or 3 live neighbors. It turns into a dead cell if it has more than 3, or less than 2, live neighbors.
 * A dead cell turns into a live cell only if it has exactly three live neighbors.
 * Notice that at step i, the status of a cell at step i+1 is determined only based on the status of itself and its surrounding neighbors at step i.
 * Three functions:
 *      countLiveNeighbor: Return the number of alive neighbors.
 *      updateBoard: Update the game board to the next step.
 *      aliveStable: Checks if the alive cells stay the same for next step
 */







/*
 * countLiveNeighbor
 * Inputs:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * row: the row of the cell that needs to count alive neighbors.
 * col: the col of the cell that needs to count alive neighbors.
 * Output:
 * return the number of alive neighbors. There are at most eight neighbors.
 * Pay attention for the edge and corner cells, they have less neighbors.
 */

int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){
    int NumLiveNeighbors = 0;
    int i = row;
    int j = col;

    for (i = row -1; i <= row + 1; i++){
        for (j = col -1; j <= col + 1; j++){                                        // 2 nest loops to traverse its neighbors
            if ((i < boardRowSize) && (i >= 0) && (j < boardColSize) && (j >= 0)){  // in size range
                if ((board[(i * boardColSize) + j] == 1) && !((i == row) && (j == col))){ // exclude self
                    NumLiveNeighbors = NumLiveNeighbors + 1;                        // increment live neighbors
                }
            }
        }
    }
    return NumLiveNeighbors;
}
/* 
 * updateBoard
 * Update the game board to the next step.
 * Input: 
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: board is updated with new values for next step.
 */
void updateBoard(int* board, int boardRowSize, int boardColSize) {
    int NewBoard[boardRowSize * boardColSize];
    int i = 0;
    int j = 0;
    int NumLivNei;

    for (i = 0; i < boardRowSize; i++){
        for (j = 0; j < boardColSize; j++){
            NumLivNei = countLiveNeighbor(board, boardRowSize, boardColSize, i, j);
            /* A live cell stays alive if it has 2 or 3 alive neighbors, otherwise the cell dies.*/ 
            if (board[(i * boardColSize) + j] == 1){
                if ((NumLivNei == 2) || (NumLivNei == 3)){
                    NewBoard[(i * boardColSize) + j] = 1;   /* A live cell is represented as 1, a dead cell is represented as 0.*/
                }
                else {
                    NewBoard[(i * boardColSize) + j] = 0;
                }
            }
            else{             /* A dead cell turns alive if it has exactly 3 live neighbors. */
                if (NumLivNei == 3){
                    NewBoard[(i * boardColSize) + j] = 1;
                }
                else {
                    NewBoard[(i * boardColSize) + j] = 0;
                }
            }
        }
    }

    for (i = 0; i < boardRowSize * boardColSize; i++){
        board[i] = NewBoard[i];
    }
}

/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with 
 * current step or there is no alive cells at all.
 * return 0 if the alive cells change for the next step.
 */ 
int aliveStable(int* board, int boardRowSize, int boardColSize){
    int NewB[boardRowSize * boardColSize];  // size = cols * rows
    int i;

    for (i = 0; i < (boardRowSize * boardColSize); i++){
        NewB[i] = board[i];                 // copy board one by one
    }

    updateBoard(NewB, boardRowSize, boardColSize); 

     for(i = 0; i < (boardColSize * boardRowSize); i++){
        if (NewB[i] != board[i]) {
            return 0; // if change
        }
     }
     
    return 1; // if not change
}

			
				
			

