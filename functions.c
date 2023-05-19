/*RADU Andrei-Laurentiu - 312CC*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#define INFINIT 1000000000

// alocam memorie si initializam un nod din listele de adiacenta
// ce vor retine graful
place *init_place(int index, int cost) {
    place *new = (place *)malloc(sizeof(place));
    new->next = NULL;
    new->index = index;
    new->cost = cost;
    return new;
}

// vericam daca am asociat un index unei locatii
// in caz afirmativ returnam indexul asociat
int index_of_location(char **name, char *s, int n) {
    for (int i = 0; i < n; ++i) {
        if (strcmp(name[i], s) == 0) return i;
    }
    return -1;
}

// facem o parcurgere in adancime pe graful ce reprezinta harta

void dfs(map *g, int index, int vis[], int nr_conex) {
    // marcam in vectorul de vizitati componenta din care face parte nodul
    vis[index] = nr_conex;

    // folosim un nod auxiliar pentru a parcurge lista vecinilor nodului curent
    place *aux = g[index].adj_list;
    for (int i = 0; i < g[index].adj_list->index; ++i) {
        aux = aux->next;

        // daca nodul nu a fost vizitat facem autoapelul
        if (vis[aux->index] == 0) {
            dfs(g, aux->index, vis, nr_conex);
        }
    }
}

// eliberam memoria ocupata de o lista primind ca parametru capul ei
void delete_list(place *start) {
    if (start != NULL) {
        delete_list(start->next);
        free(start);
    }
}

// aflam varful arborelui ce contine un nod dat(index)
int root(int index, int father[]) {
    while (index != father[index]) index = father[index];
    return index;
}

// unim 2 arbori
void link(int a, int b, int father[]) { father[a] = b; }

// comparam costul a doua muchii
int cmp_way(const void *a, const void *b) {
    return (((way *)a)->cost - ((way *)b)->cost);
}

// comparam 2 numare intregi
int cmp(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

// adaugam o locatie in containerul name
void add_location(char **name, int index, char *location) {
    name[index] = (char *)malloc((strlen(location) + 1) * sizeof(char));
    strcpy(name[index], location);
}

// interschimbam doua valori intregi
void swap(int *a, int *b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

// sortam nodurile din container, descrescator, pe baza scorului
// astfel incat elementul extras sa fie cel cu scorul minim
void selection_sort(int index[], int n, float score[]) {
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            // daca avem un scor mai mic la dreapta
            if (score[index[i]] < score[index[j]]) swap(&index[i], &index[j]);
        }
    }
}

// aplicam algoritmul lui Dijkstra pentru a determina drumul optim pe care
// trebuie sa il parcurga echipajul de la corabie la insula
void dijkstra(FILE *out, char **name, map *g, int depths[], int n, int start,
              int stop, int weight) {
    // containerul cu nodurile pentru care s-a gasit un scor mai bun
    int *better_score = (int *)calloc(n, sizeof(int));

    // vectorul caracteristic in care marcam daca elementul a fost adaugat in
    // containerul anterior
    int *in_better_score = (int *)calloc(n, sizeof(int));

    // pozitia in container
    int pos_better_score = 0;

    // vectorul de distante
    int *dist = (int *)malloc(n * sizeof(int));

    // vectorul de "tati"
    int *father = (int *)malloc(n * sizeof(int));

    // vectorul de "fii"
    int *son = (int *)malloc(n * sizeof(int));

    // vectorul de scoruri de la start pana la fiecare nod
    float *score = (float *)malloc(n * sizeof(float));

    // initializam vectorul de "tati" astfel incat elementul i sa il aiba ca
    // "tata" tot pe i
    for (int i = 0; i < n; ++i) {
        father[i] = i;
    }

    // initializam vectorii de distante si scor cu o valoare suficient de mare
    for (int i = 0; i < n; ++i) {
        dist[i] = INFINIT;
        score[i] = INFINIT;
    }

    // adaugam nodul in contai er
    better_score[pos_better_score] = start;

    // actualizam distanta(de la start la start)
    dist[start] = 0;

    // marcam nodul ca fiind in container
    in_better_score[start] = 1;

    // initializam scorul de la start la start
    score[start] = 0;

    int current;
    place *neigh;
    float actual_score;

    // cat timp existe elemente "nescoase" din container
    while (pos_better_score != -1) {
        // retinem nodul cu scorul cel mai mic
        current = better_score[pos_better_score];

        // actualizam pozitia nodului cu cel mai bun scor
        pos_better_score--;

        // marcam nodul current ca fiind "scos" din container
        in_better_score[current] = 0;

        // retinem primul nod al listei nodului curent(cel care contine nr de
        // vecini)
        neigh = g[current].adj_list;

        // parcurgem lista de adiacenta
        for (int i = 0; i < g[current].adj_list->index; ++i) {
            // trecem la nodul urmator
            neigh = neigh->next;

            // calculam scorul muchiei de la nodul curent la nodul vecin
            actual_score = ((float)(neigh->cost)) / depths[neigh->index];

            // daca am gasit un drum cu scor mai bun pentru calea start->vecin
            if (score[neigh->index] > score[current] + actual_score) {
                // actualizam scorul
                score[neigh->index] = score[current] + actual_score;

                // actualizam distanta
                dist[neigh->index] = dist[current] + neigh->cost;

                // "tatal" vecinului devine nodul curent
                father[neigh->index] = current;

                // daca nodul nu se afla in container
                if (in_better_score[neigh->index] == 0) {
                    // il marcam
                    in_better_score[neigh->index] = 1;

                    // actualizam pozitia
                    pos_better_score++;

                    // il punem in container
                    better_score[pos_better_score] = neigh->index;

                    // sortam containerul pe baza scorului
                    selection_sort(better_score, pos_better_score + 1, score);
                }
            }
        }
    }

    // salvam distanta de la corabie la insula cu scorul optim
    int total_cost = dist[stop];

    // initializam "fiul" ultimului nod din drum cu -1
    son[stop] = -1;

    // pe baza vectorului de "tati" realizam vectorul de "fii"
    while (stop != start) {
        son[father[stop]] = stop;
        stop = father[stop];
    }

    // initializam adancimea maxima cu o valoare suficient de mare
    int depthest = INFINIT;

    // afisam prima locatie din drum(Corabia)
    fprintf(out, "%s ", name[start]);

    // trecem la urmatoarea locatie
    start = son[start];

    // cati timp nu am ajuns la destinatie
    while (son[start] != -1) {
        // verificam daca am gasit o adancime mai mare
        if (depthest > depths[start]) depthest = depths[start];

        // afisam urmatoarea locatie
        fprintf(out, "%s ", name[start]);

        // trecem la urmatoarea locatie
        start = son[start];
    }

    // afisam ultima locatie(Insula), costul drumului, adancimea maxima si
    // numarul minim de drumuri pentru a transporta toata comoara
    fprintf(out, " %s\n", name[start]);
    fprintf(out, "%d\n", total_cost);
    fprintf(out, "%d\n", depthest);
    fprintf(out, "%d", weight / depthest);

    // eliberam memoria ocupata de containere
    free(score);
    free(father);
    free(son);
    free(better_score);
    free(dist);
    free(in_better_score);
}
