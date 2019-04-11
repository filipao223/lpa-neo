#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TEMP 1024
#define MAX_NODES 300




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
 *      target: Target node;
 *      width: Largest width of any value, for nice printing.
 * 
 ************************************************************************/
void print_input(int graph[][MAX_NODES], int num_nodes, int target, int width){
    printf("Target: %d\n", target);
    for (int i=0; i<num_nodes; i++){
        for (int j=0; j<num_nodes; j++){
            printf("%*d ", width, graph[i][j]);
        }
        printf("\n");
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
 *      target: Target node.
 * 
 ************************************************************************/
void dijkstra(int graph[][MAX_NODES], int num_nodes, int target){
    /*Start by setting distance of the nodes to infinity (max integer value)*/
    /*Uses array diagonal, because nodes can't be connected to themselves*/
    for (int i=0; i<num_nodes; i++) graph[i][i] = __INT_MAX__;

    /*Set source distance to zero*/
    graph[0][0] = 0;

    /*Create a set containing all nodes, will be unvisited nodes*/
    int node_set[num_nodes];
    for (int i=0; i<num_nodes; i++) node_set[i] = i;

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
            if (graph[smallest][i] != -1 && node_set[i] != -1){
                /*If total distance of node i is bigger than the path from smallest to i, then update new distance*/
                if (graph[i][i] > graph[smallest][smallest] + graph[smallest][i]){
                    graph[i][i] = graph[smallest][smallest] + graph[smallest][i];
                }
            }
        }
    }
}





/********************************************************************
 * Main function, reads the graph input as a file passed as parameter
 * 
 ********************************************************************/
int main(int argc, char **argv){
    char temp[MAX_TEMP];
    int graph[MAX_NODES][MAX_NODES];
    int num_nodes=0, j=0;
    int target;
    int maxval; //For nice printing only, ignore

    /*Read actual graph*/
    while(fgets(temp, MAX_TEMP, stdin) != NULL){
        /*Target node*/
        char *token = strtok(temp, " ");
        num_nodes = atoi(token); token = strtok(NULL, " ");
        target = atoi(token);

        /*Graph nodes*/
        for (int i=0; i<num_nodes; i++){
            fgets(temp, MAX_TEMP, stdin);
            token = strtok(temp, " ");
            token = strtok(NULL, " "); //Ignore node label

            j=0;
            while (token != NULL){
                if(atoi(token)>maxval) maxval=atoi(token); //For nice printing only, ignore

                graph[i][j] = atoi(token); //Nodes updated here
                token = strtok(NULL, " ");
                j+=1;
            }
        }
    }

    /*Print received input*/
    //int width = round(1+log(maxval)/log(10)); //Adjust for different sized numbers in output
    //print_input(graph, num_nodes, target, width);

    /*Use algorithm*/
    dijkstra(graph, num_nodes, target);

    /*Print solution*/
    printf("%d\n", graph[target-1][target-1]);

    return 0;
}