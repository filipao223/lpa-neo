#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TEMP 1024
#define MAX_NODES 1001





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






/************************************************************************
 * Main problem function.
 * Using Bellman-Ford algorithm to detect if a negative cycle exists.
 * 
 * Parameters:
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 * 
 * Returns:
 *      -1 if any negative cycle was found and 1 otherwise.
 * 
 ************************************************************************/
int bellman_ford(int graph[][MAX_NODES], int num_nodes){
    /*Start by setting distance of the nodes to infinity (max integer value)*/
    /*Uses array diagonal, because nodes can't be connected to themselves*/
    for (int i=0; i<num_nodes; i++) graph[i][i] = __INT_MAX__;

    /*Set source distance to zero*/
    graph[0][0] = 0;

    /*Iterate |V| - 1, i.e, number of nodes - 1 */
    for (int i=0; i<num_nodes-1; i++){
        /*For each possible connection*/
        for (int j=0; j<num_nodes; j++){
            for (int k=0; k<num_nodes; k++){
                if (k==j || graph[j][k] == 0) continue; //Node connected to itself or nodes have no connection
                /*If total distance of node k is bigger than the path from j to k, then update new distance*/
                if (graph[k][k] > graph[j][j] + graph[j][k]){
                    graph[k][k] = graph[j][j] + graph[j][k];
                }
            }
        }
    }

    /*Iteration number |V| serves to detect any negative cycles*/
    /*For each possible connection*/
    for (int j=0; j<num_nodes; j++){
        for (int k=0; k<num_nodes; k++){
            if (k==j || graph[j][k] == 0) continue; //Node connected to itself or nodes have no connection
            /*If total distance of node k is bigger than the path from j to k, then we have a negative cycle*/
            if (graph[k][k] > graph[j][j] + graph[j][k]){
                return -1;
            }
        }
    }

    return 1;
}






/********************************************************************
 * Main function, reads the graph input from stdin
 * 
 ********************************************************************/
int main(int argc, char **argv){
    char temp[MAX_TEMP];
    int num_cases, num_nodes=0, num_wormholes;
    int maxval; //For nice printing only, ignore

    /*Get number of cases*/
    fgets(temp, MAX_TEMP, stdin);
    num_cases = atoi(temp);

    for (int t=0; t<num_cases; t++){
        int graph[MAX_NODES][MAX_NODES] = {{0}};
        char *token;

        /*Get number of nodes*/
        fgets(temp, MAX_TEMP, stdin);
        token = strtok(temp, " ");
        num_nodes = atoi(token);

        /*Get number of wormholes*/
        token = strtok(NULL, " ");
        num_wormholes = atoi(token);

        /*Get wormholes (graph edges)*/
        for (int k=0; k<num_wormholes; k++){
            fgets(temp, MAX_TEMP, stdin);
            token = strtok(temp, " ");

            /*Get source star system and destination system (i and j)*/
            int i = atoi(token); token = strtok(NULL, " ");
            int j = atoi(token); token = strtok(NULL, " ");

            /*Cost of the travel (edge weight)*/
            graph[i][j] = atoi(token);

            if(atoi(token)>maxval) maxval=atoi(token); //For nice printing only, ignore
        }

        /*Print received input*/
        //int width = round(1+log(maxval)/log(10)); //Adjust for different sized numbers in output
        //print_input(graph, num_nodes, width);

        /*Use algorithm*/
        if (bellman_ford(graph, num_nodes)==-1) printf("possible\n");
        else printf("not possible\n");
    }

    return 0;
}