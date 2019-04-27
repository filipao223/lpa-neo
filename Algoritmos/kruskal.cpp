#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "disjoint-set.h"

#define MAX_TEMP 1024
#define MAX_NODES 101





/******************************************
 * Structure that represents a graph edge.
 * u: node u;
 * v: node v;
 *
 ******************************************/
typedef struct Edge{
    int u, v;
    int weight;
} Edge;






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







/************************************************************************
 * Comparator function for qsort(). Smaller weighted Edges first.
 * 
 ************************************************************************/
int comparator(const void *p, const void *q){
    if (((struct Edge*)p)->weight < ((struct Edge*)q)->weight) return -1;
    else return 1;
}







/*******************************************************************************
 * Kruskal's algorithm. Finds a minimum spanning tree for a weighted graph.
 * Adapted from pseudocode: https://en.wikipedia.org/wiki/Kruskal%27s_algorithm
 * Prints the selected edges at the end.
 * 
 * Parameters: 
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 * 
 *******************************************************************************/
void kruskal(int graph[][MAX_NODES], int num_nodes){
    /*Empty set of edges, will hold result*/
    Edge *spanning_tree = (Edge*) malloc(MAX_NODES*(MAX_NODES)-1 * sizeof(int));
    int spanning_tree_size = 0;

    /*Set that will hold all edges in graph sorted by weight*/
    Edge *sorted_edges = (Edge*)  malloc(MAX_NODES*(MAX_NODES)-1 * sizeof(int));
    int sorted_edges_size = 0;

    /*Create new set*/
    int parent_set[num_nodes];
    int rank_set[num_nodes];
    int size_set[num_nodes];
    make_set(parent_set, rank_set, size_set, num_nodes);

    /*Add the edges*/
    for (int i=0; i<num_nodes; i++){
        for (int j=0; j<num_nodes; j++){
            if (graph[i][j] != 0){
                sorted_edges[sorted_edges_size].u = i;
                sorted_edges[sorted_edges_size].v = j;
                sorted_edges[sorted_edges_size].weight = graph[i][j];
                sorted_edges_size+=1;
            }
        }
    }

    /*Sort the edges*/
    qsort(sorted_edges, sorted_edges_size, sizeof(struct Edge), comparator);

    /*For each sorted edge*/
    for (int i=0; i<sorted_edges_size; i++){
        int u = sorted_edges[i].u;
        int v = sorted_edges[i].v;
        if (find_compress(parent_set, u) != find_compress(parent_set, v)){
            /*Add the edge to the spanning tree*/
            spanning_tree[spanning_tree_size] = sorted_edges[i];
            spanning_tree_size+=1;
            union_rank(parent_set, rank_set, u, v);
        }
    }

    /*Print results*/
    printf("Spanning tree: \n");
    for (int i=0; i<spanning_tree_size; i++){
        printf("Edge (%d, %d): %d\n", spanning_tree[i].u, spanning_tree[i].v, spanning_tree[i].weight);
    }

    /*Free allocated memory*/
    free(spanning_tree);
    free(sorted_edges);
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
    int maxval = -__INT_MAX__; //For nice printing only, ignore

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
    //print_input(graph, num_nodes, source, target, width);

    /*Use algorithm*/
    kruskal(graph, num_nodes);

    return 0;
}