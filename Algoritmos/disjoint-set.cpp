#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/******************************************************************************************
 *  All functions adapted from: https://en.wikipedia.org/wiki/Disjoint-set_data_structure
 *  
 ******************************************************************************************/






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







/******************************************************************
 * Finds the root element of a given node's set.
 *  Follows the chain of parent pointers from x up the tree until 
 *  it reaches a root element, whose parent is itself.
 * 
 * Halving: Makes every other node on the path point to its 
 *  grandparent.
 * 
 * Parameters:
 *      parent_set[]: Parent nodes;
 *      x: Node to evaluate.
 * 
 * Returns:
 *      Root element of x's set.
 * 
 ******************************************************************/
int find_halve(int parent_set[], int x){
    while (parent_set[x] != x){
        parent_set[x] = parent_set[parent_set[x]];
        x = parent_set[x];
    }

    return x;
}







/******************************************************************
 * Finds the root element of a given node's set.
 *  Follows the chain of parent pointers from x up the tree until 
 *  it reaches a root element, whose parent is itself.
 * 
 * Splitting: Makes every node on the path point to its grandparent.
 * 
 * Parameters:
 *      parent_set[]: Parent nodes;
 *      x: Node to evaluate.
 * 
 * Returns:
 *      Root element of x's set.
 * 
 ******************************************************************/
int find_split(int parent_set[], int x){
    while (parent_set[x] != x){
        int next = parent_set[x];
        parent_set[x] = parent_set[next];
        x = next;
    }

    return x;
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







/*******************************************************************************
 * Union uses Find to determine the roots of the trees x and y belong to. 
 *  If the roots are distinct, the trees are combined by attaching the root 
 *  of one to the root of the other.
 * 
 * By size: Always attaches the tree with fewer elements to the root of the 
 *  tree having more elements.
 * 
 * Parameters:
 *      parent_set[]: Parent nodes;
 *      size_set[]: Set containing sizes (number of elements) of each node;
 *      x: First node's set;
 *      y: Second node's set.
 * 
 *******************************************************************************/ 
void union_size(int parent_set[], int size_set[], int x, int y){
    int xRoot = find_compress(parent_set, x);
    int yRoot = find_compress(parent_set, y);

    /* x and y are already in the same set */
    if (xRoot == yRoot) return;

    /* x and y are not in same set, so we merge them */ 
    if (size_set[xRoot] < size_set[yRoot]){
        int temp = xRoot;
        xRoot = yRoot;
        yRoot = temp;
    }

    /*Merge yRoot into xRoot*/
    parent_set[yRoot] = xRoot;
    size_set[xRoot] = size_set[xRoot] + size_set[yRoot];
}