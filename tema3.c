#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

node *init_node(int number, int cost);
int number_of_atraction(char **name, char *s, int n);
void dfs(graph *g, int index, int vis[], int nr_conex);
void delete_list(node *start);
int root(int index, int father[]);
void link(int a, int b, int father[]);
int cmp_edge(const void *a, const void *b);
int cmp(const void *a, const void *b);
void add_location(char **name, int index, char *atraction);

int main(int argc, char const *argv[]) {
    FILE *in = fopen("tema3.in", "r");
    FILE *out = fopen("tema3.out", "w");

    // n = nr de locatii
    // m = nr de drumuri
    int n, m;

    fscanf(in, "%d %d\n", &n, &m);

    // name[i] reprezinta stringul atractiei cu nr i
    char **name = (char **)malloc(n * sizeof(char *));

    char atraction[100];
    int cost;  // il facem float pt cerinta 2
    int temp_n = 0, index1, index2;
    graph *g = (graph *)malloc(
        n * sizeof(graph));  // nr de liste de adiacenta cu santinela

    // cream listele de adiacenta cu santinela avand numarul de muchii ce
    // pleaca din i
    for (int i = 0; i < n; ++i) g[i].adj_list = init_node(0, 0);

    // retinem intr-un nod auxiliar inceputul listei la care adaugam
    // elemente
    node *aux;

    if (strcmp(argv[1], "1") == 0) {
        edge *e = (edge *)malloc(m * sizeof(edge));

        for (int i = 0; i < m; ++i) {
            fscanf(in, "%s ", atraction);
            index1 = number_of_atraction(name, atraction, temp_n);
            if (index1 == -1) {
                // indexul de acum
                index1 = temp_n;
                add_location(name, index1, atraction);
                // crestem nr de atractii citite pana atunci
                temp_n++;
            }

            fscanf(in, "%s ", atraction);
            fscanf(in, "%d\n", &cost);

            index2 = number_of_atraction(name, atraction, temp_n);
            if (index2 == -1) {
                // index2
                index2 = temp_n;
                add_location(name, index2, atraction);
                // crestem nr de atractii citite pana atunci
                temp_n++;
            }

            // pentru muchia index1 -> index2
            aux = g[index1].adj_list;
            for (int j = 0; j < g[index1].adj_list->number; ++j) {
                aux = aux->next;
            }
            // aux->next va fi NULL
            // // alocam pentru urmatorul de dupa ultimul element din
            // lista(practic ala de pe NULL)
            aux->next = init_node(index2, cost);
            (g[index1].adj_list->number)++;

            // pentru muchia index2 -> index1
            aux = g[index2].adj_list;
            for (int j = 0; j < g[index2].adj_list->number; ++j) {
                aux = aux->next;
            }
            // aux->next va fi NULL
            // // alocam pentru urmatorul de dupa ultimul element din
            // lista(practic ala de pe NULL)
            aux->next = init_node(index1, cost);
            (g[index2].adj_list->number)++;

            e[i].start = index1;
            e[i].stop = index2;
            e[i].cost = cost;
        }

        // declaram si initializam vectorul de vizitati
        int *vis = (int *)calloc(n, sizeof(int));

        // facem dfs pe fiecare nod nevizitat
        int nr_conex = 0;
        for (int i = 0; i < n; ++i) {
            if (vis[i] == 0) {
                nr_conex++;
                // printf("dfs: ");
                dfs(g, i, vis, nr_conex);
                // printf("\n");
            }
        }

        // cerinta 1 a
        fprintf(out, "%d\n", nr_conex);

        // cerinta 1 b
        // sortam muchiile dupa cost

        qsort(e, m, sizeof(edge), cmp_edge);

        // kruskal
        // avem nevoie de un vector de tati pentru a crea arborele partial de
        // cost minim
        int *father = (int *)malloc(n * sizeof(int));

        // for(int i = 0; i < n; ++i)
        //     printf("%d ", vis[i]);
        // printf("\n");
        // initializam fiecare padure disjuncta ca fiind propriul tata
        for (int i = 0; i < n; ++i) father[i] = i;

        int root1, root2;
        int *cost_comp = (int *)calloc(nr_conex, sizeof(int));
        for (int i = 0; i < m; ++i) {
            root1 = root(e[i].start, father);
            root2 = root(e[i].stop, father);
            if (root1 != root2) {
                link(root1, root2, father);
                cost_comp[vis[e[i].start] - 1] += e[i].cost;
            }
        }

        qsort(cost_comp, nr_conex, sizeof(int), cmp);
        for (int i = 0; i < nr_conex; ++i) fprintf(out, "%d\n", cost_comp[i]);

        // eliberarea memoriei

        free(cost_comp);  // costul fiecarei componente conexe
        free(e);          // vectorul de muchii
        free(vis);        // vectorul de vizitati
        free(father);     // vectorul de tati
    }

    if (strcmp(argv[1], "2") == 0) {
        // depths[i] reprezinta adancimea in locatia i
        int *depths = (int *)malloc(n * sizeof(int));

        for (int i = 0; i < m; ++i) {
            fscanf(in, "%s ", atraction);
            index1 = number_of_atraction(name, atraction, temp_n);
            if (index1 == -1) {
                // indexul de acum
                index1 = temp_n;
                add_location(name, index1, atraction);
                // crestem nr de atractii citite pana atunci
                temp_n++;
            }

            fscanf(in, "%s ", atraction);
            fscanf(in, "%d\n", &cost);

            index2 = number_of_atraction(name, atraction, temp_n);
            if (index2 == -1) {
                // index2
                index2 = temp_n;
                add_location(name, index2, atraction);
                // crestem nr de atractii citite pana atunci
                temp_n++;
            }

            // pentru muchia index1 -> index2
            aux = g[index1].adj_list;
            for (int j = 0; j < g[index1].adj_list->number; ++j) {
                aux = aux->next;
            }
            // aux->next va fi NULL
            // // alocam pentru urmatorul de dupa ultimul element din
            // lista(practic ala de pe NULL)
            aux->next = init_node(index2, cost);
            (g[index1].adj_list->number)++;
        }


        // eliberarea memoriei
        free(depths);  // vectorul de adancimi

        }

    // eliberam memoria de la nume
    for (int i = 0; i < n; ++i) {
        free(name[i]);
    }
    free(name);

    // mai ai de eliberat memoria pentru graph
        for (int i = 0; i < n; ++i) {
            delete_list(g[i].adj_list);
        }

        free(g);       // vectorul de liste de adiacenta


    fclose(in);
    fclose(out);

    return 0;
}
