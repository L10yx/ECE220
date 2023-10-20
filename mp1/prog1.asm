; ece220/fa23/mp1
; Yuxuan Lin
; yuxuan42@illinois.edu
;
; partners: 
; Sicheng Jiang
; sj57@illinois.edu
; Zhibo Zhang
; zhiboz3@illinois.edu
;
; This program extends the code that we started to develop in class 
; to compute a histogram of letters and non-letters in a string. 
; The program will print the resulting histogram in hexadecimal to the monitor. 


; The code given to you here implements the histogram calculation that 
; we developed in class.  In programming lab, we will add code that
; prints a number in hexadecimal to the monitor.
;
; Your assignment for this program is to combine these two pieces of 
; code to print the histogram to the monitor.
;
; If you finish your program, 
;    ** commit a working version to your repository  **
;    ** (and make a note of the repository version)! **


	.ORIG	x3000		; starting address is x3000


;
; Count the occurrences of each letter (A to Z) in an ASCII string 
; terminated by a NUL character.  Lower case and upper case should 
; be counted together, and a count also kept of all non-alphabetic 
; characters (not counting the terminal NUL).
;
; The string starts at x4000.
;
; The resulting histogram (which will NOT be initialized in advance) 
; should be stored starting at x3F00, with the non-alphabetic count 
; at x3F00, and the count for each letter in x3F01 (A) through x3F1A (Z).
;
; table of register use in this part of the code
;    R0 holds a pointer to the histogram (x3F00)
;    R1 holds a pointer to the current position in the string
;       and as the loop count during histogram initialization
;    R2 holds the current character being counted
;       and is also used to point to the histogram entry
;    R3 holds the additive inverse of ASCII '@' (xFFC0)
;    R4 holds the difference between ASCII '@' and 'Z' (xFFE6)
;    R5 holds the difference between ASCII '@' and '`' (xFFE0)
;    R6 is used as a temporary register
;

	LD R0,HIST_ADDR      	; point R0 to the start of the histogram
	
	; fill the histogram with zeroes 
	AND R6,R6,#0		; put a zero into R6
	LD R1,NUM_BINS		; initialize loop count to 27
	ADD R2,R0,#0		; copy start of histogram into R2

	; loop to fill histogram starts here
HFLOOP	STR R6,R2,#0		; write a zero into histogram
	ADD R2,R2,#1		; point to next histogram entry
	ADD R1,R1,#-1		; decrement loop count
	BRp HFLOOP		; continue until loop count reaches zero

	; initialize R1, R3, R4, and R5 from memory
	LD R3,NEG_AT		; set R3 to additive inverse of ASCII '@',-64
	LD R4,AT_MIN_Z		; set R4 to difference between ASCII '@' and 'Z',-26
	LD R5,AT_MIN_BQ		; set R5 to difference between ASCII '@' and '`'
	LD R1,STR_START		; point R1 to start of string

	; the counting loop starts here
COUNTLOOP
	LDR R2,R1,#0		; read the next character from the string
	BRz PRINT_HIST		; found the end of the string

	ADD R2,R2,R3		; subtract '@' from the character
	BRp AT_LEAST_A		; branch if > '@', i.e., >= 'A'
NON_ALPHA
	LDR R6,R0,#0		; load the non-alpha count
	ADD R6,R6,#1		; add one to it
	STR R6,R0,#0		; store the new non-alpha count
	BRnzp GET_NEXT		; branch to end of conditional structure
AT_LEAST_A
	ADD R6,R2,R4		; compare with 'Z'
	BRp MORE_THAN_Z         ; branch if > 'Z'

; note that we no longer need the current character
; so we can reuse R2 for the pointer to the correct
; histogram entry for incrementing
ALPHA	ADD R2,R2,R0		; point to correct histogram entry
	LDR R6,R2,#0		; load the count
	ADD R6,R6,#1		; add one to it
	STR R6,R2,#0		; store the new count
	BRnzp GET_NEXT		; branch to end of conditional structure

; subtracting as below yields the original character minus '`'
MORE_THAN_Z
	ADD R2,R2,R5		; subtract '`' - '@' from the character
	BRnz NON_ALPHA		; if <= '`', i.e., < 'a', go increment non-alpha
	ADD R6,R2,R4		; compare with 'z'
	BRnz ALPHA		; if <= 'z', go increment alpha count
	BRnzp NON_ALPHA		; otherwise, go increment non-alpha

GET_NEXT
	ADD R1,R1,#1		; point to next character in string
	BRnzp COUNTLOOP		; go to start of counting loop

;Intro: This code uses a loop to process each 4-bit half byte of the input number. 
;It shifts the half byte to the leftmost position and checks if the highest bit is set. 
;If the highest bit is set, add 1 to the half byte and then shift again. 
;This process is repeated until all four bits are processed. Output ASCII characters to the console.
;Then loop through the next half byte until all 16 bits are processed.
;
;Table
;R0: Used to store the value of the ASCII character for output to the console.
;R1: Used to store the value of the current nibble being processed.
;R2: Used to store the value of the current bit being processed.
;R3: Used to store the value of either the ASCII character ‘0’ or the ASCII character ‘A’.
;R4: Used to count the number of bits remaining in the current nibble being processed.
;R5: Used to count the total number of bits that have been processed.

