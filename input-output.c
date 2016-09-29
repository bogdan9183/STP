//
// Created by bogdan on 14.05.2016.
//
#include "header.h"

void freeIntMatrix (int ** Matrix, int n){
    //Eliberare memorie pentru matrice ce tip int
    int i;
    for (i=0;i<n;i++)
        free(Matrix[i]);
    free(Matrix);
}
void freePorts (Switch * X, int n){
    // Eliberarea memoriei alocate tuturor porturilor
    Port *p;
    Port *q;
    int i;
    for (i=0;i<n;i++) {
    p = X[i].port;
        while (p != NULL) {
            q = p;
            p = p->next;
            free(q);
        }
    }
}

void freeTata (CostTata *list){
    // Eliberarea listei de costuri
    CostTata *p = list;
    CostTata *q;
    while (p != NULL) {
        q = p;
        p = p->next;
        free(q);
    }
}
void freeCharMatrix(char ** Matrix, int n) {
    //Eliberare memorie pentru matrice ce tip char
    int i;
    for (i = 0; i < n; i++)
        free(Matrix[i]);
    free(Matrix);
}
void freeRoad ( Road * list){
    // Eliberarea listei in care se retin drumuri intre switchuri
    Road *p = list;
    Road *q;
    while (p != NULL) {
        q = p;
        p = p->next;
        free(q);
    }
}


void recursivePrint (FILE* stream, Port * list,int i, char t) {
    // Afisarea listei pe principiul stivei
    if (list != NULL) {
        recursivePrint(stream, list->next, i, t);
        if (list->status == t)
            fprintf(stream, " %d(%d)", i, list->number);
    }
}

void PrintPort (FILE* stream, Switch * X, int n, char t){
    //Afisarea porturilor care au ca status caracterul t
    //Se apeleaza cu parametrul 'r' pentru root port
    //                          'b' pentru blocked port
    char x = toupper(t);
    fprintf(stream, "%cP:", x);
    int i;
    for (i=0;i<n;i++)
        recursivePrint(stream, X[i].port, i, t);
    fprintf(stream, "\n");
}

void PrintTopology(FILE* stream, char ** Topology, int n){
    // Afisarea matricei de topologie
    int i, j;
    for (i=0;i<n;i++){
        for (j=0;j<n-1;j++)
            fprintf(stream ,"%c ",Topology[i][j]);
        fprintf(stream, "%c\n", Topology[i][n-1]);
    }
}

int readSwitch(char *filename, Switch **vector) {
    // Citirea switchurilor
    FILE *initialise = fopen(filename, "r");
    char x[100];
    int n;
    fscanf(initialise, "%d", &n);
    *vector = calloc(n, sizeof(Switch));
    int k = 0;
    fgets(x, 100, initialise);
    while (fgets(x, 100, initialise) != NULL) {
        sscanf(x, "%d %d %s", &(*vector)[k].index, &(*vector)[k].priority,
               (*vector)[k].mac);
        (*vector)[k].port = NULL;
        k = k + 1;
    }
    fclose(initialise);
    return n;

}

void printRoad(FILE *stream, Road *lista) {
    // Afisarea drumului dintr-o lista de drumuri
    while (lista->next != NULL) {
        fprintf(stream, "%d ", lista->Nod);
        lista = lista->next;
    }
    fprintf(stream, "%d", lista->Nod);
}

void readCostMatrix(char *filename, int ***Matrix, int n, Switch *V) {
    // Citirea legaturilor din fisierul topologie
    FILE *in = fopen(filename, "r");
    *Matrix = (int **) malloc(n * sizeof(int *));
    int i;
    Cable aux;
    for (i = 0; i < n; i++)
        (*Matrix)[i] = calloc(n, sizeof(int));
    char buffer[100];
    while (fgets(buffer, 100, in)) {
        // Trecerea datelor intr-o structura de tip Cable
        sscanf(buffer, "%d %d %d %d %d", &aux.switch1, &aux.switch2, &aux.port1,
               &aux.port2, &aux.cost);
        // Convertirea costului in functie de tipul legaturii
        aux.cost = (aux.cost == 10) * 100 + (aux.cost == 100) * 19 +
                   (aux.cost == 1000) * 4 + (aux.cost == 10000) * 1;
        (*Matrix)[aux.switch1][aux.switch2] = aux.cost;
        (*Matrix)[aux.switch2][aux.switch1] = aux.cost;
        // Adaugarea celor 2 porturi in listele de porturi
        // ale fiecarui switch
        addPort(aux, &(V[aux.switch1].port));
        reverseLink(&aux);
        addPort(aux, &(V[aux.switch1].port));
    }
    fclose(in);
}
