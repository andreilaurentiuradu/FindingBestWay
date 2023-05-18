#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#define INFINIT 1000000000

node *init_node(int number, int cost);
int number_of_location(char **name, char *s, int n);
void dfs(graph *g, int index, int vis[], int nr_conex);
void delete_list(node *start);
int root(int index, int father[]);
void link(int a, int b, int father[]);
int cmp_edge(const void *a, const void *b);
int cmp(const void *a, const void *b);
void add_location(char **name, int index, char *location);

int dijkstra(graph *g, int start, int vis[]) { return 0; }

int main(int argc, char const *argv[]) {
    FILE *in = fopen("tema3.in", "r");
    FILE *out = fopen("tema3.out", "w");

    // n = nr de locatii
    // m = nr de drumuri
    int n, m;

    fscanf(in, "%d %d\n", &n, &m);

    // declaram si initializam vectorul de vizitati
    int *vis = (int *)calloc(n, sizeof(int));

    // name[i] reprezinta stringul atractiei cu nr i
    char **name = (char **)malloc(n * sizeof(char *));

    char location[100];
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
            fscanf(in, "%s ", location);
            index1 = number_of_location(name, location, temp_n);
            if (index1 == -1) {
                // indexul de acum
                index1 = temp_n;
                add_location(name, index1, location);
                // crestem nr de atractii citite pana atunci
                temp_n++;
            }

            fscanf(in, "%s ", location);
            fscanf(in, "%d\n", &cost);

            index2 = number_of_location(name, location, temp_n);
            if (index2 == -1) {
                // index2
                index2 = temp_n;
                add_location(name, index2, location);
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
        free(father);     // vectorul de tati
    }

    if (strcmp(argv[1], "2") == 0) {
        // depths[i] reprezinta adancimea in locatia i
        int *depths = (int *)malloc(n * sizeof(int));

        for (int i = 0; i < m; ++i) {
            fscanf(in, "%s ", location);
            index1 = number_of_location(name, location, temp_n);
            if (index1 == -1) {
                // indexul de acum
                index1 = temp_n;
                add_location(name, index1, location);
                // crestem nr de atractii citite pana atunci
                temp_n++;
            }

            fscanf(in, "%s ", location);
            fscanf(in, "%d\n", &cost);

            index2 = number_of_location(name, location, temp_n);
            if (index2 == -1) {
                // index2
                index2 = temp_n;
                add_location(name, index2, location);
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

        for (int i = 0; i < n; ++i) {
            fscanf(in, "%s", location);
            index1 = number_of_location(name, location, n);
            fscanf(in, "%d\n", &depths[index1]);
            // printf("index %d, location %s, depths %d\n", index1, location,
            // depths[index1]);
        }

        int weight;
        fscanf(in, "%d", &weight);

        // facem dfs pe fiecare nod nevizitat
        int island_index = number_of_location(name, "Insula", n);
        int ship_index = number_of_location(name, "Corabie", n);

        // verificam daca putem ajunge cu un dfs de la corabie la insula si reciproc
        dfs(g, ship_index, vis, 1);
        if(vis[ship_index] != vis[island_index])
            fprintf(out, "Echipajul nu poate ajunge la insula\n");

        memset(vis, 0, sizeof(int));
        dfs(g, island_index, vis, 1);
        if(vis[island_index] != vis[ship_index])
            fprintf(out, "Echipajul nu poate transporta comoara inapoi la corabie\n");

        // for (int i = 0; i < n; ++i) {
        //     if (vis[i] == 0) {
        //         nr_conex++;
        //         // printf("dfs: ");
        //         dfs(g, i, vis, nr_conex);
        //         // printf("\n");
        //     }
        // }
        // aplicam dijkstra pe baza scorului
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

    free(g);  // vectorul de liste de adiacenta
    free(vis);        // vectorul de vizitati

    fclose(in);
    fclose(out);

    return 0;
}
