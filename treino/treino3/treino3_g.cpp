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
    for (int i=0; i<num_nodes; i++){
        for (int j=0; j<num_nodes; j++){
            printf("%*d ", width, graph[i][j]);
        }
        printf("\n");
    }
}







/*********************************************************************************************
 * Main algorithm function.
 * Uses BFS search to detect if a function is recursive or not, using a given target id
 *  representing it.
 * 
 * Parameters:
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 *      id_source: Id of the function which calls we will analyse;
 *      id_target: Id of the function we are trying to find if it's recursive;
 *      visited[]: Array that contains which id's have been checked ([id0, id1, ..., idN]),
 *                  if a value is -1, then id at that index hasn't been visited, 1 othwerwise.
 * 
 * Returns:
 *      1 if the function is recursive, 0 otherwise.
 * 
 *********************************************************************************************/
int bfs(int graph[][MAX_NODES], int num_nodes, int id_source, int id_target, int visited[]){
    /*FIFO queue that holds neighbour nodes of id_source that haven't yet been visited*/
    queue <int> bfs_queue;

    /*We have already visited this function node*/
    visited[id_source] = 1;

    /*For each edge that goes out of id_source node*/
    for (int j=0; j<num_nodes; j++){
        /*If a connection actually exists*/
        if (graph[id_source][j] == 1)
        {
            /*If we have found our target function node, then it's recursive*/
            if (j == id_target) return 1;
            
            /*Else, if we haven't visited it yet, we add it to the queue to visit*/
            else if ( visited[j] == -1)
            {
                bfs_queue.push(j);
            }
        }
    }

    /*If we have no new nodes to visit, we won't find our target node here*/
    if (bfs_queue.empty()) return 0;
    else{
        /*While we have nodes to visit*/
        while(!bfs_queue.empty())
        {
            /*If algorithm found that target node is a neighbor of the next node to visit (function is recursive)*/
            if ( bfs(graph, num_nodes, bfs_queue.front(), id_target, visited) == 1) return 1;
            /*Else, just remove it from the queue and move on to the next unvisited node*/
            else bfs_queue.pop();
        }
    }

    /*Didn't find our target node in this recursive step*/
    return 0;
}







/******************************************************************
 * Main function, reads the graph input from stdin, and calls bfs()
 *  to solve the problem.
 * 
 ******************************************************************/
int main(){
    char temp[MAX_TEMP];
    int num_nodes, maxval;

    while(fgets(temp, MAX_TEMP, stdin) != NULL){
        num_nodes = atoi(temp);

        /*Matrix that contains adjacency matrix*/
        int graph[MAX_NODES][MAX_NODES];

        /*Initialize matrix to -1*/
        for (int i=0; i<num_nodes; i++){
            for (int j=0; j<num_nodes; j++){
                graph[i][j] = -1;
            }
        }

        /*Get graph data*/
        for (int i=0; i<num_nodes; i++){
            fgets(temp, MAX_TEMP, stdin);
            char *token = strtok(temp, " ");
            token = strtok(NULL, " "); //Ignore node label

            while(token != NULL){
                if(atoi(token)>maxval) maxval=atoi(token); //For nice printing only, ignore

                graph[i][atoi(token)] = 1; //Edge weight
                token = strtok(NULL, " ");
            }
        }

        /*Print received input*/
        //int width = round(1+log(maxval)/log(10)); //Adjust for different sized numbers in output
        //print_input(graph, num_nodes, width);

        /*Algorithm*/
        /*For each function, detect if they are recursive*/
        for (int i=0; i<num_nodes; i++){
            for (int j=0; j<num_nodes; j++)
            {
                /*If function is called*/
                if (graph[i][j] == 1)
                {
                    /*If it's calling itself, it's recursive*/
                    if (i==j){
                        printf("%d\n", i);
                        break;
                    }

                    /*Visited id list, to stop BFS search*/
                    int visited[num_nodes];
                    for (int m=0; m<num_nodes; m++) visited[m] = -1;

                    /*Perform search for recursion on this function id*/
                    if (bfs(graph, num_nodes, j, i, visited) == 1)
                    {
                        /*BFS found this function being called, it's recursive*/
                        printf("%d\n", i);
                        break;
                    }
                }
            }
        }
    }

    return 0;
}