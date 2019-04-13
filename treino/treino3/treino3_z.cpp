#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <queue>
#include <list>

#define MAX_TEMP 1024
#define MAX_SIZE 451

using namespace std;




/*********************************************************************
 * Prints received input. For debugging only.
 * 
 * Parameters:
 *      matrix[][MAX_SIZE]: Matrix containg pixel values;
 *      rows: Number of rows of the matrix;
 *      columns: Number of columns of the matrix;
 * 
 *********************************************************************/
void print_input(char matrix[][MAX_SIZE], int rows, int columns){
    printf("Received info: \n");
    printf("Rows: %d\nColumns: %d\n", rows, columns);
    for (int i=0; i<rows; i++){
        for (int j=0; j<columns; j++){
            printf("%c", matrix[i][j]);
        }
        printf("\n");
    }
}






/************************************************************************
 * Checks if a pixel is inside the matrix using a row and column number.
 * 
 * Parameters:
 *      rows: Number of rows of the matrix;
 *      columns: Number of columns of the matrix;
 *      source_r: Row number to be checked;
 *      source_c: Column number to be checked.
 * 
 * Returns:
 *      True if pixel is inside matrix, false otherwise
 * 
 ************************************************************************/
bool inside_limits(int rows, int columns, int source_r, int source_c){
    return source_r >= 0 && source_r<rows && source_c >= 0 && source_c<columns;
}






/****************************************************************************************
 * Searches adjacent pixels.
 * If a pixel exists and represents a strain, then its adjacent pixels are checked
 *  if they are also a strain. If they are, they are added to a visited matrix, and it's
 *  adjacent pixels are also checked.
 * 
 * Parameters:
 *      matrix[][MAX_SIZE]: Matrix containg pixel values;
 *      rows: Number of rows of the matrix;
 *      columns: Number of columns of the matrix;
 *      source_r: Row number to be checked;
 *      source_c: Column number to be checked;
 *      visited[][]: Matrix containing which pixels were visited already.
 * 
 * Returns:
 *      1 if a strain was found, 0 otherwise.
 * 
 ****************************************************************************************/
int search(char matrix[][MAX_SIZE], int rows, int columns, int source_r, int source_c, int visited[][MAX_SIZE]){
    if ( inside_limits(rows, columns, source_r, source_c) && visited[source_r][source_c] != 1)
    {
        if (matrix[source_c][source_r] == '+')
        {
            /*Add this pixel to the visited list*/
            visited[source_r][source_c] = 1;

            /*Visit possible pixels belonging to same stain*/
            search(matrix, rows, columns, source_r+1, source_c-1, visited);
            search(matrix, rows, columns, source_r+1, source_c,   visited);
            search(matrix, rows, columns, source_r+1, source_c+1, visited);
            search(matrix, rows, columns, source_r,   source_c-1, visited);
            search(matrix, rows, columns, source_r,   source_c+1, visited);
            search(matrix, rows, columns, source_r-1, source_c-1, visited);
            search(matrix, rows, columns, source_r-1, source_c,   visited);
            search(matrix, rows, columns, source_r-1, source_c+1, visited);
            return 1;
        }
    }

    return 0;
}





/****************************************************
 * Main function, reads the graph input from stdin.
 * Uses search() function on each pixel of the image
 * 
 ****************************************************/
int main(){
    char temp[MAX_TEMP];
    int rows, columns;
    char matrix[MAX_SIZE][MAX_SIZE];

    while(fgets(temp, MAX_TEMP, stdin) != NULL){
        char *token = strtok(temp, " ");
        /*Get rows*/
        rows = atoi(token);
        token = strtok(NULL, " ");

        /*Get columns*/
        columns = atoi(token);

        /*Initialize matrix to '-'*/
        for (int i=0; i<rows; i++){
            for (int j=0; j<columns; j++){
                matrix[i][j] = '-';
            }
        }

        /*Get matrix data*/
        for (int i=0; i<rows; i++){
            fgets(temp, MAX_TEMP, stdin);
            for (int j=0; j<(int)strlen(temp); j++){
                if (temp[j] == '\n') break;
                matrix[i][j] = temp[j];
            }
        }

        /*Initialize visited matrix to 0's*/
        int visited[MAX_SIZE][MAX_SIZE] = {{0}};

        /*Print received input*/
        //print_input(matrix, rows, columns);

        /*Algorithm*/
        int counter=0;
        for (int i=0; i<rows; i++){
            for (int j=0; j<columns; j++){
                counter += search(matrix, rows, columns, i, j, visited);
            }
        }

        printf("%d\n", counter);
    }

    return 0;
}