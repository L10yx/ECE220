; partners: sj57, zhiboz3
;
;
; ece220/fa23/mp2
; Yuxuan Lin
; yuxuan42@illinois.edu
; partners: 
; Sicheng Jiang
; sj57@illinois.edu
; Zhibo Zhang
; zhiboz3@illinois.edu
;
;
;INTRO :
;	MP2 Stack Calculator
;	The program processes keypress inputs, adding them to the stack if they are numbers or performing operations on them if they are +, -, *, /, or ^. 
;	We check for stack underflow after popping twice before each operation to ensure that we are operating on the digit inputs. 
;	If an operator is seen, then the program calls the corresponding subroutine to perform the operation of the numbers in the stack. 
;	Then we push the result into the stack. This is repeated until the user inputs “=” where we then check if there’s only one value in the stack and print that value along with storing the final result into R5. 
;	If anytime during the program the user inputs a character that isn’t a digit or operator then we print “INVALID EXPRESSION” and halt the program. 
;	This is also done if underflow occurs.
;	Explination:
;	A postfix expression is a sequence of numbers (1,5, etc.) and operators (+, *,  - , etc.) where every operator comes after its pair of operands. 
;	For example 3 + 2 would be represented as 3 2 + ­in postfix. The expression (3 − 4) + 5 with 2 operators would be 3 4 − 5 + in postfix. 
;	Warning: 
;	Code will be tested with only non-negative single digit input integers (0-9) in the expression. The output and intermediate values on the stack could be positive or negative integers.
;	Do not have to handle overflow or underflow of registers.
;	Assume that the input expressions will have a space between operators and operands. Your code should not be outputting a space to the screen unless the user inputs a space!
;	The output of the expression should be stored in R5.


.ORIG x3000
	
;your code goes here
	
NEW_INPUT
	GETC
	OUT
	JSR EVALUATE ; use a stack to evaluate the expression

;PRINT_HEX subroutine to print the value of the evaluated expression stored in R5 to the screen in hexadecimal format.
PRINT_HEX
	JSR HEXA

DONE
	HALT


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Subroutine from lab1/mp1
;R3- value to print in hexadecimal

;Intro: This code uses a loop to process each 4-bit half byte of the input number. 
;It shifts the half byte to the leftmost position and checks if the highest bit is set. 
;If the highest bit is set, add 1 to the half byte and then shift again. 
;This process is repeated until all four bits are processed. Output ASCII characters to the console.
;Then loop through the next half byte until all 16 bits are processed.
;
;Table
;R0: Used to store the value of the ASCII character for output to the console.
;R1: Used to store the value of the current nibble being processed.
;R2: Used to store the value of either the ASCII character ‘0’ or the ASCII character ‘A’.
;R3: Used to store the value of the current bit being processed.
;R4: Used to count the number of bits remaining in the current nibble being processed.
;R6: Used to count the total number of bits that have been processed.
HEXA
	ST R0, SAVER0	; Save R0
	ST R1, SAVER1	; Save R1
	ST R2, SAVER2	; Save R2
	ST R3, SAVER3	; Save R3
	ST R4, SAVER4	; Save R4
	ST R6, SAVER6	; Save R6
	ST R7, SAVER7	; Save R7
	ADD R3,R5,#0	; input stored in R3
	AND R6,R6,#0	;
	ADD R6,R6,#4	; set R6 as counter for 16 bits
HEXALOOP	
	AND R1,R1,#0	; clear R1
	ADD R4,R1,#4	; set R4 as counter for 4 bits
FOURLOOP
	ADD R1,R1,R1	; left shift R1
	ADD R3,R3,#0	; 
	BRzp SHIFT		; if the highest bit is 0, directly go to SHIFT
	ADD R1,R1,#1	; else highest bit is 1, R1 plus 1
SHIFT 
	ADD R3,R3,R3	; left shift R3 for second highest bit
	ADD R4,R4,#-1	; counter decrement
	BRp FOURLOOP	; if not 4 bits already, go to loop again
	ADD R4,R4,#4	; set R4 as counter
	ADD R1,R1,#-10	; examine if R1 > 9
	BRzp TEN		; if R1 >= 10, go to TEN
	ADD R1,R1,#10	; R1 recover
	LD 	R2,ASC_ZERO	; else load R2 with ASCII of '0'
	ADD R0,R1,R2	; get ASCII
	OUT				; 
	ADD R6,R6,#-1	; counter decrement
	BRp HEXALOOP	; manage next 4 bits
	BR	NEWLINE     ;
TEN
	LD 	R2,ASC_A	; else load R2 with ASCII of 'A'
	ADD R0,R1,R2	; get ASCII
	OUT				; 
	ADD R6,R6,#-1	; counter decrement
	BRp HEXALOOP	; manage next 4 bits	
