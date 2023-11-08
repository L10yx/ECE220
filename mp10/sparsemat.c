// Author: yuxuan42
// Partners: sj57, zhiboz3
#include "sparsemat.h"

#include <stdio.h>
#include <stdlib.h>


// intro
//This C code file implements operations on sparse matrices using a linked list structure.
// It contains functions to load, access, manipulate, and save sparse matrices. 
// The code includes functionalities for addition and destruction of sparse matrices.


// This function loads a sparse matrix from a file.
// It reads the file, populates matrix dimensions, 
// and iterates through the file to set non-zero entries using the set_tuples function.
sp_tuples * load_tuples(char* input_file)
{
    // Opening the file to read
    FILE *file;
    file = fopen(input_file, "r");

    // Allocating memory for the sparse matrix
    sp_tuples *mat = (sp_tuples*) malloc(sizeof(sp_tuples));

    int row, col;
    double val;
    
    // Reading matrix dimensions and initializing matrix properties
    fscanf(file, "%d %d", &row, &col);
    mat->m = row; // Number of rows
    mat->n = col; // Number of columns
    mat->nz = 0; // Number of non-zero entries in matrix
    mat->tuples_head = NULL; // Initializing the linked list head

    // Iterating through the file to set non-zero entries in the matrix
    while(fscanf(file, "%d %d %lf\n", &row, &col, &val) != EOF){
        set_tuples(mat, row, col, val); // Calling set_tuples to add elements to the matrix
    }

    fclose(file); // Closing the file

    return mat; // Returning the loaded matrix
}

// This function retrieves the value at a specified position in the sparse matrix.
// It traverses the linked list to find and return the requested value.
double gv_tuples(sp_tuples * mat_t,int row, int col) 
{
    sp_tuples_node* curr = mat_t->tuples_head; // set head as start
    // Traversing the linked list to find the value at the specified row and column
    while (curr != NULL) {
        if (curr->row == row && curr->col == col) // if found, return val
            return curr->value;
        curr = curr->next; // Move to the next node
    }
    return 0; // Return 0 if the value is not found
}

// Helper function to insert a new node into the sparse matrix.
// It allocates memory for a new node and sets its properties, then returns a pointer to it.
sp_tuples_node * insert(sp_tuples_node* next, int row, int col, double value) 
{ // helper function to insert node
    sp_tuples_node* node = (sp_tuples_node*) malloc(sizeof(sp_tuples_node));
    node->next = next;
    node->value = value;
    node->row = row;
    node->col = col;
    return node;
}


// Sets a value in the sparse matrix at a specified position.
// It adds or modifies a node in the matrix based on the input value and indices.
// Manages the linked list structure to maintain the matrix in sorted order.
void set_tuples(sp_tuples * mat_t, int row, int col, double value)
{
// Handling cases with null pointer or out of range indices
    if (mat_t == NULL) return; // null pointer
    if (row >= mat_t->m || col >= mat_t->n) return; // out range
// Handling cases when the matrix is empty
    if (mat_t->tuples_head == NULL && value != 0){ // matrix empty
        mat_t->tuples_head = insert(NULL, row, col, value);
        mat_t->nz++;
        return;
    }
// Handling other cases where nodes are present in the matrix
    sp_tuples_node* curr = mat_t->tuples_head;
    sp_tuples_node* next = curr->next;
    int target = (row * mat_t->n) + col; // aim location
    int currLoc = (curr->row * mat_t->n) + curr->col;
    int nextLoc = 0;


    if (target < currLoc){ // if ahead of head
        if (value != 0) {
            mat_t->tuples_head = insert(curr, row, col, value);
            mat_t->nz++;
        }
        return;
    }

    if (target == currLoc){ // if target exists
        if (value != 0) {
            mat_t->tuples_head->value = value;
        }
        else {
            mat_t->tuples_head = curr->next;
            free(curr);
            mat_t->nz--;
        }
        return;
    }


    while (curr->next != NULL) { 
        nextLoc = (next->row * mat_t->n) + next->col;
        if (target < nextLoc) { // between curr and next
            curr->next = insert(next, row, col, value);
            mat_t->nz++;
            return;
        }
        else if (target == nextLoc) {
            if (value != 0) {
                next->value = value;
            }
            else {
                curr->next = next->next;
                free(next);
                mat_t->nz--;               
            }
            return;
        }
        
        curr = next;
        next = curr->next;
    }

    if (next == NULL) { // add to the end
        if (value != 0) {
            curr->next = insert(NULL, row, col, value);
            mat_t->nz++;
        }
        return;           
    }

    return;
}

