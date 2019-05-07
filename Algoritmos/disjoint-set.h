#ifndef DISJOINTSET_H
#define DISJOINTSET_H

void make_set(int parent_set[], int rank_set[], int size_set[], int num_nodes);

int find_compress(int parent_set[], int x);
int find_halve(int parent_set[], int x);
int find_split(int parent_set[], int x);

void union_rank(int parent_set[], int rank_set[], int x, int y);
void union_size(int parent_set[], int size_set[], int x, int y);


#endif