HEXA
	AND R5, R5, #0
	ADD R5,R5,#4	; set R5 as counter for 16 bits
HEXALOOP	
	AND R1,R1,#0	; clear R1
	ADD R4,R1,#4	; set R4 as counter for 4 bits
FOURLOOP
	ADD R1,R1,R1	; left shift R1
	ADD R2,R2,#0	; 
	BRzp SHIFT		; if the highest bit is 0, directly go to SHIFT
	ADD R1,R1,#1	; else highest bit is 1, R1 plus 1
SHIFT 
	ADD R2,R2,R2	; left shift R2 for second highest bit
	ADD R4,R4,#-1	; counter decrement
	BRp FOURLOOP	; if not 4 bits already, go to loop again
	ADD R4,R4,#4	; set R4 as counter
	ADD R1,R1,#-10	; examine if R1 > 9
	BRzp TEN		; if R1 >= 10, go to TEN
	ADD R1,R1,#10	; R1 recover
	LD 	R6,ASC_ZERO	; else load R3 with ASCII of '0'
	ADD R0,R1,R6	; get ASCII
	OUT				; 
	ADD R5,R5,#-1	; counter decrement
	BRp HEXALOOP	; manage next 4 bits
	BR	NEWLINE     ;
TEN
	LD 	R6,ASC_A	; else load R3 with ASCII of 'A'
	ADD R0,R1,R6	; get ASCII
	OUT				; 
	ADD R5,R5,#-1	; counter decrement
	BRp HEXALOOP	; manage next 4 bits	
NEWLINE
	LD  R0,ASC_NEWL	; finish one line
	OUT				; \n
	BR  GONEXTHIST	;



;Intro:The code uses a loop to process each 4-bit nibble of the input number. 
;It shifts the nibble to the leftmost position and then checks if the highest bit is set. 
;If it is set, it adds 1 to the nibble and then shifts it again. 
;This process repeats until all four bits have been processed. 
;The resulting ASCII character is output to the console. The loop then repeats for the next nibble until all 16 bits have been processed.

;Table
;R0: Used to store the value of the ASCII character for output to the console.
;R1: Used to store the value of the current nibble being processed.
;R2: Used to store the value of the current bit being processed.
;R3: Used to store the value of either the ASCII character ‘0’ or the ASCII character ‘A’.
;R4: Used to count the number of bits remaining in the current nibble being processed.
;R5: Used to count the total number of bits that have been processed.
;R6: Used as a temporary register


PRINT_HIST
AND R3, R3, #0      ;COUNTER
PRINTLOOP
    	LD R0, NUM64        ;GET THE ASCII OF WHAT SHOULD BE PRINTED AT THE FIRST OF A LINE
	ADD R0, R0, R3
	OUT				
	LD R0, SPACE		;PRINT ' '
	OUT					
	LD R4, HIST_ADDR    ;GET THE LOCATION WHERE SOTRE THE NUMBER OF EACH ALPHA
	ADD R4, R4, R3		
	LDR R2, R4, #0      ;READ THE NUMBER TO R2
	BRnzp HEXA    ;TRANSFROM THE NUMBER OF R2 TO HEX

GONEXTHIST
	ADD R3, R3, #1		; NEXT IN HISTGROM
	AND R6, R6, #0
    	ADD R6, R6, #-13    ; R6 = -26
    	ADD R6, R6, #-13
	ADD R6, R6, R3
	BRnz PRINTLOOP      ; WHEN R3 - 26 <= 0, THERE ARE STILL STH NEED TO BE PRINTED
	BRnzp DONE




DONE	HALT			; done
; the data needed by the program
SPACE .FILL x0020
NUM64 .FILL x0040
NUM_BINS	.FILL #27	; 27 loop iterations
NEG_AT		.FILL xFFC0	; the additive inverse of ASCII '@'
AT_MIN_Z	.FILL xFFE6	; the difference between ASCII '@' and 'Z'
AT_MIN_BQ	.FILL xFFE0	; the difference between ASCII '@' and '`'
HIST_ADDR	.FILL x3F00     ; histogram starting address
ASC_A		.FILL x0041	; ASCII 'A'
ASC_ZERO	.FILL X0030	; ASCII '0'
ASC_NEWL 	.FILL x000A	; ASCII '\n'

STR_START	.FILL x4000	; string starting address
; for testing, you can use the lines below to include the string in this
; program...
;STR_START	.FILL STRING	; string starting address
;STRING		.STRINGZ "AaaabCd..//.;'[]="



	; the directive below tells the assembler that the program is done
	; (so do not write any code below it!)

	.END