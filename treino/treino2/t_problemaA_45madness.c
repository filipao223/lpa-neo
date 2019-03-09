#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_TEMP 255
#define MAX_ROWS 255
#define MAX_COLS 255

/*Function that returns the biggest of two integers*/
int max(int num1, int num2){ return num1>num2?num1:num2; }

/*Function that returns largest resulting sum matrix number*/
int best_sum_matrix(int matrix[][MAX_COLS], int num_rows){
    int i,j, best=INT_MIN;
    for(i=0; i<num_rows; i++){
        for(j=0; j<num_rows; j++){
            /*If matrix value is bigger than best, then best is equal to that value*/
            best = matrix[i][j]>best ? matrix[i][j]:best;
        }
    }

    return best;
}

/*Main problem function*/
int ex1(int P[][MAX_COLS], int DP[][MAX_COLS], int num_rows){
    int i,j;
    for(i=0; i<num_rows; i++){
        for(j=0; j<i+1; j++){
            if(i==0) DP[i][j] = P[i][j];
            else DP[i][j] = P[i][j] + max(DP[i-1][j], DP[i-1][j-1]);
        }
    }

    return best_sum_matrix(DP, num_rows);
}

int main(){
    /*-------------Input code--------------*/
    int P[MAX_ROWS][MAX_COLS];

    int i=0,j,k;

    int test_cases;
    char temp[MAX_TEMP];

    /*Read number of test cases*/
    fgets(temp, MAX_TEMP, stdin);
    test_cases= atoi(temp);

    while(i < test_cases){
        /*Initialize sum array to 0*/
        int DP[MAX_ROWS][MAX_COLS] = {{0}};

        /*Read number of rows*/
        fgets(temp, MAX_TEMP, stdin);
        int num_rows= atoi(temp);

        /*Read rows*/
        for(j=0; j<num_rows; j++){
            fgets(temp, MAX_TEMP, stdin);
            char *tokens = strtok(temp, " ");

            /*While we have tokens to read*/
            for(k=0; tokens!=NULL; k++){
                P[j][k] = atoi(tokens);
                tokens = strtok(NULL, " ");
            }
        }

        /*---------End of input code---------*/
        /*---------Main problem--------------*/
        printf("%d\n", ex1(P, DP, num_rows));
        ++i;
    }

    return 0;
}