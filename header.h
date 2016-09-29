//
// Created by bogdan on 13.05.2016.
//

#ifndef TEMA3_SD_HEADER_H
#define TEMA3_SD_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define infinity 25000
typedef struct Port {
    int number;
    char status;
    int destination;
    struct Port * next;
    // x - Dont know
    // r - Root port
    // d - Designated
    // b - Blocked Port
} Port;

typedef struct Road {
    int Nod;
    struct Road* next;
} Road;

typedef struct Switch {
    int index;
    int priority;
    char mac[18];
    Port * port;
} Switch;

typedef struct Cable {
    int switch1, switch2, port1, port2;
    int cost;
} Cable;

typedef struct CostTata{
    int index;
    int cost;
    int father;
    struct CostTata * next;
} CostTata;

int readSwitch(char *, Switch **);
int find_min(Switch *, int );
void readCostMatrix(char *, int ***, int, Switch *);
int find_min(Switch *, int );
void reverseLink ( Cable *);
void addPort(Cable , Port** );
void initCostTata ( CostTata **, int );
void ShortestPath(char ***, Switch *, int , CostTata **, int **, int );
void ChangePort(Switch , int, char);
void PrintPort(FILE*, Switch *, int, char);
void PrintTopology ( FILE*, char** , int );
void buildRoadAtoB (FILE*, int, int,  int , CostTata *);
CostTata *getObj(CostTata *, int );
void resetPorts(Switch *, int);
void printRoad (FILE*, Road*);
void freeIntMatrix(int **, int );
void freeCharMatrix(char **, int);
void freePorts(Switch *, int );
void freeTata(CostTata *);
void freeRoad(Road *);
#endif //TEMA3_SD_HEADER_H
