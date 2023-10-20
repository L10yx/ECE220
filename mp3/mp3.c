/*
author: yuxuan42
partners: sj57, zhiboz3

INTRO:
    A row in the Pascal' triangle contains all the coefficients of expanding the polynomial (1+x)^n
    where n corresponds to the index of row in Pascal's triangle.

    The program first asks the user to enter a row index,
    then it calculates all coefficients in that row,
    and use the standard function printf to print out the coefficients.
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
  int row;
  printf("Enter a row index: ");
  scanf("%d",&row);
  // Write your code here
  unsigned long coef = 1; // set usigned long coefficient to 1
  for (int col = 0; col <= row; col++) {
      printf("%lu ", coef); 
      coef = coef * (row - col) / (col + 1); // use equtions with pi
  }
  printf("\n"); // LF
  return 0;
}
