#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TEMP 1024
#define MAX_NODES 101





/*************************************************************
 * Checks if node set no longer contains unvisited nodes.
 * 
 * Parameters:
 *      node_set[]: Set contaning visited and unvisited nodes;
 *      num_nodes: Number of nodes of the graph.
 * 
 * Return:
 *      true if set is empty, false otherwise;
 *************************************************************/
bool set_is_empty(int node_set[], int num_nodes){
    for (int i=0; i<num_nodes; i++){
        /*If node wasn't yet visited*/
        if (node_set[i] == 1){
            return false;
        }
    }
    return true;
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






/*******************************************************************************************
 * Function that prints taken path.
 * Source: https://www.geeksforgeeks.org/printing-paths-dijkstras-shortest-path-algorithm/
 * 
 * Arguments:
 *      parent[]: Array that contains each parent node for each node (index);
 *      j: Which node to start path reconstruction (target node).
 * 
 *******************************************************************************************/
void printPath(int parent[], int j){
    // Base Case : If j is source 
    if (parent[j] == - 1) 
        return; 
  
    printPath(parent, parent[j]); 
  
    printf("%d ", j); 
}






/************************************************************************
 * Main algorithm function.
 * Adapted from pseudocode in slides: https://pastebin.com/jFTWTjQ3
 * Algorithm stores calculated distance in provided adjacency matrix.
 * Also stores path taken, to calculate elevator switching.
 * 
 * Parameters:
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 *      target: Target node.
 * 
 ************************************************************************/
void dijkstra(int graph[][MAX_NODES], int num_nodes, int target){
    /*Array to store path*/
    //int parent[num_nodes];

    /*Start by setting distance of the nodes to infinity (max integer value)*/
    /*Uses array diagonal, because nodes can't be connected to themselves*/
    for (int i=0; i<num_nodes; i++){
        graph[i][i] = __INT_MAX__;
        //parent[i] = -1;
    }

    /*Set source distance to zero*/
    graph[0][0] = 0;

    /*Create a set containing all nodes, will be unvisited nodes*/
    int node_set[num_nodes];
    for (int i=0; i<num_nodes; i++) node_set[i] = 1;

    /*While set containg unvisited nodes is not empty*/
    while ( !set_is_empty(node_set, num_nodes) ){
        /*Find node with smallest distance in node_set*/
        int smallest = smallest_dist(graph, node_set, num_nodes);

        /*If there is no possible node, break*/
        if (smallest == -1) break;

        /*Remove picked node from unvisited nodes*/
        node_set[smallest] = -1;

        /*If smallest node is our target, break*/
        if (smallest == target) break;

        /*For each possible unvisited node from picked smallest node*/
        for (int i=0; i<num_nodes; i++){
            /*If node smallest is connected to node i and it wasn't yet visited*/
            if (graph[smallest][i] != -1 && node_set[i] != -1 && smallest!=-1){
                /*If total distance of node i is bigger than the path from smallest to i, then update new distance*/
                /*Do not do calculation if any of the right side values are INT_MAX, overflow problems*/
                if ((graph[i][i] > graph[smallest][smallest] + graph[smallest][i]) && graph[smallest][smallest] !=__INT_MAX__){
                    graph[i][i] = graph[smallest][smallest] + graph[smallest][i];
                    //parent[i] = smallest;
                }
            }
        }
    }
}







/*************************************************
 * Main function, reads the graph input from stdin
 * 
 *************************************************/
int main(int argc, char **argv){
    char temp[MAX_TEMP];
    int graph[MAX_NODES][MAX_NODES];
    int num_nodes=100, num_elevators, target;
    int maxval = -__INT_MAX__; //For nice printing only, ignore

    while( fgets(temp, MAX_TEMP, stdin)!=NULL ){
        char *token = strtok(temp, " ");

        /*Get number of elevators and target floor*/
        num_elevators = atoi(token); token = strtok(NULL, " ");
        target = atoi(token);


        /*Get travel time of each elevator*/
        fgets(temp, MAX_TEMP, stdin);
        token = strtok(temp, " ");
        int elevator_time[num_elevators];
        for (int i=0; i<num_elevators; i++){
            if (atoi(token)>maxval) maxval = atoi(token); //For nice printing only, ignore

            elevator_time[i] = atoi(token);
            token = strtok(NULL, " ");
        }

        /*Set graph to -1*/
        for (int i=0; i<num_nodes; i++){
            for (int j=0; j<num_nodes; j++){
                graph[i][j] = -1;
            }
        }

        /*Get graph data (which floors are accessible by which elevator)*/
        for (int i=0; i<num_elevators; i++){
            fgets(temp, MAX_TEMP, stdin);
            token = strtok(temp, " ");

            int previous_floor = -1;

            while( token!=NULL ){
                int current_floor = atoi(token);
                int gap;

                /*If it's not the first floor, connect both floors (weight is the travel time of elevator i)*/
                if (previous_floor!=-1)
                {
                    if (current_floor-previous_floor>1) gap = current_floor-previous_floor-1;
                    else gap = 0;

                    graph[previous_floor][current_floor] = elevator_time[i] + elevator_time[i]*gap;
                    graph[current_floor][previous_floor] = elevator_time[i] + elevator_time[i]*gap;
                }
                /*Save previous elevator*/
                previous_floor = current_floor;

                token = strtok(NULL, " ");
            }
        }

        /*Print received input*/
        //int width = round(1+log(maxval)/log(10)); //Adjust for different sized numbers in output
        //print_input(graph, num_nodes, target, width);

        /*Use algorithm*/
        dijkstra(graph, num_nodes, target);

        /*Print solution*/
        int value = graph[target][target];
        if (value==__INT_MAX__) printf("IMPOSSIBLE\n");
        else printf("%d\n", value);
    }

    return 0;
}