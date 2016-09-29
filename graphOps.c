//
// Created by bogdan on 14.05.2016.
//

#include "header.h"

void AddToRoad ( Road ** head, int Node ){
    // Adaugarea nodului Node la lista de drum.
    if (*head == NULL) {
        *head = (Road *) malloc(sizeof(Road));
        (*head)->Nod = Node;
        (*head)->next = NULL;
    }
    else {
        Road *nou = malloc(sizeof(Road));
        nou->Nod = Node;
        nou->next = *head;
        *head = nou;
    }
}

void Reverse(Road **list) {
    /* Inversarea listei . Fiecare Road va fi plasat inaintea fostului sau
     * predecesor. In final, lista va avea pe prima pozitie ultimul Road, iar pe ultima
     * primul Road */
    Road *prev = NULL;
    Road *curent = (*list);
    Road *next;
    while (curent != NULL) {
        next = curent->next;
        curent->next = prev;
        prev = curent;
        curent = next;
    }
    (*list) = prev;
}

Road * findNode ( Road * list, int Node ){
    // Cauta si returneaza adresa nodului Node in lista de drum
    // In caz contrar, intoarce NULL
    Road * p = list;
    while (p!=NULL){
        if (p->Nod == Node)
            return p;
        p = p->next;
    }
    return NULL;
}
void buildList (Road ** list, int X, int root, CostTata * Tata){
    //Construieste lista drumului lui X catre root-bridge
    //Utilizand tatii din lista Tata
    CostTata * p = getObj(Tata, X);
    if (X == root) {
        AddToRoad(list, p->index);
    }
    while (p->index!=root){
        AddToRoad(list, p->index);
        p = getObj(Tata,p->father);
    }
    AddToRoad(list, root);
}

void buildRoadAtoB (FILE* stream, int A, int B, int root, CostTata * Tata){
    // Gasirea drumului de cost minim de la A la B
    // Foloseste drumurile catre root bridge calculate anterior
    Road * RoadA = NULL;
    Road * RoadB = NULL;
    buildList(&RoadA, A, root, Tata);
    buildList(&RoadB, B, root, Tata);
    Reverse(&RoadA);
    // Se inverseaza lista pentru a pune elementele in ordinea corecta
    Road *p;
    if (A == B){
        // Daca swichul de plecare este egal cu switchul final
        freeRoad(RoadA);
        freeRoad(RoadB);
        fprintf(stream, "%d %d\n",A,A);
    }
    else if ( findNode(RoadA, B)!= NULL){
        // Daca nodul B se gaseste pe drumul lui A catre rootbridge
        // Se elimina tot ce urmeaza dupa B in lista lui A
        // Se afiseaza restul drumului
        p = findNode(RoadA,B);
        freeRoad(p->next);
        freeRoad(RoadB);
        p->next = NULL;
        printRoad(stream, RoadA);
        freeRoad(RoadA);
        fprintf(stream, "\n");
    }
    else if (findNode(RoadB, A) != NULL) {
        // Daca A se afla in lista lui B
        // Se elimina tot ce se afla inaintea lui A
        // Se afiseaza lista incepand cu A, catre B.
            p=findNode(RoadB,A);
            printRoad(stream, p);
            freeRoad(RoadA);
            freeRoad(RoadB);
            fprintf(stream, "\n");
        }
        else {
        // Daca niciunul din noduri nu se afla in lista celuilalt
        // Se cauta noduri de intersectie pe drumurile catre rootbridge
            p = RoadA;
            while (p != NULL){
                // Se parcurge fiecare element din drumul lui A si se cauta in B
                // Daca se gaseste, se concateneaza cele doua liste si se afiseaza
                // drumul cautat.
            Road * x = findNode(RoadB, p->Nod);
                if ( x != NULL){
                     freeRoad(p->next);
                     p->next = NULL;
                     printRoad(stream, RoadA);
                     fprintf(stream, " ");
                     printRoad(stream, x->next);
                     fprintf(stream, "\n");
                     freeRoad(RoadA);
                     freeRoad(RoadB);
                     return ;
                }
                p = p->next;
        }

    }

}

void initCostTata ( CostTata ** list, int n){
    //Initializarea listei cu costuri si tati cu n elemente
    if (*list == NULL){
        *list = calloc(1, sizeof (CostTata));
        (*list)->next = NULL;
        (*list)->cost = infinity;
        (*list)->index = n-1;
        CostTata *p;
        int i;
        for (i=n-2;i>=0;i--){
            p = (CostTata*) calloc(1,sizeof(CostTata));
            p->index = i;
            p->cost = infinity;
            p->next = *list;
            *list = p;
        }
    }
}

void resetPorts( Switch * X, int n){
    // Resetarea statutului tuturor porturilor
    // in urma cerintei 4.
    Port *p;
    int i;
    for (i=0;i<n;i++){
        p = X[i].port;
        while(p!=NULL){
            if (p->status!='t')
            p->status = 'x';
            p=p->next;
        }
    }
}

CostTata * getObj (CostTata* list, int x){
    // Intoarce adresa elementului cu indice x din lista de costuri.
    int i = 0;
    CostTata * p = list;
    while (i<x) {
        i = i + 1;
        p = p->next;
    }
    return p;
}

int finishExploring (int *v, int n){
    // Verifica daca vectorul de vizitare contine noduri nevizitate.
    int i;
    for (i=0;i<n;i++)
        if (v[i]==0)
            return 0;
    return 1;
}

int findMinUnexplored ( int *v, CostTata *list, int n){
    // Cauta elementul de cost minim care nu a fost vizitat din lista de costuri
    int i, index, costMin = infinity;
    CostTata *p = list;
    for(i=0;i<n;i++){
        if (p->cost < costMin && v[i]==0) {
            index = i;
            costMin = p->cost;
        }
        p=p->next;
    }
    v[index] = 1;
    return index;
}

