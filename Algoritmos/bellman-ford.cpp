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
 *      source: Node where algorithm will start searching shortest path;
 *      target: Target node;
 *      width: Largest width of any value, for nice printing.
 * 
 ************************************************************************/
void print_input(int graph[][MAX_NODES], int num_nodes, int source, int target, int width){
    printf("Source: %d\n", source);
    printf("Target: %d\n", target);
    for (int i=0; i<num_nodes; i++){
        for (int j=0; j<num_nodes; j++){
            printf("%*d ", width, graph[i][j]);
        }
        printf("\n");
    }
}






/************************************************************************
 * Prints calculated shortest path to each node of the graph.
 * If value is equal to 2147483647, then node wasn't visited
 * 
 * Parameters:
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph.
 * 
 ************************************************************************/
void print_solution(int graph[][MAX_NODES], int num_nodes){
    for(int i=0; i<num_nodes; i++){
        printf("Smallest path to node %d is %d\n", i, graph[i][i]);
    }
}







/************************************************************************
 * Main problem function.
 * Adapted from pseudocode from slides: https://pastebin.com/jFTWTjQ3
 * 
 * Parameters:
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 *      source: Node where algorithm will start searching shortest path;
 *      target: Target node.
 * 
 * Returns:
 *      -1 if any negative cycle was found and 1 otherwise.
 * 
 ************************************************************************/
int bellman_ford(int graph[][MAX_NODES], int num_nodes, int source, int target){
    /*Start by setting distance of the nodes to infinity (max integer value)*/
    /*Uses array diagonal, because nodes can't be connected to themselves*/
    for (int i=0; i<num_nodes; i++) graph[i][i] = __INT_MAX__;

    /*Set source distance to zero*/
    graph[source][source] = 0;

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
 * Input format is as follows:
 *  <source node>
 *  <target node>
 *  <adjacency matrix>
 ********************************************************************/
int main(int argc, char **argv){
    char temp[MAX_TEMP];
    int graph[MAX_NODES][MAX_NODES];
    int num_nodes=0, i=0, j=0;
    int source, target;
    int maxval; //For nice printing only, ignore

    /*Source and target nodes*/
    fgets(temp, MAX_TEMP, stdin);
    source = atoi(temp);
    fgets(temp, MAX_TEMP, stdin);
    target = atoi(temp);

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
    //print_input(graph, num_nodes, source, target, width);

    /*Use algorithm*/
    if(bellman_ford(graph, num_nodes, source, target)==-1){
        printf("Negative cycle found.\n");
        exit(1);
    }

    /*Print results*/
    print_solution(graph, num_nodes);

    return 0;
}