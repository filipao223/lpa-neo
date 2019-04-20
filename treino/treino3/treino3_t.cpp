#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <queue>

#define MAX_TEMP 1024
#define MAX_NODES 101


using namespace std;



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







/*****************************************************************************************************************
 * Main algorithm function.
 * Uses BFS to find the smallest cycle in the graph.
 * 
 * Parameters:
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 *      id_source: Id of the function which calls we will analyse;
 *      id_target: Id of the function we are trying to find if it's recursive;
 *      visited[]: Array that contains which id's have been checked ([id0, id1, ..., idN]),
 *                  if a value is -1, then id at that index hasn't been visited, 1 othwerwise.
 *      level: Current recursive level of BFS, represents distance;
 *      best: Best value found at time of call (shortest distance yet).
 * 
 * Returns:
 *      An integer, representing best distance found (or INT_MAX if target node wasn't found).
 * 
 *****************************************************************************************************************/
int bfs(int graph[][MAX_NODES], int num_nodes, int id_source, int id_target, int visited[], int level, int best){
    /*FIFO queue that holds neighbour nodes of id_source that haven't yet been visited*/
    queue <int> bfs_queue;

    /*We have already visited this node*/
    visited[id_source] = 1;

    /*For each edge that goes out of id_source node*/
    for (int j=0; j<num_nodes; j++){
        /*If a connection actually exists*/
        if (graph[id_source][j] == 1)
        {
            /*If we have found our source node, return current edge count*/
            if (j == id_target) return level+1;
            
            /*Else, if we haven't visited it yet, we add it to the queue to visit*/
            if ( visited[j] == 0)
            {
                bfs_queue.push(j);
            }
        }
    }

    /*While we have nodes to visit*/
    while(!bfs_queue.empty())
    {
        /*If a better distance was found, save it*/
        int value = bfs(graph, num_nodes, bfs_queue.front(), id_target, visited, level+1, best);
        if (value < best) best = value;
        /*Remove node from the queue and move on to the next unvisited node*/
        bfs_queue.pop();
    }
    
    /*Didn't find our target node in this recursive step*/
    return best;
}







/*************************************************
 * Main function, reads the graph input from stdin
 * 
 *************************************************/
int main(int argc, char **argv){
    char temp[MAX_TEMP];
    int graph[MAX_NODES][MAX_NODES];
    int num_nodes=0, j=0;
    int maxval; //For nice printing only, ignore

    /*Read actual graph*/
    while(fgets(temp, MAX_TEMP, stdin) != NULL){
        /*Target node*/
        num_nodes = atoi(temp);

        /*Graph nodes*/
        for (int i=0; i<num_nodes; i++){
            fgets(temp, MAX_TEMP, stdin);
            char *token = strtok(temp, " ");

            j=0;
            while (token != NULL){
                if(atoi(token)>maxval) maxval=atoi(token); //For nice printing only, ignore

                graph[i][j] = atoi(token); //Nodes updated here
                token = strtok(NULL, " ");
                j+=1;
            }
        }

        /*Print received input*/
        //int width = round(1+log(maxval)/log(10)); //Adjust for different sized numbers in output
        //print_input(graph, num_nodes, width);

        int visited[MAX_NODES];
        int min = __INT_MAX__;

        /*Use algorithm*/
        for (int i=0; i<num_nodes; i++){
            for (int j=0; j<num_nodes; j++){
                if (graph[i][j] != 0){
                    /*Set all nodes as unvisited*/
                    for (int k=0; k<num_nodes; k++){
                        visited[k] = 0;
                    }
                    
                    /*Search for the smallest cycle starting from this node*/
                    int rc = bfs(graph, num_nodes, j, i, visited, 1, __INT_MAX__);
                    min = rc<min ? rc:min;
                }
            }
        }

        printf("%d\n", min);
    }

    return 0;
}