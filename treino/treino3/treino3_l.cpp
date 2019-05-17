#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TEMP 1024
#define MAX_NODES 751

typedef struct node{
    int x, y;
    int index;
} Node;


typedef struct Edge{
    struct node u, v;
    double distance;
} Edge;




double distance(Node n1, Node n2){
    return sqrt( fabs(n1.x - n2.x) * fabs(n1.x - n2.x) + fabs(n1.y - n2.y) * fabs(n1.y - n2.y));
}






/******************************************************************************
 * Creates a new set by filling in data to the arrays passed as parameter.
 * Parent_set[] indexes are the nodes, and the value the parent nodes.
 * Rank_set[] and size_set[] are for efficient union functions.
 * 
 * Parameters:
 *      parent_set[]: Parent nodes;
 *      rank_set[]: Ranks of each node;
 *      size_set[]: Size value of each node;
 *      num_nodes: Number of total graph nodes.
 * 
 ******************************************************************************/
void make_set(int parent_set[], int rank_set[], int size_set[], int num_nodes){
    /*Create a new set with num_nodes nodes, all of them are their own parents*/
    for (int i=0; i<num_nodes; i++){
        parent_set[i] = i;
        rank_set[i] = 0;
        size_set[i] = 1;
    }
}






/******************************************************************
 * Finds the root element of a given node's set.
 *  Follows the chain of parent pointers from x up the tree until 
 *  it reaches a root element, whose parent is itself.
 * 
 * Compression: Flattens the tree by making every node 
 *  point to the root whenever Find is used on it.
 * 
 * Parameters:
 *      parent_set[]: Parent nodes;
 *      x: Node to evaluate.
 * 
 * Returns:
 *      Root element of x's set.
 * 
 ******************************************************************/
int find_compress(int parent_set[], int x){
    if (parent_set[x] != x){
        parent_set[x] = find_compress(parent_set, parent_set[x]);
    }

    return parent_set[x];
}





/*******************************************************************************
 * Union uses Find to determine the roots of the trees x and y belong to. 
 *  If the roots are distinct, the trees are combined by attaching the root 
 *  of one to the root of the other.
 * 
 * By rank: Always attaches the shorter tree to the root of the taller tree. 
 *  Thus, the resulting tree is no taller than the originals unless they were 
 *  of equal height, in which case the resulting tree is taller by one node.
 * 
 * Parameters:
 *      parent_set[]: Parent nodes;
 *      rank_set[]: Set containing ranks (height) of each node;
 *      x: First node's set;
 *      y: Second node's set.
 * 
 *******************************************************************************/ 
void union_rank(int parent_set[], int rank_set[], int x, int y){
    int xRoot = find_compress(parent_set, x);
    int yRoot = find_compress(parent_set, y);

    /* x and y are already in the same set */
    if (xRoot == yRoot) return;

    /* x and y are not in same set, so we merge them */
    if (rank_set[xRoot] < rank_set[yRoot]){
        int temp = xRoot;
        xRoot = yRoot;
        yRoot = temp;
    }

    /*Merge yRoot into xRoot*/
    parent_set[yRoot] = xRoot;
    if (rank_set[xRoot] == rank_set[yRoot]) rank_set[xRoot] = rank_set[xRoot] + 1;
}





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







/************************************************************************
 * Comparator function for qsort(). Smaller weighted Edges first.
 * 
 ************************************************************************/
