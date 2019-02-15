#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ROW 400
#define MAX_COL 400
#define MAX_TEMP 1024

int chess(int board[][MAX_ROW], int x, int y, int moves){
    if (moves<0 || x<0 || y<0 || x>399 || y>399) {
        return 0;
    }

    /*printf("X: %d\tY: %d\tMoves left: %d\n",x,y,moves);*/

    int v=0;
    if (board[x][y]==0){
        board[x][y]=1;
        v=1;
    }
    return v + chess(board, x+2, y+1, moves-1) + chess(board, x+2, y-1, moves-1)
        + chess(board, x-2, y+1, moves-1) + chess(board, x-2, y-1, moves-1)
        + chess(board, x+1, y+2, moves-1) + chess(board, x+1, y-2, moves-1)
        + chess(board, x-1, y+2, moves-1) + chess(board, x-1, y-2, moves-1);
}

int main(){
    /*Get the input*/
    char temp[MAX_TEMP];
    while(fgets(temp, MAX_TEMP, stdin)!=NULL){
        int n = atoi(temp);
        int j, counter=0;

        /*Initiliaze board*/
        int board[MAX_ROW][MAX_COL] = {{0}};

        for(j=0; j<n; j++){
            fgets(temp, MAX_TEMP, stdin);
            char* token = strtok(temp, " ");
            int x = atoi(token); token = strtok(NULL, " ");
            int y = atoi(token); token = strtok(NULL, " ");
            int moves = atoi(token);

            /*Run chess*/
            counter += chess(board, x+200, y+200, moves);
        }

        printf("%d\n", counter);
    }

    return 0;
}
