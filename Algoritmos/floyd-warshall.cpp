#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TEMP 1024
#define MAX_NODES 200





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
    for (int i=0; i<num_nodes; i++){
        for (int j=0; j<num_nodes; j++){
            printf("%*d ", width, graph[i][j]);
        }
        printf("\n");
    }
}






/***********************************************************************************
 * Prints calculated shortest path to each node pair of the graph.
 * If diagonal contains any non 0 value, then graph has at least one negative cycle.
 * 
 * Parameters:
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph.
 * 
 ***********************************************************************************/
void print_solution(int graph[][MAX_NODES], int num_nodes){
    /*------FOR NICE PRINTING ONLY-----*/
    /*Find maximum value*/
    int max = -__INT_MAX__;
    for(int i=0; i<num_nodes; i++){
        for (int j=0; j<num_nodes; j++){
            if (graph[i][j] != __INT_MAX__ && graph[i][j] > max) max = graph[i][j];
        }
    }
    max = max>999 ? max:999; //3 spaces for 'inf' word
    int width = round(1+log(max)/log(10)); //Adjust for different sized numbers in output
    /*---END OF NICE PRINTING ONLY-----*/

    /*Actual values*/
    printf("Results: \n");
    for (int i=0; i<num_nodes; i++){
        for (int j=0; j<num_nodes; j++){
            /*'inf' means no path available for that pair of nodes*/
            if (graph[i][j] == __INT_MAX__) printf(" inf ");
            else printf("%*d ", width, graph[i][j]);
        }
        printf("\n");
    }
}






/******************************************************************************************
 * Main problem function.
 * Adapted from pseudocode: https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm
 * 
 * Parameters:
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 * 
 ******************************************************************************************/
void floyd_warshall(int graph[][MAX_NODES], int num_nodes){
    /*Set the diagonal to 0*/
    for (int i=0; i<num_nodes; i++) graph[i][i] = 0;

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
 * Main function, reads the graph input from stdin
 * 
 * Input format is as follows:
 *  <adjacency matrix>
 ********************************************************************/
int main(int argc, char **argv){
    char temp[MAX_TEMP];
    int graph[MAX_NODES][MAX_NODES];
    int num_nodes=0, i=0, j=0;
    int maxval; //For nice printing only, ignore

    /*Read actual graph*/
    while(fgets(temp, MAX_TEMP, stdin) != NULL){
        char *token = strtok(temp, " ");
        j=0;
        while (token != NULL){
            if(atoi(token)>maxval) maxval=atoi(token); //For nice printing only, ignore

            graph[i][j] = atoi(token); //Nodes updated here
            token = strtok(NULL, " ");
            j+=1;
        }
        i+=1;
        num_nodes+=1; //Count overall number of nodes
    }

    /*Print received input*/
    //int width = round(1+log(maxval)/log(10)); //Adjust for different sized numbers in output
    //print_input(graph, num_nodes, width);

    /*Use algorithm*/
    floyd_warshall(graph, num_nodes);

    /*Print results*/
    print_solution(graph, num_nodes);

    return 0;
}