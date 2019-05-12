#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TEMP 1024
#define MAX_NODES 101





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
int min(int a, int b) { return a>b?b:a; }




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







/***********************************************************************************************************************************************************
 * Function to find articulation points in a graph. Adapted from pseudocode: https://en.wikipedia.org/wiki/Biconnected_component
 * 
 * Parameters:
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 *      current: Current node being visited;
 *      depth: Current depth of the node being visited;
 *      visited[]: List that keeps track of which nodes have been visited ([id0, id1, id2, ..., idN]);
 *      parent[]: List that keeps track of the parent of index (i) node;
 *      depths[]: List that keeps track of index (i) node's depth;
 *      low[]: List that keeps track of index (i) node's low point;
 *      points[]: List that keeps track of found articulated points.
 * 
 ***********************************************************************************************************************************************************/
void articulation_points(int graph[][MAX_NODES], int num_nodes, int current, int depth, int visited[], int parent[], int depths[], int low[], int points[]){
    visited[current] = 1;
    depths[current] = depth;
    low[current] = depth;
    int child_count = 0;
    bool is_articulation = false;

    /*For each neighbor*/
    for (int j=0; j<num_nodes; j++){
        /*If a connection exists*/
        if (graph[current][j]!=0){
            if (visited[j]==0){
                parent[j] = current;
                articulation_points(graph, num_nodes, j, depth+1, visited, parent, depths, low, points);
                child_count += 1;

                if (low[j] >= depths[current]) is_articulation = true;
                low[current] = min(low[current], low[j]);
            }
            else if (j != parent[current]) low[current] = min(low[current], depths[j]);
        }
    }

    /*If node isn't the root node and is an articulation point, 
        or it is root node and has more than a child, mark it as an articulation point of graph*/
    if ((parent[current]!=-1 && is_articulation) || (parent[current]==-1 && child_count>1)) points[current] = 1;
}






/*************************************************
 * Main function, reads the graph input from stdin
 * 
 *************************************************/
int main(int argc, char **argv){
    char temp[MAX_TEMP];
    int graph[MAX_NODES][MAX_NODES];
    int num_nodes;
    //int maxval = -__INT_MAX__; //For nice printing only, ignore

    while( fgets(temp, MAX_TEMP, stdin)!=NULL ){
        char *token = strtok(temp, " ");

        /*Get number of nodes*/
        num_nodes = atoi(token);

        /*End program if number of nodes is 0*/
        if (num_nodes==0) break;

        /*Set graph to 0*/
        for (int i=0; i<num_nodes; i++){
            for (int j=0; j<num_nodes; j++){
                graph[i][j] = 0;
            }
        }

        /*Get graph data*/
        /*Get at most num_nodes lines of info*/
        for (int n=0; n<num_nodes; n++){
            fgets(temp, MAX_TEMP, stdin);
            token = strtok(temp, " ");

            /*If number is 0, there are no more blocks*/
            if (atoi(token)==0) break;

            /*Node from which there are direct connections*/
            int current = atoi(token);

            /*Get those connections (places start at 1, so -1)*/
            token = strtok(NULL, " ");
            while (token != NULL){
                graph[current-1][atoi(token)-1] = 1;
                graph[atoi(token)-1][current-1] = 1; //Lines are bidirectional
                token = strtok(NULL, " ");
            }
        }

        /*Print received input*/
        //int width = round(1+log(maxval)/log(10)); //Adjust for different sized numbers in output
        //print_input(graph, num_nodes, 0, 3);

        /*Visited nodes, parents of nodes, depths of each node and lowpoints*/
        int visited[num_nodes], parent[num_nodes], depths[num_nodes], low[num_nodes];
        /*Calculated articulation points*/
        int points[num_nodes];

        /*Initialize the arrays*/
        for (int i=0; i<num_nodes; i++){
            visited[i] = 0;
            parent[i] = -1;
            depths[i] = 0;
            low[i] = 0;
            points[i] = -1;
        }

        /*Find articulation points*/
        for (int i=0; i<num_nodes; i++){
            if (visited[i]==0) articulation_points(graph, num_nodes, i, 0, visited, parent, depths, low, points);
        }

        /*Count the points*/
        int counter = 0;
        for (int i=0; i<num_nodes; i++){
            if (points[i]==1) counter++;
        }

        /*Print the results*/
        printf("%d\n", counter);
    }

    return 0;
}