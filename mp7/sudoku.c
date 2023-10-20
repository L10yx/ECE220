//author: yuxuan42
//partners: zhiboz3, sj57  

/*I have implemented a C program to solve Sudoku puzzles.
Sudoku is a popular number puzzle game that aims to fill a 9x9 grid,
so that each row, column, and nine 3x3 subgrids (also known as "regions") 
contain numbers from 1 to 9 without repetition. This program includes a series of functions to check 
whether a given value can be placed in a specific cell of a Sudoku grid without violating rules, 
solve Sudoku puzzles recursively, and verify the existence of a solution. 
These functions work together to solve Sudoku puzzles by repeatedly trying to fill empty cells with 
valid numbers until a complete solution is found or it is determined that no solution exists.

Is_ Val_ In_ Row function:
This function is used to check if the value val already exists in the i-th line of Sudoku. 
The function checks for the existence of the same value by traversing each cell in the row. 
If found, it returns 1 to indicate existence, otherwise it returns 0 to indicate non existence.

Is_ Val_ In_ Col function:
This function is used to check if the value val already exists in the jth column of Sudoku. 
The function checks for the existence of the same value by traversing each cell of the column. 
If found, it returns 1 to indicate existence, otherwise it returns 0 to indicate non existence.

Is_ Val_ In_ 3x3_ Zone function:
This function is used to check if the value val already exists in the 3x3 region of Sudoku, 
which is specified by (i, j). The function calculates the starting coordinates of the specified area, 
and then traverses each cell in the area to check for the existence of the same value. 
If found, it returns 1 to indicate existence, otherwise it returns 0 to indicate non existence.

Is_ Val_ Valid function:
This function is used to check whether the value val can be filled in the given position (i, j), 
that is, to check whether the Sudoku rule is met. 
Function call is_ Val_ In_ Row, is_ Val_ In_ Col and is_ Val_ In_ 3x3_ 
The zone function checks whether the corresponding rows, columns, and regions already contain the same value. 
If any check returns 1, indicating a conflict, and the function returns 0, 
indicating that it cannot be filled in; Otherwise, it returns 1, indicating that it can be filled in.
*/


#include "sudoku.h"

//-------------------------------------------------------------------------------------------------
// Start here to work on your MP7
//-------------------------------------------------------------------------------------------------

// You are free to declare any private functions if needed.

// Function: is_val_in_row
// Return true if "val" already existed in the ith row of the sudoku array.
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {

  assert(i >= 0 && i < 9);

  // BEGIN TODO: Check if 'val' is already present in the ith row.
  for (int j = 0; j < 9; j++) {
    if (sudoku[i][j] == val) {
      return 1; // 'val' exists in this row, return true (1).
    }
  }
  return 0; // 'val' does not exist in this row, return false (0).
  // END TODO
}

// Function: is_val_in_col
// Return true if "val" already exists in the jth column of the sudoku array.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {

  assert(j >= 0 && j < 9);

  // BEGIN TODO: Check if 'val' is already present in the jth column.
  for (int i = 0; i < 9; i++) {
    if (sudoku[i][j] == val) {
      return 1; // 'val' exists in this column, return true (1).
    }
  }
  return 0; // 'val' does not exist in this column, return false (0).
  // END TODO
}

// Function: is_val_in_3x3_zone
// Return true if val already exists in the 3x3 zone corresponding to (i, j).
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i >= 0 && i < 9);

  // BEGIN TODO: Check if 'val' is already present in the 3x3 zone corresponding to (i, j).
  int i_start, j_start;
  i_start = i - (i % 3);
  j_start = j - (j % 3);
  for (int row = i_start; row < (i_start + 3); row++) {
    for (int col = j_start; col < (j_start + 3); col++) {
      if (sudoku[row][col] == val) {
        return 1; // 'val' exists in this 3x3 zone, return true (1).
      }
    }
  }
  return 0; // 'val' does not exist in this 3x3 zone, return false (0).
  // END TODO
}

// Function: is_val_valid
// Return true if the val can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i >= 0 && i < 9 && j >= 0 && j < 9);

  // BEGIN TODO: Check if 'val' can be filled in the given entry without violating Sudoku rules.
  if ((is_val_in_3x3_zone(val, i, j, sudoku)) || (is_val_in_col(val, j, sudoku)) || (is_val_in_row(val, i, sudoku))) {
    return 0; // 'val' cannot be filled in this entry, return false (0).
  }
  return 1; // 'val' can be filled in this entry, return true (1).
  // END TODO
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) {

  // BEGIN TODO: Implement the Sudoku solving logic using a recursive approach.
  int r, c;
  int flag = 1;
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (sudoku[i][j] == 0) {
        r = i;
        c = j;
        flag = 0; // Set flag to 0 to indicate an empty cell was found.
        continue;
      }
      if (flag == 0) {
        continue; // Skip further processing if an empty cell has already been found.
      }
    }
  }
  if (flag == 1) {
    return 1; // If no empty cells are left, the Sudoku is solved successfully, return true (1).
  }

  for (int num = 1; num <= 9; num++) {
    if (is_val_valid(num, r, c, sudoku) == 1) {
      sudoku[r][c] = num; // Try filling the empty cell with 'num'.
      if (solve_sudoku(sudoku)) {
        return 1; // Recursively attempt to solve the Sudoku.
      }
      sudoku[r][c] = 0; // If unsuccessful, backtrack and reset the cell to 0.
    }
  }
  return 0; // No solution found, return false (0).
  // END TODO.
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9]) {
  int i, j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}


