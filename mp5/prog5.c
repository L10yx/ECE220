// author: yuxuan42
// partners: sj57, zhiboz3

/*			
 *
 * prog5.c - source file adapted from UIUC ECE198KL Spring 2013 Program 4
 *           student code -- GOLD VERSION by Steven S. Lumetta
 */



/*
INTRO：
To play the game, the user first enters a "seed" value, which is an integer. This seed value initializes a pseudo-random number generator. 
Using the pseudo-random number generator, a solution code sequence of four numbers from 1 to 8 is chosen at "random." This solution is kept hidden from the player.
The player then has 12 guesses to get the sequence of four numbers correct. Guesses are typed into the terminal. 
After each guess, the player is given feedback on the number of perfect matches and misplaced matches, which is printed to the screen.
The values of the guess that appear in the same place in the solution code are called perfect matches. 
The number of values that appear somewhere in the solution code but in a different place in the solution code are called misplaced matches. 
If the player manages to guess the correct sequence in twelve or fewer guesses, they win the game. Otherwise, they lose.
Three functions:
set_seed -- This function sets the seed value for pseudorandom
start_game -- This function is called by main.c after set_seed but before the user makes guesses.
make_guess -- This function is called by main.c after the user types in a guess.
*/






/*
 * The functions that you must write are defined in the header file.
 * Blank function prototypes with explanatory headers are provided
 * in this file to help you get started.
 */



#include <stdio.h>
#include <stdlib.h>

#include "prog5.h"


/*
 * You will need to keep track of the solution code using file scope
 * variables as well as the guess number.
 */

static int guess_number;
static int solution1;
static int solution2;
static int solution3;
static int solution4;


/*
 * set_seed -- This function sets the seed value for pseudorandom
 * number generation based on the number the user types.
 * The input entered by the user is already stored in the string seed_str by the code in main.c.
 * This function should use the function sscanf to find the integer seed value from the 
 * string seed_str, then initialize the random number generator by calling srand with the integer
 * seed value. To be valid, exactly one integer must be entered by the user, anything else is invalid. 
 * INPUTS: seed_str -- a string (array of characters) entered by the user, containing the random seed
 * OUTPUTS: none
 * RETURN VALUE: 0 if the given string is invalid (string contains anything
 *               other than a single integer), or 1 if string is valid (contains one integer)
 * SIDE EFFECTS: initializes pseudo-random number generation using the function srand. Prints "set_seed: invalid seed\n"
 *               if string is invalid. Prints nothing if it is valid.
 */
int
set_seed (const char seed_str[])
{
//    Example of how to use sscanf to read a single integer and check for anything other than the integer
//    "int seed" will contain the number typed by the user (if any) and the string "post" will contain anything after the integer
//    The user should enter only an integer, and nothing else, so we will check that only "seed" is read. 
//    We declare
    int seed;
    char post[2];
//    The sscanf statement below reads the integer into seed. 
//    sscanf (seed_str, "%d%1s", &seed, post)
//    If there is no integer, seed will not be set. If something else is read after the integer, it will go into "post".
//    The return value of sscanf indicates the number of items read succesfully.
//    When the user has typed in only an integer, only 1 item should be read sucessfully. 
//    Check that the return value is 1 to ensure the user enters only an integer. 
//    Feel free to uncomment these statements, modify them, or delete these comments as necessary. 
//    You may need to change the return statement below

    if (sscanf(seed_str, "%d%1s", &seed, post) == 1) {
        srand((unsigned int)seed); // set the rand func
        return 1;    // valid seed
    } 
    else { 
        printf("set_seed: Invalid seed\n");
        return 0;    // invalid seed
    }
}


/*
 * start_game -- This function is called by main.c after set_seed but before the user makes guesses.
 *               This function creates the four solution numbers using the approach
 *               described in the wiki specification (using rand())
 *               The four solution numbers should be stored in the static variables defined above. 
 *               The values at the pointers should also be set to the solution numbers.
 *               The guess_number should be initialized to 1 (to indicate the first guess)
 * INPUTS: none
 * OUTPUTS: *one -- the first solution value (between 1 and 8)
 *          *two -- the second solution value (between 1 and 8)
 *          *three -- the third solution value (between 1 and 8)
 *          *four -- the fourth solution value (between 1 and 8)
 * RETURN VALUE: none
 * SIDE EFFECTS: records the solution in the static solution variables for use by make_guess, set guess_number
 */
void
start_game (int* one, int* two, int* three, int* four)
{
    // Step 1: Starting with the first value in the solution code sequence, generate a random integer in the range 0 to 7 inclusive using a single call to rand() and a single modulus (%) operator
    // Step 2: Add 1 to the resulting number to get a value in the range 1 to 8.
    solution1 = rand() % 8 + 1;
    solution2 = rand() % 8 + 1;
    solution3 = rand() % 8 + 1;
    solution4 = rand() % 8 + 1;
    // set the static variable guess_number to 1. 
    // You must also make your own copy of the solution code using static variables solution1, solution2, solution3, and solution4. 
    // These variables will be necessary when you implement make_guess. Set the values at one, two, three and four.
    guess_number = 1;   
    *one = solution1;
    *two = solution2;
    *three = solution3;
    *four = solution4;
    return;
}

