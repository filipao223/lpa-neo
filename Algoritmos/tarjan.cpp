#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stack>
#include <set>

#define MAX_TEMP 1024
#define MAX_NODES 101



using namespace std;
int index_global = 0;
int num_components=0;



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






void tarjan(int graph[][MAX_NODES], int num_nodes, int current, stack <int> S,int index[], int lowlink[], bool onStack[], set <int> strong_component[]){
    // Set the depth index for v to the smallest unused index
    index[current] = index_global;
    lowlink[current] = index_global;
    index_global += 1;
    S.push(current);
    onStack[current] = true;

    // Consider successors of v
    for (int j=0; j<num_nodes; j++){
        /*If there's a connection*/
        if (graph[current][j] != 0){
            if (index[j] == -1){
                // Successor w has not yet been visited; recurse on it
                tarjan(graph, num_nodes, j, S, index, lowlink, onStack, strong_component);
                lowlink[current] = min(lowlink[current], lowlink[j]);
            }
            else if (onStack[j]){
                // Successor w is in stack S and hence in the current SCC
                // If w is not on stack, then (v, w) is a cross-edge in the DFS tree and must be ignored
                // Note: The next line may look odd - but is correct.
                // It says w.index not w.lowlink; that is deliberate and from the original paper
                lowlink[current] = min(lowlink[current], index[j]);
            }
        }
    }

    // If v is a root node, pop the stack and generate an SCC
    int j;
    if (lowlink[current] == index[current]){
        do{
            j = S.top();
            S.pop();
            onStack[j] = false;
            strong_component[num_components].insert(j);
        } while (j != current);

        num_components+=1;
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
    //print_input(graph, num_nodes, source, target, width);

    /*Index array (depths/id's), lowlink array and onStack array*/
    int index[num_nodes];
    int lowlink[num_nodes];
    bool onStack[num_nodes];
    for (int i=0; i<num_nodes; i++){
        index[i] = -1;
        lowlink[i] = -1;
        onStack[i] = false;
    }

    /*Empty stack*/
    stack <int> S;

    /*Array of sets, to store strong components*/
    set <int> components[num_nodes];

    /*Use algorithm*/
    for (int i=0; i<num_nodes; i++){
        if (index[i]==-1){
            tarjan(graph, num_nodes, i, S, index, lowlink, onStack, components);
        }
    }

    printf("Found %d components\n", num_components);

    return 0;
}