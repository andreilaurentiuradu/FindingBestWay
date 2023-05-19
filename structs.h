#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// datele unei locatii
typedef struct Place {
    struct Place *next;
    int index, cost;
} place;

// harta
typedef struct Map {
    place *adj_list;
} map;

// drumurile dintre 2 locatii vecine
typedef struct Way {
    int cost, start, stop;
} way;

