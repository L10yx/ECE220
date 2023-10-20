/* 
author: yuxuan42
partners: sj57, zhiboz3

INTRO:

Bug in is_prime:
In is_prime function, the error is return 1; 
Used to indicate that a number is a prime number, 
but it should actually be a return 0;. 
Because if there is a factor, it should return 1, 
indicating that it is not a prime number. 
So after gdb, the value of return was modified.

bug1: In is_prime, the value returned is revered so that
    result to decide if input is prime is totally wrong.
    In for loop, if number is multiple of i(number % i == 0)
    it is not a prime and should return 0, 
    else when loop finish, return 1 to indicate it's prime;      


Bug in print_semiprimes
The correct logic for loops in the semiprimes function should be the loop for (i=a; i<=b; i++) to traverse all integers within the given range [a, b]. 
In the internal loop for (j=2; j<=i; j++), it attempts to find all factors of i and only continues when j is a prime number. 
If a prime number j is found, calculate k=i/j, which is the result of dividing i by the prime number j. Next, check if k is also a prime number. 
If it is a prime number, print i as a semi prime number and set ret to 1, indicating that a semi prime number has been found. 
Finally, use the break statement to exit the internal loop without looking for more factors.
 
bug2: In print_semiprimes 'k = i%j' is wrong for its '%'.
    The aim of third loop is under the condition of j is prime,
    find another factor k is also prime to support i as semiprime.
    So change '%' to '/' that k correctly represent factor of i.
bug3: Notice that the program print duplicate number. 
    Correct values need to be printed once and only once.
    Because after found semiprime, we had not jump directly to next i. 
    To solve that, add 'break;' after printf and it iterates i immediately.
bug4: print_semiprimes returns ret, 0 if there is no semiprime in [a,b], else 1.
    The original one only return 0. So add 'ret = 1' in third loop before break.
    As a result, the return value update correctly.
*/


#include <stdlib.h>
#include <stdio.h>

/*
 * is_prime: determines whether the provided number is prime or not
 * Input    : a number
 * Return   : 0 if the number is not prime, else 1
 */
int is_prime(int number)
{
    int i;
    if (number == 1) {return 0;}
    for (i = 2; i < number; i++) { //for each number smaller than it
        if (number % i == 0) { //check if the remainder is 0
            return 0;
        }
    }
    return 1;
}


/*
 * print_semiprimes: prints all semiprimes in [a,b] (including a, b).
 * Input   : a, b (a should be smaller than or equal to b)
 * Return  : 0 if there is no semiprime in [a,b], else 1
 */
int print_semiprimes(int a, int b)
{
    int i, j, k;
    int ret = 0;
    for (i = a; i <=b; i++) { //for each item in interval
        //check if semiprime
        for (j = 2; j <= i; j++) {
            if (i%j == 0) {
                if (is_prime(j)) {
                    k = i/j;
                    if (is_prime(k)) {
                        printf("%d ", i);
                        ret = 1;
                        break;
                    }
                }
            }
        }

    }
    printf("\n");
    return ret;

}
