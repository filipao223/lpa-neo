#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TEMP 1024
#define MAX_NODES 200




/*************************************************************
 * Checks if node set no longer contains unvisited nodes.
 * 
 * Parameters:
 *      node_set[]: Set contaning visited and unvisited nodes;
 *      num_nodes: Number of nodes of the graph.
 * 
 * Return:
 *      1 if set is empty, -1 otherwise;
 *************************************************************/
int set_is_empty(int node_set[], int num_nodes){
    int is_empty = 1; //Assume it's empty
    for (int i=0; i<num_nodes; i++){
        /*If node wasn't yet visited*/
        if (node_set[i] != -1){
            return -1;
        }
    }
    return is_empty;
}




/************************************************************************
 * Returns unvisited node with smallest total distance
 * 
 * Parameters:
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      node_set[]: Set contaning visited and unvisited nodes;
 *      num_nodes: Number of nodes of the graph.
 * 
 * Returns:
 *      Index of node with smallest distance.
 * 
 *************************************************************************/
int smallest_dist(int graph[][MAX_NODES], int node_set[], int num_nodes){
    int min = __INT_MAX__, node=-1;
    for (int i=0; i<num_nodes; i++){
        /*If node wasn't yet visited*/
        if (node_set[i] != -1){
            /*If node distance is smallest than current min*/
            if (graph[i][i] < min){
                min = graph[i][i];
                node = i;
            }
        }
    }

    return node;
}





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
 * Main algorithm function.
 * Adapted from pseudocode in slides: https://pastebin.com/jFTWTjQ3
 * Algorithm stores calculated distance in provided adjacency matrix.
 * 
 * Parameters:
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 *      source: Node where algorithm will start searching shortest path;
 *      target: Target node.
 * 
 ************************************************************************/
void dijkstra(int graph[][MAX_NODES], int num_nodes, int source, int target){
    /*Start by setting distance of the nodes to infinity (max integer value)*/
    /*Uses array diagonal, because nodes can't be connected to themselves*/
    for (int i=0; i<num_nodes; i++) graph[i][i] = __INT_MAX__;

    /*Set source distance to zero*/
    graph[source][source] = 0;

    /*Create a set containing all nodes, will be unvisited nodes*/
    int node_set[num_nodes];
    for (int i=0; i<num_nodes; i++) node_set[i] = 1;

    /*While set containg unvisited nodes is not empty*/
    while (set_is_empty(node_set, num_nodes) != 1){
        /*Find node with smallest distance in node_set*/
        int smallest = smallest_dist(graph, node_set, num_nodes);

        /*Remove picked node from unvisited nodes*/
        node_set[smallest] = -1;

        /*If smallest node is our target, break*/
        if (smallest == target) break;

        /*For each possible unvisited node from picked smallest node*/
        for (int i=0; i<num_nodes; i++){
            /*If node smallest is connected to node i and it wasn't yet visited*/
            if (graph[smallest][i] != 0 && node_set[i] != -1){
                /*If total distance of node i is bigger than the path from smallest to i, then update new distance*/
                if (graph[i][i] > graph[smallest][smallest] + graph[smallest][i]){
                    graph[i][i] = graph[smallest][smallest] + graph[smallest][i];
                }
            }
        }
    }
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
    dijkstra(graph, num_nodes, source, target);

    /*Print results*/
    print_solution(graph, num_nodes);

    return 0;
}