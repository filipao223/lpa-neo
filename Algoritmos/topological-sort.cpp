#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

#define MAX_TEMP 1024
#define MAX_NODES 101



using namespace std;




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





/*********************************************************************************************
 * Topologically sorts a DAG. Verifies if it's a cyclic graph.
 * Adapted from: https://en.wikipedia.org/wiki/Topological_sorting
 * 
 * Parameters:
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 *      current: Current node being visited;
 *      *sorted: Vector containing the sorted node's id's;
 *      visited[]: Array representing which nodes have been or are being visited.
 * 
 * Returns:
 *      -1 if a cycle was found, 1 othwerwise.
 * 
 *********************************************************************************************/
int sort(int graph[][MAX_NODES], int num_nodes, int current, vector<int> *sorted, int visited[]){
    /*If current node was already visited*/
    if (visited[current]==1) return 1;
    /*If current node is already being visited in another stack call*/
    if (visited[current]==-1) return -1;

    /*Mark the node with a temporary mark*/
    visited[current] = -1;

    /*For each neighbor of current node*/
    for (int j=0; j<num_nodes; j++){
        /*If a connection exists*/
        if (graph[current][j]!=0){
            if (sort(graph, num_nodes, j, sorted, visited)==-1) return -1;
        }
    }

    /*Mark this node as visited*/
    visited[current] = 1;
    /*Add the node to the list*/
    (*sorted).emplace_back(current);

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
    //int source, target;
    int maxval; //For nice printing only, ignore

    /*Source and target nodes*/
    fgets(temp, MAX_TEMP, stdin);
    //source = atoi(temp);
    fgets(temp, MAX_TEMP, stdin);
    //target = atoi(temp);

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
    //print_input(graph, num_nodes, 0, 0, width);

    /*List that will hold sorted nodes*/
    vector<int> sorted_nodes;
    /*Array that holds which nodes have been visited*/
    /*A temporary mark is -1 and a permanent mark is 1*/
    int visited[num_nodes] = {0};

    /*Use algorithm*/
    /*While we have unvisited nodes*/
    for (int i=0; i<num_nodes; i++){
        if (visited[i]==0){
            if (sort(graph, num_nodes, i, &sorted_nodes, visited)==-1)
            {
                printf("Not a DAG.\n");
                exit(1);
            }
        }
    }

    /*Print results*/
    printf("Sorted graph: \n");
    for (auto it = sorted_nodes.rbegin(); it != sorted_nodes.rend(); ++it){
        printf("%d ", *it);
    }

    return 0;
}