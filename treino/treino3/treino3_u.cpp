#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TEMP 1024
#define MAX_NODES 101
#define RED 1
#define BLUE 2





/************************************************************************
 * Prints received input. For debugging only.
 * 
 * Parameters:
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 *      width: Largest width of any value, for nice printing.
 * 
 ************************************************************************/
void print_input(int graph[][MAX_NODES], int num_nodes, int width){
    printf("Received input: \n");
    for (int i=0; i<num_nodes; i++){
        for (int j=0; j<num_nodes; j++){
            printf("%*d ", width, graph[i][j]);
        }
        printf("\n");
    }
}




int dfs(int graph[][MAX_NODES], int num_nodes, int current, int color, int colors[]){
    /*Is it colored?*/
    if ( colors[current]!=0 )
    {
        if ( colors[current]==color ) return 1;
        else return 0; //Graph is not bipartite
    }
    /*Color it*/
    else
    {
        colors[current] = color;
        /*Travel through each arc*/
        for (int j=0; j<num_nodes; j++){
            if (graph[current][j] == 0) continue;
            if ( dfs(graph, num_nodes, j, color==RED?BLUE:RED, colors)==0 ) return 0;
        }

        return 1;
    }
}




/*************************************************
 * Main function, reads the graph input from stdin
 * 
 *************************************************/
int main(int argc, char **argv){
    char temp[MAX_TEMP];
    int graph[MAX_NODES][MAX_NODES];
    int num_nodes=0, j=0;
    int maxval; //For nice printing only, ignore

    /*Read actual graph*/
    while(fgets(temp, MAX_TEMP, stdin) != NULL){
        /*Target node*/
        num_nodes = atoi(temp);

        /*Graph nodes*/
        for (int i=0; i<num_nodes; i++){
            fgets(temp, MAX_TEMP, stdin);
            char *token = strtok(temp, " ");

            j=0;
            while (token != NULL){
                if(atoi(token)>maxval) maxval=atoi(token); //For nice printing only, ignore

                graph[i][j] = atoi(token); //Nodes updated here
                token = strtok(NULL, " ");
                j+=1;
            }
        }

        /*Print received input*/
        //int width = round(1+log(maxval)/log(10)); //Adjust for different sized numbers in output
        //print_input(graph, num_nodes, width);

        int colors[MAX_NODES] = {0};
        int flag = 0;

        /*Use algorithm*/
        for (int i=0; i<num_nodes; i++){
            /*Uncolor all nodes*/
            for (int k=0; k<num_nodes; k++){
                colors[k] = 0;
            }
            
            if ( dfs(graph, num_nodes, i, RED, colors)==0 ){
                flag = 1;
                break;
            }
        }

        if (flag == 0)
            printf("True\n");
        else printf("False\n");
    }

    return 0;
}