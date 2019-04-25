#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <queue>

#define MAX_TEMP 1024
#define MAX_NODES 5001

using namespace std;





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
 *      **graph: Adjacency matrix containg graph edge weights;
 *      node_set[]: Set contaning visited and unvisited nodes;
 *      num_nodes: Number of nodes of the graph.
 * 
 * Returns:
 *      Index of node with smallest distance.
 * 
 *************************************************************************/
int smallest_dist(int **graph, int node_set[], int num_nodes){
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
 *      **graph: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 *      width: Largest width of any value, for nice printing.
 * 
 ************************************************************************/
void print_input(int **graph, int num_nodes, int width){
    printf("Received:\n");
    for (int i=0; i<num_nodes; i++){
        for (int j=0; j<num_nodes; j++){
            printf("%*d ", width, graph[i][j]);
        }
        printf("\n");
    }
}








/*********************************************************************************************
 * Main algorithm function.
 * Uses BFS search to find the strongest reachable enemy base.
 * 
 * Parameters:
 *      **graph: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 *      current: Id of the starting base;
 *      visited[]: Array that contains which id's have been checked ([id0, id1, ..., idN]),
 *                  if a value is -1, then id at that index hasn't been visited, 1 othwerwise.
 * 
 * Returns:
 *      The node id of the best enemy base found, -1 if nothing was found.
 * 
 *********************************************************************************************/
int bfs(int **graph, int num_nodes, int current, int visited[]){
    /*FIFO queue that holds neighbour nodes of current that haven't yet been visited*/
    queue <int> bfs_queue;
    /*Push the first node (the source node)*/
    bfs_queue.push(current);

    /*Initialize best value (strongest enemy base power) and node id (which base)*/
    int best_value = -__INT_MAX__;
    int best_node = -1;

    /*While stack has nodes to visit*/
    while(!bfs_queue.empty()){
        /*Update current node and pop*/
        current = bfs_queue.front();
        bfs_queue.pop();

        /*We have visited this function node*/
        visited[current] = 1;

        /*For each edge that goes out of current node*/
        int power = 0;
        for (int j=0; j<num_nodes; j++){
            /*If a connection actually exists and it's an enemy base*/
            if (graph[current][j] != 0 && graph[j][j] < 0)
            {
                /*Add its power count*/
                power += abs(graph[j][j]);

                /*If we haven't visited it yet, we add it to the queue to visit*/
                if ( visited[j] == 0 ) bfs_queue.push(j);
                
            }
        }

        /*If local power is the best yet, save it*/
        if (power > best_value){
            best_value = power;
            best_node = current;
        }
    }

    return best_node;
}








/************************************************************************
 * Adapted from pseudocode in slides: https://pastebin.com/jFTWTjQ3
 * Algorithm stores calculated distance in provided adjacency matrix.
 * 
 * Parameters:
 *      **graph: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 *      source: Source node;
 *      target: Target node.
 * 
 ************************************************************************/
void dijkstra(int **graph, int num_nodes, int source, int target){
    /*Start by setting distance of the nodes to infinity (max integer value)*/
    /*Uses array diagonal, because nodes can't be connected to themselves*/
    for (int i=0; i<num_nodes; i++) graph[i][i] = __INT_MAX__;

    /*Set source distance to zero*/
    graph[source][source] = 0;

    /*Create a set containing all nodes, will be unvisited nodes*/
    int node_set[num_nodes];
    for (int i=0; i<num_nodes; i++) node_set[i] = 1;

    /*While set containg unvisited nodes is not empty*/
    while ( !set_is_empty(node_set, num_nodes) ){
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
                if (graph[i][i] > graph[smallest][smallest] + 1 && graph[smallest][smallest] != __INT_MAX__){
                    graph[i][i] = graph[smallest][smallest] + 1;
                }
            }
        }
    }
}









/************************************************************************
 * Main function, reads the graph input from stdin.
 * First we find the strongest base using BFS, and then we find the
 *  best path to that base (if it exists).
 * 
 ************************************************************************/
int main(){
    char temp[MAX_TEMP];
    int num_nodes;

    /*Matrix that contains adjacency matrix*/
    int **graph = (int**)malloc((MAX_NODES) * sizeof *graph);
    int *data = (int*)malloc((MAX_NODES) * (MAX_NODES) * sizeof *data);
    for (int i=0; i<MAX_NODES; i++, data+=MAX_NODES) graph[i] = data;

    while(fgets(temp, MAX_TEMP, stdin) != NULL){
        num_nodes = atoi(temp);

        /*Initialize matrix to 0*/
        for (int i=0; i<num_nodes; i++){
            for (int j=0; j<num_nodes; j++){
                graph[i][j] = 0;
            }
        }

        /*Get graph data*/
        for (int i=0; i<num_nodes; i++){
            fgets(temp, MAX_TEMP, stdin);
            char *token = strtok(temp, " ");
            /*Get base power level*/
            graph[i][i] = atoi(token); token = strtok(NULL, " ");

            /*Update it's neighbors*/
            int num_neighbors = atoi(token);
            for (int j=0; j<num_neighbors; j++){
                token = strtok(NULL, " ");
                graph[i][atoi(token)-1] = 1;
            }
        }

        /*Print received input*/
        //int width = round(1+log(maxval)/log(10)); //Adjust for different sized numbers in output
        //print_input(graph, num_nodes, 3);

        /*Set visited node array to 0*/
        int visited[MAX_NODES] = {0};

        /*Find the strongest base*/
        int best_node = bfs(graph, num_nodes, 0, visited);

        /*If no base was found*/
        if (best_node == -1) printf("No threats!\n");
        else{
            /*Use dijkstra to find best path*/
            dijkstra(graph, num_nodes, 0, best_node);

            if (graph[best_node][best_node]!=0) printf("%d\n", graph[best_node][best_node]);
            else printf("No threats!\n");
        }
    }

    /*Free memory*/
    free(*graph);
    free(graph);

    return 0;
}