#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>

#define MAX_TEMP 1024
#define MAX_NODES 401





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
    int num_nodes, num_connections;

    while(fgets(temp, MAX_TEMP, stdin) != NULL){
        /*Get number of nodes*/
        num_nodes = atoi(temp)+1;

        /*Get number of connections*/
        fgets(temp, MAX_TEMP, stdin);
        num_connections = atoi(temp);

        /*Set graph to 0*/
        for (int i=0; i<num_nodes; i++){
            for (int j=0; j<num_nodes; j++){
                graph[i][j] = 0;
            }
        }

        for (int t=0; t<num_connections; t++){
            /*Get connection data*/
            fgets(temp, MAX_TEMP, stdin);
            char *token = strtok(temp, " ");

            int source = atoi(token); token = strtok(NULL, " ");
            int target = atoi(token); token = strtok(NULL, " ");
            int weight = atoi(token);

            graph[source][target] = weight;
        }

        /*Print received input*/
        //print_input(graph, num_nodes, 3);

        /*Apply algorithm*/
        floyd_warshall(graph, num_nodes);

        /*Get number of queries*/
        fgets(temp, MAX_TEMP, stdin);
        int num_queries = atoi(temp);

        for (int t=0; t<num_queries; t++){
            /*Get query*/
            fgets(temp, MAX_TEMP, stdin);
            char *token = strtok(temp, " ");

            int source = atoi(token); token = strtok(NULL, " ");
            int target = atoi(token);

            /*Print value*/
            if (graph[source][target] == __INT_MAX__) printf("Impossible!\n");
            else printf("%d\n", graph[source][target]);
        }
    }

    return 0;
}