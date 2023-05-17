#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

node *init_node() {
    node *new = (node *)malloc(sizeof(node));
    new->next = NULL;
    new->number = 0;
    new->cost = 0;
    return new;
}

int number_of_atraction(char **name, char *s, int n) {
    for (int i = 0; i < n; ++i) {
        if (strcmp(name[i], s) == 0)  // daca exista deja
            return i;
    }
    return -1;
}

void dfs(graph *g, int index, int vis[], int nr_conex) {
    vis[index] = nr_conex;
    //printf("%d ", index);
    node *aux = g[index].adj_list;
    for (int i = 0; i < g[index].adj_list->number; ++i) {
        aux = aux -> next;
        if (vis[aux->number] == 0) {
            //printf("aux-NUmber %d \n", aux->number);
            dfs(g, aux->number, vis, nr_conex);
        }
    }
}

void delete_list(node * start) {
    if(start != NULL) {
        delete_list(start->next);
        free(start);
    }
}
/*void delete_list(graph *g, int index, node * start) {
    if(g[index].adj_list->number != 0) {
        delete_list(g, index, start->next);
        g[index].adj_list->number--;
        free(start);
    }
    // mai ai la final de eliberat pentru ultimul element
}*/


int root(int index, int father[]) {
    while (index != father[index]) index = father[index];
    return index;
}

void link(int a, int b, int father[]) {
    father[a] = b;
}

int cmp_edge(const void *a, const void *b) {
    return (((edge *)a)->cost - ((edge *)b)->cost);
}

int cmp(const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}
