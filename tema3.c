/*RADU Andrei-Laurentiu - 312CC*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#define INFINIT 1000000000

// semnaturile functiilor implementate in functions.c
place *init_place(int index, int cost);
int index_of_location(char **name, char *s, int n);
void dfs(map *g, int index, int vis[], int nr_conex);
void delete_list(place *start);
int root(int index, int father[]);
void link(int a, int b, int father[]);
int cmp_way(const void *a, const void *b);
int cmp(const void *a, const void *b);
void add_location(char **name, int index, char *location);
void swap(int *a, int *b);
void selection_sort(int index[], int n, int dist[], int depths[]);
void dijkstra(FILE *out, char **name, map *g, int depths[], int n, int start,
              int stop, int weight);

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

    // decalaram un vector in care vom citi numele locatiilor
    char location[100];

    // costul fiecarei muchii/ distanta
    // numarul de locatii citite pana intr-un anumit moment si 2 variabile ce
    // vor retine indecsii locatiilor pentru o muchie citita
    int cost, temp_n = 0, index1, index2;

    // structura ce va retine graful(un vector de liste)
    map *g = (map *)malloc(n * sizeof(map));

    // vectorul de muchii,utilizat pentru kruskal (asa ca nu punem muchiile in
    // ambele sensuri)
    way *e = (way *)malloc(m * sizeof(way));

    // cream listele de adiacenta cu santinela
    // avand numarul de muchii ce pleaca din i
    for (int i = 0; i < n; ++i) g[i].adj_list = init_place(0, 0);

    // retinem intr-un nod auxiliar inceputul listei
    // la care adaugam elemente
    place *aux;

    // daca cerinta de rezolvat este 1
    if (strcmp(argv[1], "1") == 0) {
        // citim elementele muchiilor
        for (int i = 0; i < m; ++i) {
            // prima locatie a muchiei
            fscanf(in, "%s ", location);

            // indexul ei
            index1 = index_of_location(name, location, temp_n);

            // daca nu a fost inca adaugata in container
            if (index1 == -1) {
                // actualizam indexul
                index1 = temp_n;

                // adaugam locatia
                add_location(name, index1, location);

                // crestem nr de locatii citite pana atunci
                temp_n++;
            }

            // a doua locatie
            fscanf(in, "%s ", location);

            // costul muchiei dintre cele doua locatii
            fscanf(in, "%d\n", &cost);

            // indexul locatiei 2
            index2 = index_of_location(name, location, temp_n);

            // daca nu a fost inca adaugata in container
            if (index2 == -1) {
                // actualizam indexul
                index2 = temp_n;

                // adaugam locatia
                add_location(name, index2, location);

                // crestem nr de locatii citite pana atunci
                temp_n++;
            }

            // pentru muchia index1 -> index2
            aux = g[index1].adj_list;
            for (int j = 0; j < g[index1].adj_list->index; ++j) {
                aux = aux->next;
            }

            // adaugam in lista nodul vecin
            aux->next = init_place(index2, cost);
            (g[index1].adj_list->index)++;

            // pentru muchia index2 -> index1
            aux = g[index2].adj_list;
            for (int j = 0; j < g[index2].adj_list->index; ++j) {
                aux = aux->next;
            }

            // procedam analog pentru celalta locatie
            aux->next = init_place(index1, cost);
            (g[index2].adj_list->index)++;

            // adaugam muchia in vector
            e[i].start = index1;
            e[i].stop = index2;
            e[i].cost = cost;
        }

        // nr de componente conexe
        int nr_conex = 0;

        for (int i = 0; i < n; ++i) {
            // gasim un nod nevizitat
            if (vis[i] == 0) {
                // crestem nr de componente conexe
                nr_conex++;
                // facem dfs pe fiecare nod nevizitat
                dfs(g, i, vis, nr_conex);
            }
        }

        // cerinta 1 a numarul de componente conexe
        fprintf(out, "%d\n", nr_conex);

        // sortam muchiile dupa cost
        qsort(e, m, sizeof(way), cmp_way);

        // aplicam algoritmul lui kruskal
        // avem nevoie de un vector de tati pentru a crea arborele partial de
        // cost minim
        int *father = (int *)malloc(n * sizeof(int));

        // initializam fiecare padure disjuncta ca fiind propriul tata
        for (int i = 0; i < n; ++i) father[i] = i;

        int root1, root2;
        // vector care va retine costul fiecarei arbore partial de cost minim
        // determinat pentru fiecare componenta
        int *cost_comp = (int *)calloc(nr_conex, sizeof(int));

        // parcurgem vectorul de muchii
        for (int i = 0; i < m; ++i) {
            // luam radacinile arborilor din care fac parte cele 2 noduri
            root1 = root(e[i].start, father);
            root2 = root(e[i].stop, father);

            // daca sunt din arbori diferiti
            if (root1 != root2) {
                // ii unim
                link(root1, root2, father);
                // actualizam costul componentei
                cost_comp[vis[e[i].start] - 1] += e[i].cost;
            }
        }

        // sortam crescator costurile si le afisam
        qsort(cost_comp, nr_conex, sizeof(int), cmp);
        for (int i = 0; i < nr_conex; ++i) fprintf(out, "%d\n", cost_comp[i]);

        // eliberam memoria
        free(cost_comp);
        free(father);
    }

    // daca cerinta de rezolvat este 2
    if (strcmp(argv[1], "2") == 0) {
        // depths[i] reprezinta adancimea in locatia i
        int *depths = (int *)malloc(n * sizeof(int));

        // citim muchiile
        for (int i = 0; i < m; ++i) {
            // citim primul nod(locatia)
            fscanf(in, "%s ", location);

            // determinam indexul
            index1 = index_of_location(name, location, temp_n);

            // daca nu a fost adaugat
            if (index1 == -1) {
                // actualizam indexul
                index1 = temp_n;

                // adugam locatia
                add_location(name, index1, location);

                // crestem nr de locatii citite pana atunci
                temp_n++;
            }

            // citim al doilea nod(locatie) si distanta
            fscanf(in, "%s ", location);
            fscanf(in, "%d\n", &cost);

            // determinam indexul
            index2 = index_of_location(name, location, temp_n);

            // daca nu a fost adaugata
            if (index2 == -1) {
                // actualizam indexul
                index2 = temp_n;

                // adaugam locatia
                add_location(name, index2, location);

                // crestem nr de atractii citite pana atunci
                temp_n++;
            }

            // pentru muchia index1 -> index2
            aux = g[index1].adj_list;
            for (int j = 0; j < g[index1].adj_list->index; ++j) {
                aux = aux->next;
            }

            // adaugam noul vecin
            aux->next = init_place(index2, cost);
            (g[index1].adj_list->index)++;

            // adaugam noua muchie
            e[i].start = index1;
            e[i].stop = index2;
            e[i].cost = cost;
        }

        // citim locatiile si adaugam in vectorul de adancimi la pozitia
        // corespunzatoare
        for (int i = 0; i < n; ++i) {
            fscanf(in, "%s", location);
            index1 = index_of_location(name, location, n);
            fscanf(in, "%d\n", &depths[index1]);
        }

        // greutatea comorii
        int weight;
        fscanf(in, "%d", &weight);

        // facem dfs pentru locatiile de start si stop
        int island_index = index_of_location(name, "Insula", n);
        int ship_index = index_of_location(name, "Corabie", n);

        // verificam daca putem ajunge cu un dfs de la corabie la insula si
        // reciproc

        int ok = 1;
        dfs(g, ship_index, vis, 1);
        if (vis[ship_index] != vis[island_index]) {
            ok = 0;
            fprintf(out, "Echipajul nu poate ajunge la insula\n");
        }
        memset(vis, 0, sizeof(int));  // resetam vectorul de vizitati
        dfs(g, island_index, vis, 1);
        if (vis[island_index] != vis[ship_index]) {
            ok = 0;
            fprintf(
                out,
                "Echipajul nu poate transporta comoara inapoi la corabie\n");
        }

        // daca se poate ajunge in ambele sensuri
        if (ok) {
            // aplicam dijkstra pe baza scorului
            dijkstra(out, name, g, depths, n,
                    island_index, ship_index, weight);
        }
        // eliberarea memoriei
        free(depths);  // vectorul de adancimi
    }

    // eliberam memoria pentru containerele comune ambelor cerinte
    for (int i = 0; i < n; ++i) {
        free(name[i]);
    }
    free(name);

    for (int i = 0; i < n; ++i) {
        delete_list(g[i].adj_list);
    }

    free(g);
    free(vis);
    free(e);

    // inchidem fisierele
    fclose(in);
    fclose(out);

    return 0;
}
