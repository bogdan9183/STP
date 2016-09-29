#include "header.h"

int main ( int argc, char* argv[]){
    FILE *tasks, *out;
    Switch *vector;
    CostTata * Costuri = NULL;
    char ** Topology = NULL;
    int ** Matrix = NULL;
    int n = readSwitch(argv[1], &vector);
    int i = find_min(vector,n);
    char x[100];
    tasks = fopen(argv[3], "r");
    out = fopen(argv[4], "w");
    while (fgets(x, 100, tasks) != NULL) {
        if (strcmp(x, "c1\n") == 0)
            // Cerinta 1
            fprintf(out, "%d\n", i);
        if (strcmp(x, "c2-1\n")== 0) {
            // Cerinta 2-1
            if (Costuri == NULL) {
                readCostMatrix(argv[2], &Matrix, n, vector);
                ShortestPath(&Topology, vector, i, &Costuri, Matrix, n);
            }
                PrintPort(out, vector, n, 'r');
        }
        if (strcmp(x, "c2-2\n")== 0) {
            //Cerinta 2-2
            if (Costuri == NULL) {
                readCostMatrix(argv[2], &Matrix, n, vector);
                ShortestPath(&Topology, vector, i, &Costuri, Matrix, n);
            }
            PrintPort(out, vector, n, 'b');
        }
        if (strcmp(x, "c2-3\n")==0) {
            // Cerinta 2-3
            if (Costuri == NULL) {
                readCostMatrix(argv[2], &Matrix, n, vector);
                ShortestPath(&Topology, vector, i, &Costuri, Matrix, n);
            }
            PrintTopology(out, Topology, n);
        }
        if (strncmp(x, "c3", 2)==0){
                //Cerinta 3
                int A, B;
                sscanf(x, "%*s %d %d", &A, &B);
                if (Costuri == NULL) {
                    readCostMatrix(argv[2], &Matrix, n, vector);
                    ShortestPath(&Topology, vector, i, &Costuri, Matrix, n);
                }
                buildRoadAtoB(out, A, B, i, Costuri);

            }
        if (strncmp(x, "c4", 2) == 0) {
            //Cerinta 4
            int A, B;
            sscanf(x, "%*s %d %d", &A, &B);
            if (Costuri == NULL) {
                readCostMatrix(argv[2], &Matrix, n, vector);
                ShortestPath(&Topology, vector, i, &Costuri, Matrix, n);
            }
            //Stergerea legaturii si eliminarea porturilor corespunzatoare
            Matrix[A][B]=0;
            Matrix[B][A]=0;
            int remake = 0;
            Port *p = vector[A].port;
            while (p->destination != B) {
                p = p->next;
            }
            if (p->status == 'r') {
                remake = 1;
            }
            p->status = 't';
            p = vector[B].port;
            while (p->destination != A) {
                p = p->next;
            }
            if (p->status == 'r') {
                remake = 1;
            }
            p->status = 't';
            if (remake == 1){
                // Daca a fost sters un RootPort, se reface STP-ul.
                resetPorts(vector,n);
                freeTata(Costuri);
                freeCharMatrix(Topology, n);
                Costuri = NULL;
                ShortestPath(&Topology, vector, i, &Costuri, Matrix, n);
            }
        }
    }
    // Eliberarea memoriei la finalul programului
    if (Matrix!=NULL)
    freeIntMatrix(Matrix,n);
    if (Topology!=NULL)
    freeCharMatrix(Topology, n);
    if (vector[0].port!=NULL)
        freePorts(vector,n);
    if (Costuri!=NULL)
        freeTata(Costuri);
    free(vector);
    fclose(tasks);
    fclose(out);
}