NEWLINE
	LD  R0,ASC_NEWL	; finish one line
	OUT				; \n
	LD R0, SAVER0	; Load R0
	LD R1, SAVER1	; Load R1
	LD R2, SAVER2	; Load R2
	LD R3, SAVER3	; Load R3
	LD R4, SAVER4	; Load R4
	LD R6, SAVER6	; Load R6
	LD R7, SAVER7	; Load R7
	RET
ASC_A		.FILL x0041	; ASCII 'A'
ASC_ZERO	.FILL X0030	; ASCII '0'
ASC_NEWL 	.FILL x000A	; ASCII '\n'

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; INTRO:
;	Use a stack to evaluate the expression
;	If the read value is an operand, push it onto the stack. 
;	If the read value is an operator, pop two values from the stack, 
;	apply the operator on the two values and push the result back on the stack. 
;	Keep repeating this for every value read from keyboard and stop when you reach the ASCII value of the = (equal sign) character. 
;
;R0 - character input from keyboard
;R6 - current numerical output
;
;
EVALUATE

;your code goes here
ST R7, SAVER7	     ; Save R7
	AND R1, R1, #0
	AND R2, R2, #0
	AND R3, R3, #0	; Clear R3
	AND R4, R4, #0	; Clear R4

	IF_EQUAL
		LD  R6, ASC_EQL	;  if R0 is the equal sign
		ADD R1, R0, R6		; if R1 is zero
	BRz OUTPUT			

	IF_SPACE
		LD  R6, SPACE	; if R0 is the space sign
		ADD R1, R0, R6		
	BRz NEW_INPUT		

	IF_NUMBER
		LD R6, NUM_LOWBOUND		; if R0 is larger than 0. To define number			
		ADD R1, R0, R6
		BRn IF_PLUS
		LD R6, NUM_HIGHBOUND
		ADD R1, R0, R6
		BRp IF_PLUS
		LD  R6, NUM_LOWBOUND	
		ADD R0, R0, R6		 ; Remove ASCII of 0
		JSR PUSH			 ; Push
		BRnzp NEW_INPUT		 ; jump to input

	IF_PLUS				    ; when it is a plus sign
		LD R6, ASC_PLUS		; if value is a plus sign
		ADD R1, R0, R6		; if R1 is zero
		BRnp IF_MIN		    ; not 
		JSR POP				; pop two numbers, when pop fails go, then print invalid
		ADD R5, R5, #0
		BRp IVD_OUTPUT
		ADD R4, R4, R0
		JSR POP
		ADD R5, R5, #0
		BRp IVD_OUTPUT
		ADD R3, R3, R0
		JSR PLUS			
		JSR PUSH			; push
		BRnzp NEW_INPUT		; input

	IF_MIN			        ; when it is a minus sign
		LD R6, ASC_MIN		; if value is a minus sign
		ADD R1, R0, R6		; if R1 is zero
		BRnp IF_MUL			; not
		JSR POP				; pop two numbers, when pop fails go, then print invalid
		ADD R5, R5, #0
		BRp IVD_OUTPUT
		ADD R4, R4, R0
		JSR POP
		ADD R5, R5, #0
		BRp IVD_OUTPUT
		ADD R3, R3, R0
		JSR MIN				
		JSR PUSH			; push result to the stack
		BRnzp NEW_INPUT		

	IF_MUL				; same as before
		LD R6, ASC_MUL
		ADD R1, R0, R6
		BRnp IF_DIV
		JSR POP
		ADD R5, R5, #0
		BRp IVD_OUTPUT
		ADD R4, R4, R0
		JSR POP
		ADD R5, R5, #0
		BRp IVD_OUTPUT
		ADD R3, R3, R0
		JSR MUL
		JSR PUSH
		BRnzp NEW_INPUT

	IF_DIV				; same as before
		LD R6, ASC_DIV
		ADD R1, R0, R6
		BRnp IF_EXP
		JSR POP
		ADD R5, R5, #0
		BRp IVD_OUTPUT
		ADD R4, R4, R0
		JSR POP
		ADD R5, R5, #0
		BRp IVD_OUTPUT
		ADD R3, R3, R0
		JSR DIV
		JSR PUSH
		BRnzp NEW_INPUT

	IF_EXP				; same as before
		LD R6, ASC_EXP
		ADD R1, R0, R6
		BRnp IF_IVD
		JSR POP
		ADD R5, R5, #0
		BRp IVD_OUTPUT
		ADD R4, R4, R0
		JSR POP
		ADD R5, R5, #0
		BRp IVD_OUTPUT
		ADD R3, R3, R0
		JSR EXP
		JSR PUSH
		BRnzp NEW_INPUT

	IF_IVD				; invalid outputs end up there
		BRnzp IVD_OUTPUT

	OUTPUT				; Check if there is only one answer in stack
	LD R1, STACK_TOP
	ADD R1, R1, #1
	LD R2, STACK_START
	NOT R2, R2
	ADD R2, R2, #1
	ADD R1, R1, R2
	BRz GO_OUTPUT		; 
	BRnzp IVD_OUTPUT

	GO_OUTPUT		
		JSR POP
		AND R5, R5, #0
		ADD R5, R5, R0
		BRnzp RET_EVAL

	IVD_OUTPUT		; Invalid
		LEA R0, STR_IVD
		PUTS
	BRnzp DONE