void fillPorts ( Switch * X, int n, CostTata * list ){
    // Completarea porturilor ramase in urma marcarii RP si DP.
    // Se parcurge lista de porturi a fiecarui switch
    // Daca un port are statut necunoscut, se fac comparatiile cerute si se
    // decide daca este DP sau BP. Portul aflat la capatul celeilalte legaturi
    // va primi statutul complementar.
    int i;
    Port *p;
    CostTata *start, *end;
    for (i=0;i<n;i++) {
        p = X[i].port;
        while (p!=NULL){
            if (p->status == 'x'){
                start = getObj(list, i);
                end = getObj(list, p->destination);
                if (start->cost < end->cost){
                    ChangePort(X[i], p->destination, 'd' );
                    ChangePort(X[p->destination], i , 'b');
                }
                else if (start->cost > end->cost) {
                        ChangePort(X[i], p->destination, 'b');
                        ChangePort(X[p->destination], i , 'd');
                    }
                     else if (X[i].priority < X[p->destination].priority){
                            ChangePort(X[i], p->destination, 'd');
                            ChangePort(X[p->destination], i , 'b');
                            }
                            else if (X[i].priority > X[p->destination].priority) {
                                ChangePort(X[i], p->destination, 'b');
                                ChangePort(X[p->destination], i , 'd');
                                }
                            else if (strcmp( X[i].mac, X[p->destination].mac)<0) {
                                      ChangePort(X[i], p->destination, 'd');
                                      ChangePort(X[p->destination], i , 'b');
                                    }
                                else {
                                     ChangePort(X[i], p->destination, 'b');
                                    ChangePort(X[p->destination], i , 'd');
                                }
                }
            p=p->next;
            }
        }
    }

void ShortestPath (char *** Topology, Switch* vector,int root, CostTata ** list, int **Matrix, int n){
    // Drumurile de cost minim de la root la toate celelalte switchuri
    // ale retelei
    // Alg. Dijkstra cu lista de costuri + vizitare.
    initCostTata(list, n);
    int * vizitat = calloc (n, sizeof(int));
    int i, j;
    (*Topology) = (char**) calloc ( n, sizeof(char*));
    for (i=0;i<n;i++)
        (*Topology)[i]= (char*)calloc(n,sizeof(char));
    for (i=0;i<n;i++)
        for(j=0;j<n;j++)
            (*Topology)[i][j]='0';
    CostTata * radacina = getObj(*list, root);
    CostTata * CTroot;
    CostTata * CTi;
    radacina -> cost = 0;
    while ( finishExploring(vizitat,n) == 0 ) {
        root = findMinUnexplored(vizitat, *list, n);
        CTroot = getObj(*list, root);
        for (i = 0; i < n; i++) {
            CTi = getObj(*list, i);
            if (Matrix[root][i] != 0) {
                if (CTi->cost > (CTroot->cost + Matrix[root][i])) {
                    CTi->cost = CTroot->cost + Matrix[root][i];
                    CTi->father = root;
                }
                else if (CTi->cost == (CTroot->cost + Matrix[root][i])) {
                    if (vector[CTi->father].priority > vector[root].priority)
                        CTi->father = root;
                    else if (vector[CTi->cost].priority ==
                             vector[root].priority) {
                        if (strcmp(vector[CTi->father].mac, vector[root].mac) >
                            0)
                            //CTi->father = i;
                            //else
                            CTi->father = root;
                    }
                }
            }
        }
    }
    radacina = getObj( *list, 0);
    //Se parcurg toate elementele listei si se marcheaza porturile RP
    //si cele DP aferente.
    while (radacina != NULL) {
        if (radacina->cost != 0 ){
            ChangePort( vector[radacina->index], radacina->father, 'r');
            (*Topology)[radacina->index][radacina->father]='1';
            ChangePort( vector[radacina->father], radacina->index, 'd');
        }
        radacina=radacina->next;
    }

    fillPorts(vector, n, *list);
    free(vizitat);

}

void ChangePort ( Switch X, int end, char t ) {
    //Modificarea portului care pleaca de la switchul X catre switchul End
    //si alocarea statutului transmis prin parametrul t
    Port *p = X.port;
    while (p->destination != end) {
        p=p->next;
    }
    if (p->status=='x')
    p->status = t;
}

void addPort (Cable Connection, Port **head){
    //Adaugarea unui port nou la lista
    if (*head == NULL) {
        *head = (Port *) malloc ( sizeof (Port));
        (*head)->destination = Connection.switch2;
        (*head)->number = Connection.port1;
        (*head)->status = 'x';
        (*head)->next = NULL;
    }
    else {
        Port * nou = malloc (sizeof(Port));
        nou->destination = Connection.switch2;
        nou->number = Connection.port1;
        nou->status = 'x';
        nou->next = *head;
        *head = nou;
    }
}

void reverseLink (Cable * Connection){
    //Inversarea porturilor si switchurilor unei anumite legaturi
    int pahar;
    pahar = Connection->port1;
    Connection->port1 = Connection->port2;
    Connection->port2 = pahar;
    pahar = Connection->switch1;
    Connection->switch1 = Connection->switch2;
    Connection->switch2 = pahar;

}

int find_min(Switch *T, int size) {
    //Cautarea switchului cu prioritate minima / MAC minim
    Switch minim = T[0];
    int i;
    if (size>1) {
        for (i = 1; i < size; i++) {
            if (T[i].priority < minim.priority)
                minim = T[i];
            else if (minim.priority == T[i].priority)
                      if (strcmp(T[i].mac, minim.mac) < 0)
                            minim = T[i];
        }
    }
    return minim.index;
}

