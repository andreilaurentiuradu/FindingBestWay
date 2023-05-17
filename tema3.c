#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node {
    struct Node *next;
    int number;
} node, *list;

node *init_node(){
    node *new = (node *)malloc(sizeof(node));
    new->number = -1;
    return new;
}

// typedef struct Graph{
//     node **adj_list;
// }

bool check_new_atraction(char **name, char *s, int n){
    for(int i = 0; i < n; ++i) {
        if(strcmp(name[i], s) == 0) // daca exista deja
            return false;
    }
    return true;
}
int main(int argc, char const *argv[]) {
    FILE *in = fopen("tema3.in", "r");
    FILE *out = fopen("tema3.out", "w");

    // n = nr de obiective
    // m = nr de drumuri
    int n, m;

    fscanf(in, "%d %d\n", &n, &m);
    fprintf(out, "%d %d\n", n, m);

    // name[i] reprezinta stringul atractiei cu nr i
    char **name = (char **) malloc(n * sizeof(char *));
    char atraction[100];
    int temp_n = 0, cost;
    for(int i = 0; i < m; ++i) {
        fscanf(in, "%s ", atraction);
        if(check_new_atraction(name, atraction, temp_n)){
            printf("intra\n");
            // punem noua atractie;
            name[temp_n] = (char *)malloc(strlen(atraction) * sizeof(char));
            strcpy(name[temp_n], atraction);
            // crestem nr de atractii citite pana atunci
            temp_n++;
        }

        fscanf(in, "%s ", atraction);
        if(check_new_atraction(name, atraction, temp_n)){
            printf("intra\n");
            // punem noua atractie;
            name[temp_n] = (char *)malloc(strlen(atraction) * sizeof(char));
            strcpy(name[temp_n], atraction);
            // crestem nr de atractii citite pana atunci
            temp_n++;
        }

        fscanf(in, "%d\n", &cost);
    }
    for(int i = 0; i < n; ++i)
        fprintf(out, "%d %s\n", i, name[i]);
    fclose(in);
    fclose(out);

    return 0;
}
