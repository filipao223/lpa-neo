#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>

#define MAX_TEMP 1024
#define MAX_NODES 27





/************************************************************************
 * Prints received input. For debugging only.
 * 
 * Parameters:
 *      graph[][MAX_NODES]: Adjacency matrix containg graph edge weights;
 *      num_nodes: Number of nodes of the graph;
 *      width: Largest width of any value, for nice printing.
 * 
 ************************************************************************/
void print_input(int graph1[][MAX_NODES], int graph2[][MAX_NODES], int num_nodes, int width){
    printf("Graph 1\n");
    for (int i=0; i<26; i++) printf("%*c ", width, i+'A');
    printf("\n");
    for (int i=0; i<(width+1)*26; i++) printf("-");
    printf("\n");
    for (int i=0; i<num_nodes; i++){
        for (int j=0; j<num_nodes; j++){
            if (graph1[i][j]==__INT_MAX__) printf("%s ", "inf");
            else printf("%*d ", width, graph1[i][j]);
        }
        printf("\n");
    }
    printf("Graph 2\n");
    for (int i=0; i<num_nodes; i++){
        for (int j=0; j<num_nodes; j++){
            if (graph2[i][j]==__INT_MAX__) printf("%s ", "inf");
            else printf("%*d ", width, graph2[i][j]);
        }
        printf("\n");
    }
}






/********************************************************************************************
 * Main problem function.
 * Adapted from pseudocode: https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm
 * 
 * Parameters:
 *      graph1[][MAX_NODES]: Adjacency matrix containg graph edge weights of first gondola;
 *      graph2[][MAX_NODES]: Adjacency matrix containg graph edge weights of second gondola;
 *      num_nodes: Number of nodes of the graph;
 * 
 ********************************************************************************************/
void floyd_warshall(int graph1[][MAX_NODES], int graph2[][MAX_NODES], int num_nodes){
    /*Set the diagonal to 0*/
    for (int i=0; i<num_nodes; i++){
        graph1[i][i] = 0;
        graph2[i][i] = 0;
    }

    /*Set 0 values to infinity (INT_MAX)*/
    for(int i=0; i<num_nodes; i++){
        for(int j=0; j<num_nodes; j++){
            if (i!=j && graph1[i][j] == 0) graph1[i][j] = __INT_MAX__;
            if (i!=j && graph2[i][j] == 0) graph2[i][j] = __INT_MAX__;
        }
    }

    /*Actual algorithm*/
    for (int k=0; k<num_nodes; k++){
        for (int i=0; i<num_nodes; i++){
            for (int j=0; j<num_nodes; j++){
                /*Do not do calculation if any of the right side values are INT_MAX, overflow problems*/
                if (graph1[i][j] > graph1[i][k] + graph1[k][j] && graph1[i][k]!=__INT_MAX__ && graph1[k][j] != __INT_MAX__)
                    graph1[i][j] = graph1[i][k] + graph1[k][j];
                /*Do not do calculation if any of the right side values are INT_MAX, overflow problems*/
                if (graph2[i][j] > graph2[i][k] + graph2[k][j] && graph2[i][k]!=__INT_MAX__ && graph2[k][j] != __INT_MAX__)
                    graph2[i][j] = graph2[i][k] + graph2[k][j];
            }
        }
    }
}






/********************************************************************
 * Main function, reads the graph input from sdtin
 * 
 ********************************************************************/
int main(int argc, char **argv){
    char temp[MAX_TEMP];
    int graph1[MAX_NODES][MAX_NODES];
    int graph2[MAX_NODES][MAX_NODES];
    int num_nodes=26, num_cases;

    /*Get number of test cases*/
    fgets(temp, MAX_TEMP, stdin);
    num_cases = atoi(temp);

    for (int t=0; t<num_cases; t++){
        /*Blank line*/
        fgets(temp, MAX_TEMP, stdin);

        /*Set both graphs to 0*/
        for (int i=0; i<MAX_NODES; i++){
            for (int j=0; j<MAX_NODES; j++){
                graph1[i][j] = 0;
                graph2[i][j] = 0;
            }
        }

        /*First gondola/bus*/
        fgets(temp, MAX_TEMP, stdin);
        int temp_data = atoi(temp);

        for (int u=0; u<temp_data; u++){
            /*Get graph data*/
            fgets(temp, MAX_TEMP, stdin);
            char *token = strtok(temp, " ");

            int source = (token[0] - 'A'); token = strtok(NULL, " ");
            int target = (token[0] - 'A');

            graph1[source][target] = 1;
        }

        /*Second gondola/bus*/
        fgets(temp, MAX_TEMP, stdin);
        temp_data = atoi(temp);

        for (int u=0; u<temp_data; u++){
            /*Get graph data*/
            fgets(temp, MAX_TEMP, stdin);
            char *token = strtok(temp, " ");

            int source = (token[0] - 'A'); token = strtok(NULL, " ");
            int target = (token[0] - 'A');

            graph2[source][target] = 1;
        }

        /*Print received input*/
        //print_input(graph1, graph2, num_nodes, 2);

        /*Use algorithm on both graphs*/
        floyd_warshall(graph1, graph2, num_nodes);

        //print_input(graph1, graph2, num_nodes, 3);


        /*Compare*/
        int are_diff = 0;
        for (int i=0; i<num_nodes; i++){
            for (int j=0; j<num_nodes; j++){
                if ( (graph1[i][j] == __INT_MAX__ && graph2[i][j] != __INT_MAX__) || (graph1[i][j] != __INT_MAX__ && graph2[i][j] == __INT_MAX__)) are_diff = 1;
            }
        }

        if (are_diff) printf("NO\n");
        else printf("YES\n");
        if (t+1 < num_cases) printf("\n");
    }

    return 0;
}