/*
 * make_guess -- This function is called by main.c after the user types in a guess.
 *               The guess is stored in the string guess_str. 
 *               The function must calculate the number of perfect and misplaced matches
 *               for a guess, given the solution recorded earlier by start_game
 *               The guess must be valid (contain only 4 integers, within the range 1-8). If it is valid
 *               the number of correct and incorrect matches should be printed, using the following format
 *               "With guess %d, you got %d perfect matches and %d misplaced matches.\n"
 *               If valid, the guess_number should be incremented.
 *               If invalid, the error message "make_guess: invalid guess\n" should be printed and 0 returned.
 *               For an invalid guess, the guess_number is not incremented.
 * INPUTS: guess_str -- a string consisting of the guess typed by the user
 * OUTPUTS: the following are only valid if the function returns 1 (A valid guess)
 *          *one -- the first guess value (between 1 and 8)
 *          *two -- the second guess value (between 1 and 8)
 *          *three -- the third guess value (between 1 and 8)
 *          *four -- the fourth color value (between 1 and 8)
 * RETURN VALUE: 1 if the guess string is valid (the guess contains exactly four
 *               numbers between 1 and 8), or 0 if it is invalid
 * SIDE EFFECTS: prints (using printf) the number of matches found and increments guess_number(valid guess) 
 *               or an error message (invalid guess)
 *               (NOTE: the output format MUST MATCH EXACTLY, check the wiki writeup)
 */
int
make_guess (const char guess_str[], int* one, int* two, 
	    int* three, int* four)
{
//  One thing you will need to read four integers from the string guess_str, using a process
//  similar to set_seed
//  The statement, given char post[2]; and four integers w,x,y,z,
//  sscanf (guess_str, "%d%d%d%d%1s", &w, &x, &y, &z, post)
//  will read four integers from guess_str into the integers and read anything else present into post
//  The return value of sscanf indicates the number of items sucessfully read from the string.
//  You should check that exactly four integers were sucessfully read.
//  You should then check if the 4 integers are between 1-8. If so, it is a valid guess
//  Otherwise, it is invalid.  
//  Feel free to use this sscanf statement, delete these comments, and modify the return statement as needed
    int w, x, y, z;
    char post[2];
    int fw = 1, fx = 1, fy = 1, fz = 1; // flag set for 4 solutions
    if (sscanf(guess_str, "%d%d%d%d%1s", &w, &x, &y, &z, post) == 4) { // valid
        if (w >= 1 && w <= 8 && x >= 1 && x <= 8 && y >= 1 && y <= 8 && z >= 1 && z <= 8) { // in range
            int perfect_matches = 0;
            int misplaced_matches = 0;
            // check perfect matches
            if (w == solution1) {
                perfect_matches++; 
                fw = 0;
            } 
            if (x == solution2) {
                perfect_matches++; 
                fx = 0;
            } 
            if (y == solution3) {
                perfect_matches++; 
                fy = 0;
            } 
            if (z == solution4) {
                perfect_matches++; 
                fz = 0;
            } 
            // check misplaced matches, from first to fourth
            if (w != solution1) { // check mismatch solutions
                if (w == solution2 * fx) {
                    misplaced_matches++; 
                    fx = 0;} 
                else if (w == solution3 * fy) {
                    misplaced_matches++; 
                    fy = 0;} 
                else if (w == solution4 * fz) {
                    misplaced_matches++; 
                    fz = 0;} 
            }
            if (x != solution2) { // similar
                if (x == solution1 * fw) {
                    misplaced_matches++; 
                    fw = 0;} 
                else if (x == solution3 * fy) {
                    misplaced_matches++; 
                    fy = 0;} 
                else if (x == solution4 * fz) {
                    misplaced_matches++; 
                    fz = 0;} 
            }
            if (y != solution3) { // similar
                if (y == solution1 * fw) {
                    misplaced_matches++; 
                    fw = 0;} 
                else if (y == solution2 * fx) {
                    misplaced_matches++; 
                    fx = 0;} 
                else if (y == solution4 * fz) {
                    misplaced_matches++; 
                    fz = 0;} 
            }
            if (z != solution4) { //similar
                if (z == solution1 * fw) {
                    misplaced_matches++; 
                    fw = 0;} 
                else if (z == solution2 * fx) {
                    misplaced_matches++; 
                    fx = 0;} 
                else if (z == solution3 * fy) {
                    misplaced_matches++; 
                    fy = 0;} 
            }
            // finish counting
            printf("With guess %d, you got %d perfect matches and %d misplaced matches.\n", guess_number, perfect_matches, misplaced_matches);
            *one = w;
            *two = x;
            *three = y;
            *four = z;        // store outputs
            guess_number++;  // guess_number increment
            return 1;        // valid guess
        } 
        else {
            printf("make_guess: Invalid guess\n");
            return 0;  // beyond range
        }
    } 
    else {
        printf("make_guess: Invalid guess\n");
        return 0;  // integers not enough, invalid
    } 
}


