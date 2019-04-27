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
void bfs(int graph[][MAX_NODES], int num_nodes, int current, int target, int visited[], int previous[]){
    /*FIFO queue that holds neighbour nodes of current that haven't yet been visited*/
    queue <int> bfs_queue;
    /*Push the first node (the source node)*/
    bfs_queue.push(current);

    previous[0] = -1;

    /*While stack has nodes to visit*/
    while(!bfs_queue.empty()){
        /*Update current node and pop*/
        current = bfs_queue.front();
        bfs_queue.pop();

        /*We have visited this function node*/
        visited[current] = 1;

        /*For each edge that goes out of current node*/
        for (int j=0; j<num_nodes; j++){
            if (graph[current][j]!=0){
                /*If one of these nodes is our target*/
                if (j==target){
                    previous[j] = current;
                    return;
                }
                /*If we haven't visited node yet*/
                else if(visited[j]==0){
                    previous[j] = current;
                    bfs_queue.push(j);
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
    int maxval = -__INT_MAX__; //For nice printing only, ignore

    /*Source and target nodes*/
    fgets(temp, MAX_TEMP, stdin);
    source = atoi(temp);
    fgets(temp, MAX_TEMP, stdin);
    target = atoi(temp);

    /*Set the graph to 0*/
    for (int i=0; i<MAX_NODES; i++){
        for (int j=0; j<MAX_NODES; j++){
            graph[i][j] = 0;
        }
    }

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

    /*Array that holds the predecessors of nodes, and array of visited nodes*/
    int previous[num_nodes];
    for (int i=0; i<num_nodes; i++) previous[i] = -1;
    int visited[num_nodes] = {0};

    /*Use algorithm*/
    bfs(graph, num_nodes, source, target, visited, previous);

    /*Print results*/
    printf("Path: %d <- ", target);
    for (int i=target; i>0; i=previous[i]){
        printf("%d%s", previous[i], previous[previous[i]]>-1?" <- ":"");
    }

    return 0;
}