// Saves the sparse matrix to a file.
// Writes the matrix dimensions and non-zero entries into the file.
void save_tuples(char * file_name, sp_tuples * mat_t)
{
    // Opening the file to write
    FILE *file;
    file = fopen(file_name, "w");

    // Writing matrix dimensions to the file
    fprintf(file, "%d %d\n", mat_t->m, mat_t->n);

    if (mat_t->tuples_head == NULL) return; // If the matrix is empty, return

    // Writing non-zero entries to the file
    sp_tuples_node* curr = mat_t->tuples_head;
    while (curr != NULL){
        fprintf(file, "%d %d %lf\n", curr->row, curr->col, curr->value);
        curr = curr->next;
    }

    fclose(file); // Closing the file
    return;
}


// Adds two sparse matrices and returns a new sparse matrix.
// Creates a new matrix matC and copies the values of matA and matB into it, 
// performing addition where necessary.
sp_tuples * add_tuples(sp_tuples * matA, sp_tuples * matB){
    if (matA == NULL || matB == NULL) return NULL;

    if (matA->m != matB->m || matA->n != matB->n) return NULL; // if size not equal 

    sp_tuples* matC = (sp_tuples*) malloc(sizeof(sp_tuples)); // Creating a new matrix 'matC'
    matC->m = matA->m; // Setting the number of rows for 'matC'
    matC->n = matA->n; // Setting the number of columns for 'matC'
    matC->nz = 0; // Initializing the count of non-zero entries for 'matC'
    matC->tuples_head = NULL; // Initializing 'matC' linked list

    sp_tuples_node* curr = matA->tuples_head;

    while (curr != NULL) { // copy matA to matC
        set_tuples(matC, curr->row, curr->col, curr->value);
        curr = curr->next;
    }

    curr = matB->tuples_head; // Resetting 'curr' to 'matB' head for another iteration

    // Adding non-zero values from 'matB' to 'matC'

    while (curr != NULL) { // copy B to C
// Calculating the sum of values
        double val = curr->value + gv_tuples(matC, curr->row, curr->col); 
        set_tuples(matC, curr->row, curr->col, val); // Setting the sum in 'matC'
        curr = curr->next; // Moving to the next node in 'matB'
    }
    
	return matC;
}



sp_tuples * mult_tuples(sp_tuples * matA, sp_tuples * matB) { 
    if (matA == NULL || matB == NULL) return NULL;

    if (matA->n != matB->m) return NULL; // if size not equal 

    sp_tuples* matC = (sp_tuples*) malloc(sizeof(sp_tuples)); // Creating a new matrix 'matC'
    matC->m = matA->m; // Setting the number of rows for 'matC'
    matC->n = matB->n; // Setting the number of columns for 'matC'
    matC->nz = 0; // Initializing the count of non-zero entries for 'matC'
    matC->tuples_head = NULL; // Initializing 'matC' linked list

    
    for (sp_tuples_node* currA = matA->tuples_head; currA != NULL; currA = currA->next) {
        for (sp_tuples_node* currB = matB->tuples_head; currB != NULL; currB = currB->next) {
            if (currA->col == currB->row) {
                double val = currA->value * currB->value + gv_tuples(matC, currA->row, currB->col);
                set_tuples(matC, currA->row, currB->col, val);
            }
        }
    }

	return matC;
}

// Destroys a sparse matrix and frees memory.
// Frees memory associated with the matrix, including all its nodes and the matrix structure itself.
void destroy_tuples(sp_tuples * mat_t){
    if (mat_t == NULL) return;

    if (mat_t->tuples_head == NULL){
        free(mat_t);
        return;
    }
	sp_tuples_node* curr = mat_t->tuples_head;
	do {
        free(curr); // Free the current node
        curr = curr->next; // Move to the next node
    } while (curr != NULL);// Repeat until all nodes are freed
    free(mat_t);
    return;
}