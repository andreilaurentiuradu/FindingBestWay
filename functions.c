#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#define INFINIT 1000000000

node *init_node(int index, int cost) {
    node *new = (node *)malloc(sizeof(node));
    new->next = NULL;
    new->index = index;
    new->cost = cost;
    return new;
}

int index_of_location(char **name, char *s, int n) {
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
    for (int i = 0; i < g[index].adj_list->index; ++i) {
        aux = aux -> next;
        if (vis[aux->index] == 0) {
            //printf("aux-index %d \n", aux->index);
            dfs(g, aux->index, vis, nr_conex);
        }
    }
}

void delete_list(node * start) {
    if(start != NULL) {
        delete_list(start->next);
        free(start);
    }
}



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

void add_location(char **name, int index, char *location) {
    // punem noua atractie;
                name[index] =
                    (char *)malloc((strlen(location) + 1) * sizeof(char));
                strcpy(name[index], location);

}

void swap(int *a, int *b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

void selection_sort(int index[], int n, float score[]) {
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (score[index[i]] > score[index[j]])
                swap(&index[i], &index[j]);
        }
    }
}

void dijkstra(FILE *out, char **name, graph *g, int depths[], int n, int start, int stop, int weight) {
    int *in_queue = (int *)calloc(n, sizeof(int));
    int *queue = (int *)calloc(n, sizeof(int));
    int pos_queue = 0;
    int *dist = (int *)malloc(n * sizeof(int));
    int *father = (int *)malloc(n * sizeof(int));
    int *son = (int *)malloc(n * sizeof(int));
    float *score = (float *)malloc(n * sizeof(float));
    for(int i = 0; i < n; ++i) {
        father[i] = i;
    }

    for (int i = 0; i < n; ++i) {
        dist[i] = INFINIT;
        score[i] = INFINIT;
    }
    // punem nodul de start in coada si actualizam distanta
    dist[start] = 0;
    queue[pos_queue] = start;
    in_queue[start] = 1;
    score[start] = 0;
    int current;
    node *neigh;
    while (pos_queue != -1) {
        // scoatem un nod din coada
        current = queue[pos_queue];
        pos_queue--;
        in_queue[current] = 0;
        neigh = g[current].adj_list;

        for (int i = 0; i < g[current].adj_list->index; ++i) {
            neigh = neigh->next;
            float actual_score =
                ((float)(neigh->cost)) / depths[neigh->index];
            if (score[neigh->index] > score[current] + actual_score) {
                score[neigh->index] = score[current] + actual_score;
                // actualizam distanta
                dist[neigh->index] = dist[current] + neigh->cost;
                father[neigh->index] = current;
                // daca nu e in coada
                if (in_queue[neigh->index] == 0) {
                    in_queue[neigh->index] = 1;  // il adaugam
                    pos_queue++;
                    queue[pos_queue] = neigh->index;
                    selection_sort(queue, pos_queue + 1, score);
                }
            }
        }
    }

    //for (int i = 0; i < n; ++i) printf("name: %s %d\n", name[i], dist[i]);
    //printf("\n");
    int total_cost = dist[stop];
    // printf("total_cost %d\n", total_cost);

    son[stop] = -1;
    while(stop != start){
        // printf("index %d\n", stop);
        son[father[stop]] = stop;
        stop = father[stop];
    }

    // printf("\n");
    int depthest = INFINIT;

    fprintf(out, "%s ", name[start]);
    start = son[start];

    while(son[start] != -1){
        if(depthest > depths[start])
            depthest = depths[start];
        fprintf(out, "%s ", name[start]);
        start = son[start];
    }
    fprintf(out, " %s\n", name[start]);
    fprintf(out, "%d\n", total_cost);
    fprintf(out, "%d\n", depthest);
    fprintf(out, "%d", weight / depthest);

    free(score);
    free(father);
    free(son);
    free(queue);
    free(dist);
    free(in_queue);
}
