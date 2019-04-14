#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TEMP 1024
#define MAX_NODES 101
#define RED 1
#define BLUE 2






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







/*************************************************************************
 * Main problem function.
 * Uses DFS to travel through each arc of a given node, 
 *  coloring them if they are uncolored, and checking
 *  wether their color is the correct one if they are.
 *  When it finds a wrongly colored node, it returns 0.
 * 
 * Parameters:
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 *      current: Which node is the starting point in travel;
 *      color: Integer that represents which color the node should be;
 *      colors[]: Array holding all the colors of every node.
 * 
 * Returns:
 *      0 if a node with the wrong color was found (not a bipartite
 *       graph), 1 otherwise.
 * 
 *************************************************************************/
int dfs(int graph[][MAX_NODES], int num_nodes, int current, int color, int colors[]){
    /*Is it colored?*/
    if ( colors[current]!=0 )
    {
        if ( colors[current]==color ) return 1; //Color is correct
        else return 0; //Graph is not bipartite
    }
    /*Color it*/
    else
    {
        colors[current] = color;
        /*Travel through each arc*/
        for (int j=0; j<num_nodes; j++){
            if (graph[current][j] == 0) continue;
            /*Expect the next node to be the opposite color (if RED, then BLUE)*/
            if ( dfs(graph, num_nodes, j, color==RED?BLUE:RED, colors)==0 ) return 0;
        }

        return 1;
    }
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

        int colors[MAX_NODES] = {0};
        int flag = 0;

        /*Use algorithm*/
        for (int i=0; i<num_nodes; i++){
            /*Uncolor all nodes*/
            for (int k=0; k<num_nodes; k++){
                colors[k] = 0;
            }
            
            /*Only need one wrong case to prove it's not bipartite*/
            if ( dfs(graph, num_nodes, i, RED, colors)==0 ){
                flag = 1;
                break;
            }
        }

        if (flag == 0) printf("True\n");
        else printf("False\n");
    }

    return 0;
}