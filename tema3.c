#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    struct Node *next;
    int number;
} node;

node *init_node() {
    node *new = (node *)malloc(sizeof(node));
    new->number = 0;
    return new;
}

typedef struct Graph {
    node *adj_list;
} graph;

int number_of_atraction(char **name, char *s, int n) {
    for (int i = 0; i < n; ++i) {
        if (strcmp(name[i], s) == 0)  // daca exista deja
            return i;
    }
    return -1;
}

void dfs(graph *g, int index, int vis[]) {
    vis[index] = 1;
    //printf("%d ", index);
    node *aux = g[index].adj_list;
    for (int i = 0; i < g[index].adj_list->number; ++i) {
        aux = aux -> next;
        if (vis[aux->number] == 0) {
            //printf("aux-NUmber %d \n", aux->number);
            dfs(g, aux->number, vis);
        }
    }
}
int main(int argc, char const *argv[]) {
    FILE *in = fopen("tema3.in", "r");
    FILE *out = fopen("tema3.out", "w");

    // n = nr de obiective
    // m = nr de drumuri
    int n, m;

    fscanf(in, "%d %d\n", &n, &m);
    //fprintf(out, "%d %d\n", n, m);

    // name[i] reprezinta stringul atractiei cu nr i
    char **name = (char **)malloc(n * sizeof(char *));
    char atraction[100];
    int temp_n = 0, cost, index1, index2;
    graph *g = (graph *)malloc(
        n * sizeof(graph));  // nr de liste de adiacenta cu santinela

    // cream listele de adiacenta cu santinela avand numarul de muchii ce pleaca
    // din i
    for (int i = 0; i < n; ++i) g[i].adj_list = init_node();

    // retinem intr-un nod auxiliar inceputul listei la care adaugam elemente
    node *aux;

    for (int i = 0; i < m; ++i) {
        fscanf(in, "%s ", atraction);
        index1 = number_of_atraction(name, atraction, temp_n);
        if (index1 == -1) {
            // punem noua atractie;
            name[temp_n] =
                (char *)malloc((strlen(atraction) + 1) * sizeof(char));
            strcpy(name[temp_n], atraction);
            // indexul de acum
            index1 = temp_n;
            // crestem nr de atractii citite pana atunci
            temp_n++;
        }

        fscanf(in, "%s ", atraction);
        index2 = number_of_atraction(name, atraction, temp_n);
        if (index2 == -1) {
            // punem noua atractie;
            name[temp_n] =
                (char *)malloc((strlen(atraction) + 1) * sizeof(char));
            strcpy(name[temp_n], atraction);
            // index2
            index2 = temp_n;
            // crestem nr de atractii citite pana atunci
            temp_n++;
        }

        // pentru muchia index1 -> index2
        aux = g[index1].adj_list;
        for (int j = 0; j < g[index1].adj_list->number; ++j) {
            aux = aux->next;
        }
        // aux->next va fi NULL
        // // alocam pentru urmatorul de dupa ultimul element din lista(practic
        // ala de pe NULL)
        aux->next = init_node();
        aux->next->number = index2;
        (g[index1].adj_list->number)++;

        // pentru muchia index2 -> index1
        aux = g[index2].adj_list;
        for (int j = 0; j < g[index2].adj_list->number; ++j) {
            aux = aux->next;
        }
        // aux->next va fi NULL
        // // alocam pentru urmatorul de dupa ultimul element din lista(practic
        // ala de pe NULL)
        aux->next = init_node();
        aux->next->number = index1;
        (g[index2].adj_list->number)++;
        fscanf(in, "%d\n", &cost);
    }

    // declaram si initializam vectorul de vizitati
    int *vis = (int *)calloc(n, sizeof(int));

    // facem dfs pe fiecare nod nevizitat
    int nr_conex = 0;
    for (int i = 0; i < n; ++i) {
        if (vis[i] == 0) {
            nr_conex++;
            //printf("dfs: ");
            dfs(g, i, vis);
            //printf("\n");
        }
    }
    fprintf(out, "%d\n", nr_conex);

    //for (int i = 0; i < n; ++i) fprintf(out, "%d %s\n", i, name[i]);

    // for (int i = 0; i < n; ++i) {
    //     printf("%d g[i].adj: %d\n", i, g[i].adj_list->number);
    // }

    // afisam listele de adiacenta
    // for (int i = 0; i < n; ++i) {
    //     // printf("%d: vecini ", i);
    //     aux = g[i].adj_list;
    //     for (int j = 0; j < g[i].adj_list->number; ++j) {
    //         aux = aux->next;
    //         printf("%d ", aux->number);
    //     }
    //     printf("\n");
    // }


    // printf("vis: ");
    // for(int i = 0; i < n; ++i)
    //     printf("%d ", vis[i]);

    // printf("\n");








    // eliberarea memoriei
    for (int i = 0; i < n; ++i) {
        free(name[i]);
    }
    free(name);

    // mai ai de eliberat memoria pentru graph

    fclose(in);
    fclose(out);

    return 0;
}
