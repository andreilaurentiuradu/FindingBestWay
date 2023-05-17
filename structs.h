#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    struct Node *next;
    int number, cost;
} node;

typedef struct Graph {
    node *adj_list;
} graph;

typedef struct Edge {
    int cost, start, stop, used;
} edge;