int comparator(const void *p, const void *q){
    if (((struct Edge*)p)->distance < ((struct Edge*)q)->distance) return -1;
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
void kruskal(int graph[][MAX_NODES], int num_nodes, Edge *sorted_edges, int *sorted_edges_size, Edge *spanning_tree, int *spanning_tree_size, Node *nodes, double *existing){
    /*Create new set*/
    int parent_set[num_nodes];
    int rank_set[num_nodes];
    int size_set[num_nodes];
    make_set(parent_set, rank_set, size_set, num_nodes);

    /*Add the edges*/
    for (int i=0; i<num_nodes; i++){
        for (int j=i; j<num_nodes; j++){
            //printf("I is %d and J is %d, s_edges_size = %d\n", i, j, *sorted_edges_size);
            sorted_edges[*sorted_edges_size].u = nodes[i];
            sorted_edges[*sorted_edges_size].v = nodes[j];
            sorted_edges[*sorted_edges_size].distance = distance(nodes[i], nodes[j]);
            (*sorted_edges_size)+=1;
        }
    }

    /*Sort the edges*/
    qsort(sorted_edges, *sorted_edges_size, sizeof(struct Edge), comparator);

    /*For each sorted edge*/
    for (int i=0; i<(*sorted_edges_size); i++){
        int u = sorted_edges[i].u.index;
        int v = sorted_edges[i].v.index;
        /*Check if this edge is already selected (pre made tree)*/
        if (graph[u][v] == 1){
            spanning_tree[*spanning_tree_size] = sorted_edges[i];
            (*spanning_tree_size)+=1;
            (*existing)+=sorted_edges[i].distance;
            union_rank(parent_set, rank_set, u, v);
        }
        /*It isn't, continue with algorithm*/
        else if (find_compress(parent_set, u) != find_compress(parent_set, v)){
            /*Add the edge to the spanning tree*/
            spanning_tree[*spanning_tree_size] = sorted_edges[i];
            (*spanning_tree_size)+=1;
            union_rank(parent_set, rank_set, u, v);
        }
    }
}





/*************************************************
 * Main function, reads the graph input from stdin
 * 
 *************************************************/
int main(){
    char temp[MAX_TEMP];
    int graph[MAX_NODES][MAX_NODES];
    int num_nodes;
    //int maxval = -__INT_MAX__; //For nice printing only, ignore

    while( fgets(temp, MAX_TEMP, stdin)!=NULL ){
        /*Get number of nodes*/
        char *token = strtok(temp, " ");
        num_nodes = atoi(token);

        /*Set graph to 0*/
        for (int i=0; i<num_nodes; i++){
            for (int j=0; j<num_nodes; j++){
                graph[i][j] = 0;
            }
        }

        /*Get coordinates of the nodes*/
        Node node_xy[num_nodes];
        for (int i=0; i<num_nodes; i++){
            /*Get x and y*/
            fgets(temp, MAX_TEMP, stdin); token = strtok(temp, " ");
            node_xy[i].x = atoi(token); token = strtok(NULL, " ");
            node_xy[i].y = atoi(token);
            node_xy[i].index = i;
        }

        /*Get number of connections*/
        fgets(temp, MAX_TEMP, stdin);
        int num_connections = atoi(temp);

        /*Get connections*/
        for (int i=0; i<num_connections; i++){
            /*Get node source and target*/
            fgets(temp, MAX_TEMP, stdin); token = strtok(temp, " ");
            int source = atoi(token); token = strtok(NULL, " ");
            int target = atoi(token);
            graph[source-1][target-1] = 1;
            graph[target-1][source-1] = 1;
        }

        /*Empty set of edges, will hold result*/
        Edge *spanning_tree = (Edge*) malloc((MAX_NODES-1) * sizeof(int));
        int spanning_tree_size = 0;

        /*Set that will hold all edges in graph sorted by weight*/
        Edge *sorted_edges = (Edge*)  malloc(MAX_NODES*(MAX_NODES-1) * sizeof(int));
        int sorted_edges_size = 0;

        /*Existing amount of cable (already connected)*/
        double existing = 0;

        /*Use algorithm*/
        kruskal(graph, num_nodes, sorted_edges, &sorted_edges_size, spanning_tree, &spanning_tree_size, node_xy, &existing);

        /*Count total distance and print it*/
        double total = 0;
        for (int i=0; i<spanning_tree_size; i++){
            total += spanning_tree[i].distance;
        }
        printf("%.2lf\n", total-existing);

        /*Free allocated memory*/
        free(spanning_tree);
        free(sorted_edges);
    }

    return 0;
}