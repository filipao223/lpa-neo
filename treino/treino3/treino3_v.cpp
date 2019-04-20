#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>

#define MAX_TEMP 1024
#define MAX_NODES 601





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
    printf("Graph: \n");
    for (int i=0; i<num_nodes; i++){
        for (int j=0; j<num_nodes; j++){
            if (graph[i][j]==__INT_MAX__) printf("%s ", "inf");
            else printf("%*d ", width, graph[i][j]);
        }
        printf("\n");
    }
}






/********************************************************************************************
 * Main problem function.
 * Adapted from pseudocode: https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm
 * 
 * Parameters:
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 * 
 ********************************************************************************************/
void floyd_warshall(int graph[][MAX_NODES], int num_nodes){
    /*Set the diagonal to 0*/
    for (int i=0; i<num_nodes; i++){
        graph[i][i] = 0;
    }

    /*Set 0 values to infinity (INT_MAX)*/
    for(int i=0; i<num_nodes; i++){
        for(int j=0; j<num_nodes; j++){
            if (i!=j && graph[i][j] == 0) graph[i][j] = __INT_MAX__;
        }
    }

    /*Actual algorithm*/
    for (int k=0; k<num_nodes; k++){
        for (int i=0; i<num_nodes; i++){
            for (int j=0; j<num_nodes; j++){
                /*Do not do calculation if any of the right side values are INT_MAX, overflow problems*/
                if (graph[i][j] > graph[i][k] + graph[k][j] && graph[i][k]!=__INT_MAX__ && graph[k][j] != __INT_MAX__)
                    graph[i][j] = graph[i][k] + graph[k][j];
            }
        }
    }
}






/********************************************************************
 * Main function, reads the graph input from sdtin
 * 
 ********************************************************************/
int main(int argc, char **argv){
    char temp[MAX_TEMP];
    int graph[MAX_NODES][MAX_NODES];
    int num_nodes, j;

    /*Get number of nodes*/
    fgets(temp, MAX_TEMP, stdin);
    num_nodes = atoi(temp);

    while(1){
        /*Set graph to 0*/
        for (int i=0; i<num_nodes; i++){
            for (int j=0; j<num_nodes; j++){
                graph[i][j] = 0;
            }
        }

        /*Read the graph*/
        for (int i=0; i<num_nodes; i++){
            fgets(temp, MAX_TEMP, stdin);
            char *token = strtok(temp, " ");

            j=0;
            while(token != NULL){
                if ( atoi(token)==-1 ) graph[i][j] = 0;
                else graph[i][j] = atoi(token);

                token = strtok(NULL, " ");
                j+=1;
            }
        }


        /*Print received input*/
        //print_input(graph, num_nodes, 3);

        /*Apply algorithm*/
        floyd_warshall(graph, num_nodes);

        int end=0;
        int source, target;

        while(1)
        {
            /*Receive next line*/
            if (fgets(temp, MAX_TEMP, stdin)==NULL){
                /*EOF*/
                end = 1;
                break;
            }

            /*Check if it's a query (two integers) or a new testcase (one integer)*/
            if (sscanf(temp, "%d %d\n", &source, &target)==2)
            {
                /*It's a query*/
                printf("%d\n", graph[source-1][target-1]);
            }
            else if (sscanf(temp, "%d %d\n", &num_nodes, &source)==1)
            {
                /*It's a new testcase*/
                break;
            }
            else
            {
                /*Something else happened*/
                end=1;
                break;
            }
        }

        if (end==1) break;
    }

    return 0;
}