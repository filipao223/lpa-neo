#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <queue>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

#define MAX_TEMP 1048
#define MAX_NODES 5001



using namespace std;




/****************************************
 * Returns the smallest of two numbers
 * 
 * Parameters:
 *      a: First number to compare;
 *      b: Second number to compare.
 * 
 * Returns:
 *      The smallest of the two numbers.
 * 
 ****************************************/
int min(int a, int b){ return a>b?b:a; }







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
void print_input(int **graph, int num_nodes, int source, int target, int width){
    printf("Source: %d\n", source);
    printf("Target: %d\n", target);
    for (int i=0; i<num_nodes; i++){
        for (int j=0; j<num_nodes; j++){
            printf("%*d ", width, graph[i][j]);
        }
        printf("\n");
    }
}





/*********************************************************************************************
 * Main algorithm function.
 * Uses BFS search a path from source node to desired target node. Does not result in smallest
 *  path necessarily, but a path.
 * 
 * Parameters:
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 *      current: Starting node;
 *      target: Target node;
 *      visited[]: Array that contains which nodes have been checked, if a value is 0, 
 *                 then node hasn't been visited, 1 othwerwise;
 *      previous[]: Array that holds previous node of node at index i.
 * 
 * Returns:
 *      An int array representing previous node of index node (paths);
 * 
 *********************************************************************************************/
int bfs(int **graph, int num_nodes, int current, int visited[]){
    /*FIFO queue that holds neighbour nodes of current that haven't yet been visited*/
    queue <int> bfs_queue;
    /*Push the first node (the source node)*/
    bfs_queue.push(current);

    int largest = 0;

    /*While stack has nodes to visit*/
    while(!bfs_queue.empty()){
        /*Update current node and pop*/
        current = bfs_queue.front();
        bfs_queue.pop();

        /*Increase largest connected network value*/
        largest+=1;

        /*We have visited this function node*/
        visited[current] = 1;

        /*For each edge that goes out of current node*/
        for (int j=0; j<num_nodes; j++){
            if (graph[current][j]!=0){
                /*If we haven't visited node yet*/
                if(visited[j]==0){
                    bfs_queue.push(j);
                }
            }
        }
    }

    return largest;
}








/********************************************************************
 * Main function, reads the graph input from stdin
 * 
 * Input format is as follows:
 *  <source node>
 *  <target node>
 *  <adjacency matrix>
 ********************************************************************/
int main(){
    char temp[MAX_TEMP];
    int num_nodes, num_connections;
    //int maxval; //For nice printing only, ignore

    /*Matrix that contains adjacency matrix*/
    int **graph = (int**)malloc((MAX_NODES) * sizeof *graph);
    int *data = (int*)malloc((MAX_NODES) * (MAX_NODES) * sizeof *data);
    for (int i=0; i<MAX_NODES; i++, data+=MAX_NODES) graph[i] = data;


    while(fgets(temp, MAX_TEMP, stdin) != NULL){
        char *token = strtok(temp, " ");

        /*Get number of users (num of nodes) and connections*/
        num_nodes = atoi(token); token = strtok(NULL, " ");
        num_connections = atoi(token);

        /*Reset the graph*/
        for (int i=0; i<num_nodes; i++){
            for (int j=0; j<num_nodes; j++){
                graph[i][j] = 0;
            }
        }

        /*String map to store node info*/
        map<string, int> nodes_map;

        /*Get each user (node) information*/
        for (int i=0; i<num_nodes; i++){
            fgets(temp, MAX_TEMP, stdin);
            if (temp[strlen(temp)-1] == '\n') temp[strlen(temp)-1] = '\0'; //Remove trailing newline (if existing)
            nodes_map[temp] = i;
        }

        /*Get the actual graph connections*/
        for (int i=0; i<num_connections; i++){
            fgets(temp, MAX_TEMP, stdin);

            /*Find the index of the first name*/
            token = strtok(temp, " ");
            int source = nodes_map[token];

            /*Find the index of the second name*/
            token = strtok(NULL, " ");
            if (token[strlen(token)-1] == '\n') token[strlen(token)-1] = '\0'; //Remove trailing newline (if existing)
            int target = nodes_map[token];

            /*Make the connection*/
            graph[source][target] = 1;
            graph[target][source] = 1;
        }

        /*Print received input*/
        //int width = round(1+log(maxval)/log(10)); //Adjust for different sized numbers in output
        //print_input(graph, num_nodes, 0, 0, 3);

        /*Array that holds which nodes have been visited*/
        int visited[MAX_NODES] = {0};

        /*Search largest connected network*/
        int largest = -__INT_MAX__;
        for (int i=0; i<num_nodes; i++){
            if (visited[i]==0){
                /*Search*/
                int value = bfs(graph, num_nodes, i, visited);
                if (value > largest) largest = value;
            }
        }

        /*Print result*/
        printf("%d\n", largest);
    }

    /*Free memory*/
    free(*graph);
    free(graph);

    return 0;
}