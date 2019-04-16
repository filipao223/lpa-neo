#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <list>
#include <iterator>
#include <algorithm>

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
    printf("Graph\n");
    for (int i=0; i<num_nodes; i++){
        for (int j=0; j<num_nodes; j++){
            if (graph[i][j]==__INT_MAX__) printf("%s ", "inf");
            else printf("%*d ", width, graph[i][j]);
        }
        printf("\n");
    }
}






/******************************************************************************************
 * Main problem function.
 * Adapted from pseudocode: https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm
 * 
 * Parameters:
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 * 
 ******************************************************************************************/
void floyd_warshall(int graph[][MAX_NODES], int num_nodes){
    /*Set the diagonal to 0*/
    for (int i=0; i<num_nodes; i++) graph[i][i] = 0;

    /*Set 0 values to infinity (INT_MAX)*/
    for(int i=0; i<num_nodes; i++){
        for(int j=0; j<num_nodes; j++){
            if (i!=j && graph[i][j] == 0) graph[i][j] = __INT_MAX__;
        }
    }

    /*Actual algorithm*/
    for (int k=0; k<num_nodes; k++){
        for (int i=0; i<num_nodes; i++){
            for (int j=0; j<num_nodes; j++){
                /*Do not do calculation if any of the right side values are INT_MAX, overflow problems*/
                if (graph[i][j] > graph[i][k] + graph[k][j] && graph[i][k]!=__INT_MAX__ && graph[k][j] != __INT_MAX__)
                    graph[i][j] = graph[i][k] + graph[k][j];
            }
        }
    }
}







/********************************************************************
 * Main function, reads the graph input from sdtin
 * 
 ********************************************************************/
int main(){
    char temp[MAX_TEMP];
    int graph[MAX_NODES][MAX_NODES];
    int num_nodes;
    /*Tips of the graph*/
    int dangling_nodes[MAX_NODES];
    int largest_per_node[MAX_NODES];

    while (fgets(temp, MAX_TEMP, stdin)!=NULL){
        /*Get number of nodes*/
        num_nodes = atoi(temp);

        /*Set graph and list of danling nodes to 0*/
        for (int i=0; i<num_nodes; i++){
            dangling_nodes[i] = 0;
            largest_per_node[i] = -__INT_MAX__;
            for (int j=0; j<num_nodes; j++){
                graph[i][j] = 0;
            }
        }
        
        /*Get graph data*/
        for (int i=0; i<num_nodes-1; i++){
            fgets(temp, MAX_TEMP, stdin);
            char *token = strtok(temp, " ");

            int source = atoi(token); token = strtok(NULL, " ");
            int target = atoi(token);

            graph[source-1][target-1] = 1;
            graph[target-1][source-1] = 1;
        }
        
        /*Find dangling nodes (nodes on the ends)*/
        for (int i=0; i<num_nodes; i++){
            /*Count number of edges of each node*/
            int num_connections = 0;
            for (int j=0; j<num_nodes; j++){
                if (graph[i][j] > 0) num_connections+=1;
            }
            /*If it has more than one edge, it's not a dangling node*/
            if (num_connections<2) dangling_nodes[i] = 1;
        }

        /*Print received input*/
        //print_input(graph, num_nodes, 2);

        /*Use algorithm on graph*/
        floyd_warshall(graph, num_nodes);

        /*Find lthe largest value in each node*/
        std::list <int> small_nodes;
        std::list <int> large_nodes;
        int smallest = __INT_MAX__;
        int largest = -__INT_MAX__;
        for (int i=0; i<num_nodes; i++){
            for (int j=0; j<num_nodes; j++){
                if (i!=j && dangling_nodes[j]==1)
                {
                    if (graph[i][j] >= largest_per_node[i]) largest_per_node[i] = graph[i][j];
                }
            }
        }

        /*Find the largest and smallest values*/
        for (int i=0; i<num_nodes; i++){
            if (largest_per_node[i] >= largest) largest = largest_per_node[i];
            if (largest_per_node[i] <= smallest) smallest = largest_per_node[i];
        }

        /*Found largest and smallest, get the nodes*/
        for (int i=0; i<num_nodes; i++){
            for (int j=0; j<num_nodes; j++){
                /*If not a loop and j is a dangling node*/
                if (i!=j && dangling_nodes[j]==1)
                {
                    /*Add the node to the list if current value is the largest it has and if not already in the list*/
                    if ( graph[i][j] == largest && largest_per_node[i]==largest &&  !(find(large_nodes.begin(), large_nodes.end(), i+1)!=large_nodes.end()) ) 
                        large_nodes.push_front(i+1);
                    if ( graph[i][j] == smallest && largest_per_node[i]==smallest  && !(find(small_nodes.begin(), small_nodes.end(), i+1)!=small_nodes.end()) ) 
                        small_nodes.push_front(i+1);
                }
            }
        }

        /*Sort the nodes ascending*/
        large_nodes.sort();
        small_nodes.sort();

        /*Print the lists*/
        printf("Shortest: ");
        for (list<int>::iterator i = small_nodes.begin(); i != small_nodes.end(); ++i)
            printf("%d ", *i);
        
        printf("\nTallest: ");
        for (list<int>::iterator i = large_nodes.begin(); i != large_nodes.end(); ++i)
            printf("%d ", *i);
        printf("\n");
    }

    return 0;
}