RET_EVAL			; restore R7
	LD R7, SAVER7
RET

; INTRO:
;	Detailed operations including PLUS, MIN, MUL, DIV, EXP
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
PLUS
;your code goes here
	AND R0, R0, #0
	ADD R0, R3, R4
RET	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MIN
;your code goes here
	AND R0, R0, #0
	NOT R4, R4
	ADD R4, R4, #1	; reverse R4, remember to plus one
	ADD R0, R3, R4
RET		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MUL	
;your code goes here
	AND R0, R0, #0
	LOOP_MUL
		ADD R0, R0, R3
		ADD R4, R4, #-1
	BRp LOOP_MUL
RET		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
DIV	
;your code goes here
	AND R0, R0, #0
	NOT R4, R4
	ADD R4, R4, #1
	LOOP_DIV
		ADD R0, R0, #1
		ADD R3, R3, R4
	BRzp LOOP_DIV
	ADD R0, R0, #-1
RET	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
EXP
;your code goes here
ST R5, SaveR5_EXP			; save R5
	AND R5, R5, #0
	ADD R5, R5, R4			; R5 is loop coounter for EXP
	ADD R5, R5, #-1
	ST  R3, SAVER4_EXP		; store R3 to R4
	LOOP_EXP
		LD 	R4, SAVER4_EXP
		JSR MUL				; use MUL loop to compute EXP
		ADD R3, R0, #0		; add result of one MUL loop to R3
		ADD R5, R5, #-1		; counter decrement
		BRp LOOP_EXP
LD R5, SaveR5_EXP			; load R5
RET	


;;;;;;;;;;;;;;;;;;;;;;ORIGINAL PROG2;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;
PUSH	
	ST R3, PUSH_SaveR3	; save R3
	ST R4, PUSH_SaveR4	; save R4
	AND R5, R5, #0		;
	LD R3, STACK_END	;
	LD R4, STACk_TOP	;
	ADD R3, R3, #-1		;
	NOT R3, R3			;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz OVERFLOW		; stack is full
	STR R0, R4, #0		; no overflow, store value in the stack
	ADD R4, R4, #-1		; move top of the stack
	ST R4, STACK_TOP	; store top of stack pointer
	BRnzp DONE_PUSH		;
OVERFLOW
	ADD R5, R5, #1		;
DONE_PUSH
	LD R3, PUSH_SaveR3	;
	LD R4, PUSH_SaveR4	;
	RET


PUSH_SaveR3	.BLKW #1	;
PUSH_SaveR4	.BLKW #1	;


;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP	
	ST R3, POP_SaveR3	; save R3
	ST R4, POP_SaveR4	; save R3
	AND R5, R5, #0		; clear R5
	LD R3, STACK_START	;
	LD R4, STACK_TOP	;
	NOT R3, R3			;
	ADD R3, R3, #1		; 
	ADD R3, R3, R4		;
	BRz UNDERFLOW		;
	ADD R4, R4, #1		;
	LDR R0, R4, #0		;
	ST R4, STACK_TOP	;
	BRnzp DONE_POP		;
UNDERFLOW
	ADD R5, R5, #1		;
DONE_POP
	LD R3, POP_SaveR3	;
	LD R4, POP_SaveR4	;
	RET

SAVER4_EXP	.BLKW #1	; storage for registers
SAVER0		.BLKW #1	;
SAVER1		.BLKW #1	;
SAVER2		.BLKW #1	;
SAVER3		.BLKW #1	;
SAVER4		.BLKW #1	;
SAVER5		.BLKW #1	;
SAVER6		.BLKW #1	;
SAVER7		.BLKW #1	;
SaveR5_EXP 	.BLKW #1	; similar
SPACE 		.FILL #-32	; reverse for ASCII of " "
NUM_LowBound 		.FILL #-48	; 
NUM_HighBound		.FILL #-57	;
ASC_PLUS			.FILL #-43	;
ASC_MIN				.FILL #-45	;
ASC_MUL				.FILL #-42	;
ASC_DIV 			.FILL #-47	;
ASC_EXP				.FILL #-94	;
ASC_EQL				.FILL #-61	;similar
STR_IVD				.STRINGZ "Invalid Expression" ; out for invalid situation

POP_SaveR3	.BLKW #1	; original prog2 provided
POP_SaveR4	.BLKW #1	;
STACK_END	.FILL x3FF0	;
STACK_START	.FILL x4000	;
STACK_TOP	.FILL x4000	;


.END